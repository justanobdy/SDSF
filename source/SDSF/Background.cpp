#include "SDSF/Background.hpp"

#include "SDSF/ResourceCreator.hpp"
#include "SDSF/Utils.hpp"
#include "SDSF/FileReader.hpp"

BackgroundBase::BackgroundBase(Engine engine, u8 backgroundID)
    :   engine(engine), layer(backgroundID)
{

}

void BackgroundBase::LoadGfx(const std::string& filename, int tilesToLoad, u16 offsetFromStart)
{
    ResourceCreator::LoadTilemap(filename, tilesToLoad, gfxPtr);
}

int BackgroundBase::ConvertVectorToIndex(Vector2<u16> vec) const
{
    return vec.x + (vec.y * (size.y / 8));
}

void BackgroundBase::Scroll(Vector2<int> amount)
{
    bgScroll(id, amount.x, amount.y);
    scroll.x += amount.x;
    scroll.y += amount.y;
}

void BackgroundBase::SetScroll(Vector2<int> scroll)
{
    bgSetScroll(id, scroll.x, scroll.y);
    this->scroll = scroll;
}

const Vector2<int>& BackgroundBase::GetScroll() const
{
    return scroll;
}

void BackgroundBase::Rotate(int amount)
{
    bgRotate(id, amount);
    rotation += amount;
}

void BackgroundBase::SetRotation(int angle)
{
    bgSetRotate(id, angle);
    rotation = angle;
}

int BackgroundBase::GetRotation() const
{
    return rotation;
}

void BackgroundBase::Scale(Vector2<fpm::fixed_24_8> amount)
{
    SetScale({scale.x + amount.x, scale.y + amount.y});
}

void BackgroundBase::SetScale(Vector2<fpm::fixed_24_8> scale)
{
    this->scale = scale;

    bgSetScale(id, (fpm::fixed_24_8(1) / scale.x).raw_value(), (fpm::fixed_24_8(1) / scale.y).raw_value());
}

const Vector2<fpm::fixed_24_8>& BackgroundBase::GetScale() const
{
    return scale;
}

void BackgroundBase::SetMosaicEnabled(bool enabled)
{
    if(enabled) {
        bgMosaicEnable(id);
    }else {
        bgMosaicDisable(id);
    }
}

void BackgroundBase::SetVisible(bool visible)
{
    if(visible) {
        bgShow(id);
    }else {
        bgHide(id);
    }
}

void BackgroundBase::SetWrapEnabled(bool enabled)
{
    if(enabled) {
        bgWrapOn(id);
    }else {
        bgWrapOff(id);
    }
}

void BackgroundBase::SetAffineMatrix(int hdx, int vdx, int hdy, int vdy)
{
    bgSetAffineMatrixScroll(id, hdx, vdx, hdy, vdy, scroll.x, scroll.y);
}

void BackgroundBase::SetPriority(u8 priority)
{
    this->priority = priority;
    bgSetPriority(id, priority);
}

u8 BackgroundBase::GetPriority() const
{
    return priority;
}

void BackgroundBase::Create(BgType type, BgSize size, u16 mapBase, u16 tileBase)
{
    if(engine == Engine::Main) {
        id = bgInit(layer, type, size, mapBase, tileBase);
    }else {
        id = bgInitSub(layer, type, size, mapBase, tileBase);
    }

    if(type == BgType_Rotation || type == BgType_Bmp8) {
        bitsPerTile = 8;
    }else {
        bitsPerTile = 16;
    }

    if(type == BgType_Bmp8 || type == BgType_Bmp16) {
        if(engine == Engine::Main) {
            gfxPtr = BG_BMP_RAM(mapBase);
        }else {
            gfxPtr = BG_BMP_RAM_SUB(mapBase);
        }
    }else {
        gfxPtr = bgGetGfxPtr(id);
        mapPtr = bgGetMapPtr(id);
    }

    this->size = Utils::BgSizeToVectorPixels(size);
}

void BackgroundBase::SetRotationCenter(Vector2<fpm::fixed_24_8> center) {
    bgSetCenterf(id, center.x.raw_value(), center.y.raw_value());
}

/*void BackgroundBase::SetTile(int x, int y, int tileID)
{
    Utils::SetBGIndex(mapPtr, {x, y}, tileID, {size.x / 8, size.y / 8}, bitsPerTile);
}*/

u16 BackgroundBase::GetTile(u16 x, u16 y) const {
	return Utils::GetTileIndex(mapPtr, {x, y}, {(size.x / 8), (size.y / 8)}, bitsPerTile);
}

u16* BackgroundBase::GetMapPtr()
{
	return mapPtr;
}

u16* BackgroundBase::GetGfxPtr()
{
	return gfxPtr;
}

void BackgroundBase::LoadMap(const std::string& filename, u32 offsetFromStart) {
	FileReader reader(filename);
	
	reader.Seek(offsetFromStart);

	std::vector<u16> mapData = reader.ReadNumbers<u16>(reader.GetBytesLeft() / 2);

	DC_FlushRange(&mapData[0], mapData.size() * 2);
	dmaCopyHalfWords(3, &mapData[0], mapPtr, mapData.size() * 2);
}

void BackgroundBase::LoadGfx(const u16* const data, u16 bytesToLoad) {
	DC_FlushRange(data, bytesToLoad);
	dmaCopyHalfWords(3, data, gfxPtr, bytesToLoad);
}

void BackgroundBase::LoadMap(const u16* const data, u16 bytesToLoad) {
	DC_FlushRange(data, bytesToLoad);
	dmaCopyHalfWords(3, data, mapPtr, bytesToLoad);
}