#include "SDSF/Texture.hpp"

#include "SDSF/FileReader.hpp"

#include <filesystem>

Texture::Texture(Engine engine)
    :   engine(engine)
{
	
}

u32 Texture::CalculateTextureSize(SpriteSize spriteSize, SpriteColorFormat colorMode)
{
	u16 size = SPRITE_SIZE_PIXELS(spriteSize);

	switch(colorMode) {
		case SpriteColorFormat_16Color:
			// 4 bits per pixel, so divide by two
			size = size >> 1;
			break;
		case SpriteColorFormat_256Color:
			// 8 bits per pixel, do nothing
			break;
		case SpriteColorFormat_Bmp:
			// 16 bits per pixel, so multiply by two
			size = size << 1;
			break;
	}

	return size;
}

void Texture::loadTexture(const std::string& filename, SpriteSize spriteSize, SpriteColorFormat colorMode, u16 offsetBytes)
{
	FileReader reader;
	reader.OpenFile(filename);
	reader.Seek(offsetBytes);

	const u16 pixels = GetNumberOfPixels(spriteSize);

	u16 sizeInHalfWords = 0;

	if(colorMode == SpriteColorFormat_16Color) {
		// Number of pixels will always be a multiple of 4, so we can easily divide by 4 here
		sizeInHalfWords = pixels >> 2;
	}else if(colorMode == SpriteColorFormat_256Color) {
		sizeInHalfWords = pixels >> 1;
	}else if(colorMode == SpriteColorFormat_Bmp) {
		sizeInHalfWords = pixels;
	}

	std::vector<u16> data = reader.ReadNumbers<u16>(sizeInHalfWords);

	loadTextureFromMemory(&data[0], spriteSize, colorMode);
}

void Texture::loadTextureFromMemory(const u16* const texture, SpriteSize spriteSize, SpriteColorFormat colorMode)
{
	unloadTexture();

	OamState* oam = &oamMain;
	if(engine == Engine::Sub) {
		oam = &oamSub;
	}

	this->pointer = oamAllocateGfx(oam, spriteSize, colorMode);
	this->gfxSlot = oamGfxPtrToOffset(oam, pointer);

	size = GetNumberOfPixels(spriteSize);

	if(colorMode == SpriteColorFormat_16Color) {
		// Divide size by 2
		size = size >> 1;
	}if(colorMode == SpriteColorFormat_Bmp) {
		// Multiply size by 2
		size = size << 1;
	}

	DC_FlushAll();
	dmaCopyHalfWords(TEXTURE_DMA_CHANNEL, texture, pointer, size);
}

u16 Texture::getSlot() const
{
    return gfxSlot;
}

Texture::~Texture() {
	unloadTexture();
}

void Texture::unloadTexture() {
	if(pointer != nullptr) {
		oamFreeGfx(engine == Engine::Main ? &oamMain : &oamSub, pointer);
		pointer = nullptr;
	}
}

u16 Texture::getSize() const
{
	return size;
}

u16* Texture::getPointer() const
{
	return pointer;
}
