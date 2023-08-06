#include "SDSF/Sprite.hpp"

#include "SDSF/Matrix.hpp"

#include "SDSF/Utils.hpp"

std::array<bool, OAM_SIZE> Sprite::indexesMain = {};
std::array<bool, OAM_SIZE> Sprite::indexesSub = {};

Sprite::Sprite(Engine engine)
	:	oam(engine == Engine::Main ? oamMain : oamSub), engine(engine)
{
    
}

void Sprite::SetVisible(bool visible)
{
	entry->isHidden = !visible;
}

Sprite::~Sprite()
{
    if(created)
        Reset();
}

void Sprite::Create(ObjSize size, ObjShape shape, ObjColMode colorFormat)
{
	if(index != u8(-1)) {
		Reset();
	}

	index = AllocateOAMIndex(engine);
	entry = &oam.oamMemory[index];

    ResetEntry();

    rotation = fixed8(0);
    inverseScale = {fixed8(1), fixed8(1)};

    entry->size = size;
    // This is bad
    if(colorFormat != static_cast<ObjColMode>(3)) {
        entry->colorMode = colorFormat;
    }else {
        entry->blendMode = OBJMODE_BITMAP;
        entry->colorMode = OBJCOLOR_16;
    }
	entry->shape = shape;

    created = true;
}

void Sprite::ResetEntry() {
	*entry = DefaultEntry();
    created = false;
    rotation = fixed8(0);
    inverseScale = {fixed8(1), fixed8(1)};
}

void Sprite::Reset()
{
    ResetEntry();
	
	if(index != -1) {
		FreeOAMIndex(engine, index);
	}
}

SpriteEntry Sprite::DefaultEntry()
{
    SpriteEntry newEntry;

    newEntry.blendMode = OBJMODE_NORMAL;
    newEntry.colorMode = OBJCOLOR_16;
    newEntry.gfxIndex = 0;
    newEntry.hFlip = false;
    newEntry.isHidden = false;
    newEntry.isMosaic = false;
    newEntry.isRotateScale = false;
    newEntry.palette = 0;
    newEntry.priority = OBJPRIORITY_0;
    newEntry.shape = OBJSHAPE_SQUARE;
    newEntry.size = OBJSIZE_16;
    newEntry.vFlip = false;
    newEntry.x = 0;
    newEntry.y = 0;

    return newEntry;
}

void Sprite::EnableAffineMatrix(u8 matrixID)
{
    entry->isRotateScale = true;
    entry->rotationIndex = matrixID;
}

void Sprite::DisableAffineMatrix()
{
    entry->isRotateScale = false;
}

void Sprite::SetSizeDouble(bool sizeDouble)
{
    entry->isSizeDouble = sizeDouble;
}

void Sprite::SetPalette(u16 palIndex)
{
    entry->palette = palIndex;
}

void Sprite::SetPalette(const Palette& palette)
{
    entry->palette = palette.GetIndex();
}

void Sprite::SetPosition(Vector2<u16> position)
{
    entry->x = position.x;
    entry->y = position.y;
}

void Sprite::Move(Vector2<u16> amount)
{
    entry->x += amount.x;
    entry->y += amount.y;
}

void Sprite::SetRotation(fixed8 rotation)
{
    this->rotation = rotation;
}

void Sprite::Rotate(fixed8 amount)
{
    this->rotation += amount;
}

void Sprite::SetScale(Vector2<fixed8> scale)
{
    inverseScale = {fixed8(1) / scale.x, fixed8(1) / scale.y};
}

void Sprite::UpdateMatrix()
{
    SpriteRotation& rotation = oam.oamRotationMemory[entry->rotationIndex];

    fixed8 sinResult = fpm::sin(fixed8(this->rotation));
    fixed8 cosResult = fpm::cos(fixed8(this->rotation));

    rotation.hdx = (inverseScale.x * cosResult).raw_value();
    rotation.hdy = (inverseScale.y * sinResult).raw_value();
    rotation.vdx = (-inverseScale.x * sinResult).raw_value();
    rotation.vdy = (inverseScale.y * cosResult).raw_value();
}

void Sprite::SetInverseScale(Vector2<fixed8> scale)
{
    inverseScale = scale;
}

void Sprite::InitOAM(Engine engine, SpriteMapping mapping, bool extPalette)
{
	OamState& oam = engine == Engine::Main ? oamMain : oamSub;
	std::array<bool, OAM_SIZE>& oamArray = engine == Engine::Main ? indexesMain : indexesSub;

	oamInit(&oam, mapping, extPalette);

	oamArray.fill(false);
}

Vector2<u16> Sprite::GetPosition() const {
    return {entry->x, entry->y};
}

void Sprite::SetMosaic(bool mosaicEnabled)
{
	entry->isMosaic = mosaicEnabled;
}

void Sprite::SetVFlip(bool vFlip)
{
	entry->vFlip = vFlip;
}

void Sprite::SetHFlip(bool hFlip)
{
	entry->hFlip = hFlip;
}

void Sprite::SetTexture(u16 gfxIndex)
{
	entry->gfxIndex = gfxIndex;
}

void Sprite::SetTexture(const Texture& texture)
{
	SetTexture(texture.getSlot());
}

u16 Sprite::getGfxIndex() const {
	return gfxIndex;
}

u8 Sprite::AllocateOAMIndex(Engine engine)
{
	std::array<bool, OAM_SIZE>& indexes = engine == Engine::Main ? indexesMain : indexesSub;

	for(int i = 0; i < OAM_SIZE; i++) {
		if(!indexes[i]) {
			indexes[i] = true;

			return i;
		}
	}

	return -1;
}

void Sprite::FreeOAMIndex(Engine engine, u8 index)
{
	if(index < OAM_SIZE) {
		std::array<bool, OAM_SIZE>& indexes = engine == Engine::Main ? indexesMain : indexesSub;

		indexes[index] = false;
	}
}

void Sprite::Create(SpriteSize size, SpriteColorFormat colorFormat) {
	Create(static_cast<ObjSize>(SPRITE_SIZE_SIZE(size)), static_cast<ObjShape>(SPRITE_SIZE_SHAPE(size)), static_cast<ObjColMode>(colorFormat));
}