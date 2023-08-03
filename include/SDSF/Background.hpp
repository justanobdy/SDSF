/********************************************************
 * 					READ THIS
 * 
 * This header file is a mess, 
 * I did some crazy stuff to:
 * (1) Split the definition and implementation and definition as much as possible
 * (2) Have it use inheritance and templates as much as possible
 * 
 * I'm not the best programmer, so this most likely isn't
 * the best way to do it, but using it isn't so bad, and maintaining it
 * shouldn't be too bad either, since you only need to change 
 * one function for most changes (Since there is so much inheritance).
 * 
 * If anyone has a better way of doing this, please show me!
***************************************************/

#pragma once

#include <vector>
#include <nds.h>
#include <string>

#include "Common.hpp"
#include "Vector2.hpp"
#include "fpm/fpm.hpp"
#include "Utils.hpp"

/// @brief Separate class which contains code that does not need to be templated
class BackgroundBase {
public:
    /// @brief Constructor
    /// @param engine The engine this background will be on
    /// @param layer The background layer which this will be on (0-3)
    BackgroundBase(Engine engine, u8 layer);

    /// @brief Load graphics from a file
    /// @param filename The filename
    /// @param tilesToLoad The amount of 8x8 sprites to load
    /// @param offsetFromStart The offset from the start of the file
    void LoadGfx(const std::string& filename, int tilesToLoad = 256, u16 offsetFromStart = 0);
	/// @brief Load graphics from an array
	/// @param data The graphics data
	/// @param bytesToLoad The amount of bytes to load
	void LoadGfx(const u16* const data, u16 bytesToLoad);

	/// @brief Load map data (you currently cannot create infinite maps using this)
	/// @param filename The filename to load from
	/// @param offsetFromStart The offset from the start of the file to read from
	void LoadMap(const std::string& filename, u32 offsetFromStart = 0);
	/// @brief Load map data from an array (you currently cannot create infinite maps using this)
	/// @param data The map  data
	/// @param bytesToLoad The amount of bytes to load
	void LoadMap(const u16* const data, u16 bytesToLoad);

    /// @brief Convert a Vector2 to a index to use for accessing the map data
    /// @param vec The vector to convert to an index
    /// @return The calculated index
    int ConvertVectorToIndex(Vector2<u16> vec) const;

    /// @brief Scroll the background
    /// @param amount The amound to scroll the background
    void Scroll(Vector2<int> amount);
    /// @brief Set the scroll of the background
    /// @param scroll The scroll of the background
    void SetScroll(Vector2<int> scroll);

    // Add scrollf variants?

    /// @brief Get the scroll amount of the background
    /// @return The scroll amount of the background
    const Vector2<int>& GetScroll() const;

    /// @brief Enable/disable mosaic for this background
    /// @param enabled The new value
    void SetMosaicEnabled(bool enabled);

    /// @brief Set if the background is visible
    /// @param visible The new value
    void SetVisible(bool visible);

    /// @brief Enable/Disable wrapping for this background
    /// @param enabled The new value
    void SetWrapEnabled(bool enabled);

    /// @brief Set the new affine matrix values (only use if you know what you are doing)
    /// @param hdx The new hdx value
    /// @param vdx The new vdx value
    /// @param hdy The new hdy value
    /// @param vdy The new vdy value
    void SetAffineMatrix(int hdx, int vdx, int hdy, int vdy);

    /// @brief Set the priority of the background
    /// @param priority The new priority (0 = highest, 3 = lowest)
    void SetPriority(u8 priority);
    /// @brief Get the priority of the background
    /// @return The priority of the background
    u8 GetPriority() const;

	/// @brief Get a tile at a specific position
	/// @param x The x position of the tile (in tiles)
	/// @param y The y position of the tile (in tiles)
	/// @return The tile data at that position
	u16 GetTile(u16 x, u16 y) const;

	/// @brief Returns the pointer to the map (only use if you know what you are doing!)
	/// @return The pointer to the map
	u16* GetMapPtr();
	/// @brief Returns the pointer to the graphics (only use if you know what you are doing!)
	/// @return The pointer to the graphics
	u16* GetGfxPtr();

    BackgroundBase(BackgroundBase&) = delete;
    BackgroundBase(BackgroundBase&&) = delete;
protected:
    void Create(BgType type, BgSize size, u16 mapBase, u16 tileBase);

    void Rotate(int amount);
    void SetRotation(int angle);

    int GetRotation() const;

    void Scale(Vector2<fpm::fixed_24_8> amount);
    void SetScale(Vector2<fpm::fixed_24_8> scale);

    void SetRotationCenter(Vector2<fpm::fixed_24_8> center);

    const Vector2<fpm::fixed_24_8>& GetScale() const;

    const Engine engine;
    const u8 layer = 0;
    u8 id = 255;
    u8 priority = 0;

    u8 bitsPerTile = 16;

    Vector2<u16> size = {0, 0};
    Vector2<fpm::fixed_24_8> scale = {fpm::fixed_24_8(0), fpm::fixed_24_8(0)};
    Vector2<int> scroll = {0, 0};
    int rotation;

    u16* mapPtr = nullptr;
    u16* gfxPtr = nullptr;    
};

/// @brief Class which contains a collection of tiles that make up a larger tile
/// @tparam tileType The type of tile it has (TileMapEntry8 or TileMapEntry16)
template <typename tileType>
struct BackgroundTile {
    Vector2<u8> size;
    /// @brief The tiles indexes that make up this tile.
    ///        Make sure tiles.size() == (size.x / 8) * (size.y / 8)
    std::vector<tileType> tiles;
};

/// @brief Class for putting multiple 8-bit tiles together
using BackgroundTile8 = BackgroundTile<TileMapEntry8>;
/// @brief Class for putting multiple 16-bit tiles together
using BackgroundTile16 = BackgroundTile<TileMapEntry16>;

/// @brief Type for Non-rotatable backgrounds
/// @tparam tileType The type of TileMapEntry it is (TileMapEntry8 or TileMapEntry16)
/// @tparam convertedType The type that the background tiles are supposed to be converted to (u8 or u16)
/// @tparam type The type of background (BMP or Text background)
template <BgType type, typename tileType, typename convertedType>
class NonRotatableBackground : public BackgroundBase {
public:
    using BackgroundBase::BackgroundBase;

    /// @brief Create the background
    /// @param size The size of the background
    /// @param mapBase The 2k offset into ram where map data will be placed OR the 16k offset into vram the bitmap data will be placed for bitmap backgrounds 
    /// @param tileBase The 16k offset into vram the tile graphics data will be placed (unused for bitmap backgrounds)
    void Create(BgSize size, u16 mapBase, u16 tileBase) {
        BackgroundBase::Create(type, size, mapBase, tileBase);
    }

    /// @brief Set a tile at a given position
    /// @param x The x position (in tiles)
    /// @param y The y position (in tiles)
    /// @param tile The new value
    void SetTile(u16 x, u16 y, convertedType tile) {
        Utils::SetBGIndex(mapPtr, {x, y}, tile, {size.x / 8, size.y / 8}, sizeof(convertedType) * 8);
    }

    /// @brief Set a tile at a given position
    /// @param x The x position (in tiles)
    /// @param y The y position (in tiles)
    /// @param tile The new value
    void SetTile(u16 x, u16 y, tileType tile) {
        SetTile(x, y, *reinterpret_cast<convertedType*>(&tile));
    }

    /// @brief Set a collection of tiles down
    /// @param x The x position (in tiles)
    /// @param y The y position (in tiles)
    /// @param tile The collection of tiles to put down
    void SetTile(u16 x, u16 y, const BackgroundTile<tileType>& tile) {
        int16 i = 0;
        
        for(u16 offy = 0; offy < tile.size.y; offy++) {
            for(u16 offx = 0; offx < tile.size.x; offx++) {
                SetTile(offx + x, offy + y, tile.tiles[i]);
                i++;
            }
        }
    }
};

/// @brief Type for Rotatable backgrounds
/// @tparam tileType The type of TileMapEntry it is (TileMapEntry8 or TileMapEntry16)
/// @tparam convertedType The type that the background tiles are supposed to be converted to (u8 or u16)
/// @tparam type The type of background to make (Rotation or Extended Rotation)
template <BgType type, typename tileType, typename convertedType>
class RotatableBackground : public NonRotatableBackground<type, tileType, convertedType> {
public:
    using NonRotatableBackground<type, tileType, convertedType>::NonRotatableBackground;
    using BackgroundBase::Rotate;
    using BackgroundBase::SetRotation;
    using BackgroundBase::GetRotation;
    using BackgroundBase::Scale;
    using BackgroundBase::SetScale;
    using BackgroundBase::GetScale;
    using BackgroundBase::SetRotationCenter;
};

/// @brief Class for Rotation Background
using RotBackground = RotatableBackground<BgType_Rotation, TileMapEntry8, u8>;
/// @brief Class for Extended Rotation Backgrounds
using ExRotbackground = RotatableBackground<BgType_ExRotation, TileMapEntry16, u16>;
/// @brief Class for 8bpp Text Backgrounds
using T8Background = NonRotatableBackground<BgType_Text8bpp, TileMapEntry16, u16>;
/// @brief Class for 4bpp Text Backgrounds
using T4Background = NonRotatableBackground<BgType_Text4bpp, TileMapEntry16, u16>;
/// @brief Class for 8bpp BMP Backgrounds
using BMP8Background = NonRotatableBackground<BgType_Bmp8, TileMapEntry8, u8>;
/// @brief Class for 16bpp BMP Backgrounds
using BMP16Background = NonRotatableBackground<BgType_Bmp16, TileMapEntry16, u16>;