#include <SDSF/Palette.hpp>

#include <SDSF/FileReader.hpp>

#include <nds.h>

Palette::Palette(u16* const destination, PaletteType type)
    :   destination(destination), colorsPerPalette(static_cast<u16>(type))
{}

Palette::~Palette() {
    Unload();
}

void Palette::Unload() {
    if(index == u16(-1)) return;

    dmaFillHalfWords(PALETTE_DMA_CHANNEL, &destination[index * colorsPerPalette], bytesLoaded);

    index = -1;
    bytesLoaded = 0;
}

void Palette::Load(const std::string& filename, u16 paletteIndex, u16 colorsToLoad) {
    FileReader reader(filename, true);

    std::vector<u16> data = reader.ReadEntireFile<u16>();

    Load(&data[0], data.size(), paletteIndex, colorsToLoad);
}

void Palette::Load(const u16* const data, u32 length, u16 paletteIndex, u16 colorsToLoad) {
    Unload();

    if(colorsToLoad != u16(-1)) {
        bytesLoaded = colorsToLoad << 1;
    }else {
        bytesLoaded = length << 1;
    }

    if(bytesLoaded > colorsPerPalette << 1) {
        bytesLoaded = colorsPerPalette << 1;
    }

    if(bytesLoaded > length << 1) {
        bytesLoaded = 0;

        throw Exception("Data given to Palette::Load is too short!");
    }

    index = paletteIndex;

    nocashMessage(std::to_string(paletteIndex).c_str());

    dmaCopyHalfWords(PALETTE_DMA_CHANNEL, data, &destination[index * colorsPerPalette], bytesLoaded);
}

u16 Palette::GetIndex() const {
    return index;
}