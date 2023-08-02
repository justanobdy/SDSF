#include <SDSF/MultiSprite.hpp>

#include <filesystem>

#include <SDSF/FileReader.hpp>
#include <SDSF/Utils.hpp>

MultiSprite::MultiSprite(Engine engine)	
	:	engine(engine)
{

}

void MultiSprite::Create(const std::string& filename, SpriteSize sizePerSprite, SpriteColorFormat colorFormat, Vector2<u8> spriteNumber)
{
	// Verify that there is enough info in the file to fill all the sprites
	u8 bpp = 0;

	switch(colorFormat) {
		case SpriteColorFormat_16Color:
			bpp = 4;
			break;
		case SpriteColorFormat_256Color:
			bpp = 8;
			break;
		case SpriteColorFormat_Bmp:
			bpp = 16;
			break;
	}

	const u32 expectedSizePerSpriteInBytes = (SPRITE_SIZE_PIXELS(sizePerSprite) * bpp) / 8;

	const u32 expectedFileSizeInBytes = expectedSizePerSpriteInBytes * (spriteNumber.x * spriteNumber.y);

	if(std::filesystem::file_size(filename) < expectedFileSizeInBytes) {
		// This should throw an exception
		return;
	}
	
	FileReader reader(filename);
	
	std::vector<u16> data = reader.ReadEntireFile<u16>();

	reader.Close();

	this->spriteNumber = spriteNumber;
	this->size = Utils::SpriteSizeToVectorPixels(sizePerSprite);

	sprites.reserve(spriteNumber.x * spriteNumber.y);
	textures.reserve(spriteNumber.x * spriteNumber.y);

	for(int i = 0; i < spriteNumber.x * spriteNumber.y; i++) {
		sprites.push_back(Sprite(engine));
		textures.push_back(Engine(engine));

		textures[i].loadTextureFromMemory(&data[0], sizePerSprite, colorFormat);
		data.erase(data.begin(), data.begin() + (textures[i].getSize() / 2));

		sprites[i].Create(sizePerSprite, colorFormat);
		sprites[i].SetTexture(textures[i]);
	}

	SetPosition({0, 0});
}

void MultiSprite::SetPosition(Vector2<u16> newPosition)
{
	position = newPosition;

	for(int y = 0; y < spriteNumber.y; y++) {
		for(int x = 0; x < spriteNumber.x; x++) {
			int index = x + (y * spriteNumber.x);
			sprites[index].SetPosition({position.x + (x * size.x), position.y + (y * size.y)});
		}
	}
}

void MultiSprite::Move(Vector2<u16> amount)
{
	SetPosition({position.x + amount.x, position.y + amount.y});
}

Vector2<u16> MultiSprite::GetPosition() const
{
	return position;
}

void MultiSprite::SetPalette(u16 palIndex)
{
	for(Sprite& sprite : sprites) {
		sprite.SetPalette(palIndex);
	}
}

void MultiSprite::SetVisible(bool visible)
{
	for(Sprite& sprite : sprites) {
		sprite.SetVisible(visible);
	}
}

/*void MultiSprite::SetVFlip(bool vFlip)
{
	for(Sprite& sprite : sprites) {
		sprite.SetVFlip(vFlip);
	}
}

void MultiSprite::SetHFlip(bool hFlip)
{
	for(Sprite& sprite : sprites) {
		sprite.SetHFlip(hFlip);
	}
}*/