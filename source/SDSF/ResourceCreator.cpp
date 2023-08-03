#include "SDSF/ResourceCreator.hpp"

#include <SDSF/FileReader.hpp>

void ResourceCreator::LoadTilemap(const std::string& filename, u32 tilesToLoad, u16* gfxPointer) {
    FileReader reader;

    reader.OpenFile(filename, true);

	int dataSize = tilesToLoad * 32;

	if(tilesToLoad == UINT32_MAX) {
		dataSize = reader.GetBytesLeft() / 2;
	}

    const std::vector<u16> data = reader.ReadNumbers<u16>(dataSize);

    DC_FlushAll();
    dmaCopyHalfWords(3, &data[0], gfxPointer, data.size() * sizeof(u16));
}

void ResourceCreator::LoadPalette(const std::string& filename, u32 colorsToLoad, u16* palPointer)
{
    FileReader reader(filename, true);

    const std::vector<u16> numbers = reader.ReadNumbers<u16>(colorsToLoad == SIZE_MAX ? reader.GetBytesLeft() / 2 : colorsToLoad);

    DC_FlushAll();
    dmaCopyHalfWords(3, &numbers[0], palPointer, numbers.size() * sizeof(u16));
}
