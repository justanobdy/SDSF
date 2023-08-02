#pragma once

#include <vector>
#include <string>

#include <nds/ndstypes.h>

#include "Common.hpp"

using TileResource = std::vector<int>;

/// @brief TODO: redo this, we don't need to have different functions for loading everything, just make a generic load function using the DMA
namespace ResourceCreator
{
    void LoadTilemap(const std::string& filename, u32 tilesToLoad, u16* gfxPointer);
    void LoadPalette(const std::string& filename, u32 colorsToLoad, u16* palPointer);
}