#pragma once

#include <string>
#include <nds/ndstypes.h>

#include <SDSF/Common.hpp>

/// @brief Class to load and unload palettes
class Palette {
public:
    /// @brief The palette type (16 color or 256 color)
    enum PaletteType {
        PALETTE_SIZE_16 = 16,
        PALETTE_SIZE_256 = 256
    };

    /// @brief Contructor
    /// @param destination The destination of the palette (eg: SPRITE_PALETTE, BG_PALETTE, etc.)
    /// @param type The type of palette
    Palette(u16* const destination, PaletteType type = PALETTE_SIZE_16);
    /// @brief Destructor
    ~Palette();

    /// @brief Load a palette from a file
    /// @param filename The file to load from
    /// @param paletteIndex The index to load the palette into
    /// @param colorsToLoad The amount of colors to load (leave as -1 to load as many as possible from the file)
    void Load(const std::string& filename, u16 paletteIndex, u16 colorsToLoad = -1);
    /// @brief Load a palette from an array
    /// @param data The data to load
    /// @param length The length of the data (the amount of items in the array)
    /// @param paletteIndex The index to load the palette into
    /// @param colorsToLoad The amount of colors to load (leave as -1 to load as many as possible from the file)
    void Load(const u16* const data, u32 length, u16 paletteIndex, u16 colorsToLoad = -1);

    /// @brief Unload the palette (does nothing if the palette is not loaded)
    void Unload();

    /// @brief Get the index of the palette
    /// @return The index of the palette
    u16 GetIndex() const;
private:
    u16 index = -1;
    u16 bytesLoaded;

    u16* const destination;

    const u16 colorsPerPalette;

    static constexpr u8 PALETTE_DMA_CHANNEL = 3;
};