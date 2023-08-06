#include "SDSF/ResourceCreator.hpp"

#include <SDSF/FileReader.hpp>

#include <nds.h>

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