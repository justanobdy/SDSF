#include "SDSF/Utils.hpp"

#include <cmath>

#include <iostream>

Vector2<u16> Utils::BgSizeToVectorPixels(BgSize size)
{
    switch(size) {
        case BgSize_B16_128x128:
        case BgSize_B8_128x128:
        case BgSize_ER_128x128:
        case BgSize_R_128x128:
            return Vector2<u16>(128, 128);
            break;
        case BgSize_R_256x256:
        case BgSize_ER_256x256:
        case BgSize_T_256x256:
        case BgSize_B8_256x256:
        case BgSize_B16_256x256:
            return Vector2<u16>(256, 256);
            break;
        case BgSize_R_512x512:
        case BgSize_T_512x512:
        case BgSize_B8_512x512:
        case BgSize_B16_512x512:
        case BgSize_ER_512x512:
            return Vector2<u16>(512, 512);
            break;
        case BgSize_R_1024x1024:
        case BgSize_ER_1024x1024:
            return Vector2<u16>(1024, 1024);
            break;
        case BgSize_T_256x512:
            return Vector2<u16>(256, 512);
            break;
        case BgSize_T_512x256:
        case BgSize_B8_512x256:
        case BgSize_B16_512x256:
            return Vector2<u16>(512, 256);
            break;
        case BgSize_B8_512x1024:
            return Vector2<u16>(512, 1024);
            break;
        case BgSize_B8_1024x512:
            return Vector2<u16>(1024, 512);
            break;
        default:
            return Vector2<u16>(11, 11);
            break;
    }
}

Vector2<u16> Utils::BgSizeToVectorTiles(BgSize size)
{
    Vector2<u16> vector = Utils::BgSizeToVectorPixels(size);

    return {vector.x / 8, vector.y / 8};
}

void Utils::SetBGIndex(u16* const mapPtr, Vector2<u16> tilePosition, u16 newValue, Vector2<u16> mapSize, u8 bitsPerTile)
{
    int index = tilePosition.x + (tilePosition.y * mapSize.y);

    if(bitsPerTile == 8) {
		index = index >> 1;

		if(IsEven(tilePosition.x)) {
			mapPtr[index] = newValue | (mapPtr[index] & 0xFF00);
		}else {
			mapPtr[index] = newValue << 8 | (mapPtr[index] & 0xFF);
		}
    }else {
        mapPtr[index] = newValue;
    }
}

u16 Utils::GetTileIndex(u16* const mapPtr, Vector2<u16> tilePosition, Vector2<u16> mapSize, u8 bitsPerTile)
{
	int index = tilePosition.x + (tilePosition.y * mapSize.y);

	if(bitsPerTile == 8) {
		index = index >> 1;

		if(IsEven(tilePosition.x)) {
			return mapPtr[index] & 0xFF;
		}else {
			return (mapPtr[index] & 0xFF00) >> 8;
		}
	}else {
		return mapPtr[index];
	}
}

bool Utils::CheckAABB(Vector2<int> firstPosition, Vector2<u8> firstSize, Vector2<int> secondPosition, Vector2<u8> secondSize)
{
	return firstPosition.x < secondPosition.x+secondSize.x &&
         secondPosition.x < firstPosition.x+firstSize.x &&
         firstPosition.y < secondPosition.y+secondSize.y &&
         secondPosition.y < firstPosition.y+firstSize.y;
}

void Utils::VRAMWrite8Bit(u8 newValue, u16 *pointer)
{
	if(IsEven((int)pointer)) {
		// Write to the lower portion of memory
		*pointer = newValue | (*pointer & 0xFF00);
	}else {
		// Write to the upper portion of memory
		*pointer = (*pointer & 0xFF) | newValue >> 8;
	}
}

Vector2<u8> Utils::SpriteSizeToVectorPixels(SpriteSize size)
{
	return SpriteSizeToVectorPixels(static_cast<ObjShape>(SPRITE_SIZE_SHAPE(size)), static_cast<ObjSize>(SPRITE_SIZE_SIZE(size)));
}

Vector2<u8> Utils::SpriteSizeToVectorPixels(ObjShape shape, ObjSize size)
{
	const u8 pixelCount = ObjSizeToPixels(size);

	switch(shape) {
		case OBJSHAPE_SQUARE:
			return {pixelCount, pixelCount};
		case OBJSHAPE_WIDE:
			switch(size) {
				case OBJSIZE_8:
					return {16, 8};
				case OBJSIZE_16:
					return {32, 8};
				case OBJSIZE_32:
					return {32, 16};
				case OBJSIZE_64:
					return {64, 32};
			}
		case OBJSHAPE_TALL:
			switch(size) {
				case OBJSIZE_8:
					return {8, 16};
				case OBJSIZE_16:
					return {8, 32};
				case OBJSIZE_32:
					return {16, 32};
				case OBJSIZE_64:
					return {32, 64};
			}
	}
}

u8 Utils::ObjSizeToPixels(ObjSize size)
{
	switch(size) {
		case OBJSIZE_8:
			return 8;
		case OBJSIZE_16:
			return 16;
		case OBJSIZE_32:
			return 32;
		case OBJSIZE_64:
			return 64;
	}
}
