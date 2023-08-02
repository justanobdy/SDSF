#pragma once

#include <nds.h>

#include "Vector2.hpp"

#define Fixed8ToFixed2012(num) num << 4
#define Fixed2012ToFixed8(num) num >> 4

#define IsOdd(num) ((num) & 1)
#define IsEven(num) !IsOdd(num)

namespace Utils
{
    /// @brief Convert a BgSize to a vector (in pixels)
    /// @param size The BgSize
    /// @return The size (in pixels)
    Vector2<u16> BgSizeToVectorPixels(BgSize size);
    /// @brief Convert a BgSize to a vector (in tiles)
    /// @param size The BgSize
    /// @return The size (in tiles)
    Vector2<u16> BgSizeToVectorTiles(BgSize size);

	/// @brief Convert a SpriteSize to a vector (in pixels)
	/// @param size The SpriteSize
	/// @return The size (in pixels)
	Vector2<u8> SpriteSizeToVectorPixels(SpriteSize size);
	/// @brief Convert an ObjShape & ObjSize pair to a vector
	/// @param shape The ObjShape
	/// @param size The ObjSize
	/// @return The size (in pixels)
	Vector2<u8> SpriteSizeToVectorPixels(ObjShape shape, ObjSize size);

	/// @brief Convert an ObjSize to a unsigned 8-bit integer
	/// @param size The ObjSize
	/// @return The size in pixels
	u8 ObjSizeToPixels(ObjSize size);

    /// @brief Sets a tile for a background (This isn't in the *Background classes just in case you need to manually set it, but it is recommended to use the *Background classes instead of manipulating the background manually)
    /// @param mapPtr The pointer to the map
    /// @param tilePosition The position where you want the tile to be placed
    /// @param newValue The new value for the tile
    /// @param mapSize The size of the map (in tiles)
    /// @param bitsPerTile The bits per tile of the map (8 or 16)
    void SetBGIndex(u16* const mapPtr, Vector2<u16> tilePosition, u16 newValue, Vector2<u16> mapSize, u8 bitsPerTile = 16);

	/// @brief Gets a tile from the background (This isn't in the *Background classes just in case you need to manually set it, but it is recommended to use the *Background classes instead of manipulating the background manually)
	/// @param mapPtr The pointer to the map
	/// @param tilePosition The position you want to get the tile for
	/// @param mapSize The size of the map (in tiles)
	/// @param bitsPerTile The bits per tile of the map (8 or 16)
	/// @return The tile at the given position
	u16 GetTileIndex(u16* const mapPtr, Vector2<u16> tilePosition, Vector2<u16> mapSize, u8 bitsPerTile = 16);

	/// @brief Simple AABB function
	/// @param firstPosition The position of the first object
	/// @param firstSize The size of the first object
	/// @param secondPosition The position of the second object
	/// @param secondSize The size of the second object
	/// @return If the two objects are colliding
	bool CheckAABB(Vector2<int> firstPosition, Vector2<u8> firstSize, Vector2<int> secondPosition, Vector2<u8> secondSize);

	/// @brief The DS doesn't allow 8 bit writes to VRAM, so this function allows you to do it easily (please don't use this too much, because it's not very fast)
	/// @param newValue The new value to use
	/// @param pointer The pointer to the place in VRAM to write to
	void VRAMWrite8Bit(u8 newValue, u16* pointer);
} // namespace Utils
