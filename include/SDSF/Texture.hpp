#pragma once

#include <string>
#include <nds.h>

#include "Common.hpp"

constexpr u8 TEXTURE_DMA_CHANNEL = 3;

static constexpr u16 GetNumberOfPixels(SpriteSize size) {
	return static_cast<int>(size) << 5;
}

/// @brief Simple Texture class
class Texture {
public:
    /// @brief Default constructor
    /// @param engine 
    Texture(Engine engine);

    /// @brief Load a texture from a file (and allocate VRAM memory for the texture)
    /// @param filename The filename
    /// @param spriteSize The size of the texture to load
    /// @param colorMode The color mode of the texture
    /// @param offsetBytes The offset from the start of file in bytes
    void loadTexture(const std::string& filename, SpriteSize spriteSize, SpriteColorFormat colorMode, u16 offsetBytes = 0);
	/// @brief Load a texture from a byte array (and allocate VRAM memory for the texture)
	/// @param texture The texture data
	/// @param spriteSize The size of the texture
	/// @param colorMode The color mode of the texture
	void loadTextureFromMemory(const u16* const texture, SpriteSize spriteSize, SpriteColorFormat colorMode);

	/// @brief Unload the texture from memory (and deallocate VRAM memory)
	void unloadTexture();
	/// @brief Destructor (calls unloadTexture() if the texture has not been freed)
	~Texture();

    /// @brief Get a pointer to the texture
    /// @return The pointer to the texture
    u16* getPointer() const;
    /// @brief Get the vram slot the texture uses
    /// @return The vram slot the texture uses
    u16 getSlot() const;
	/// @brief Get the size (in bytes) of the texture
	/// @return The texture size (in bytes)
	u16 getSize() const;

    /// @brief Calculate the size of a texture (in bytes)
    /// @param spriteSize The size of the sprite
    /// @param colorMode The color mode of the sprite
    /// @return The size of the texture (in bytes)
    static u32 CalculateTextureSize(SpriteSize spriteSize, SpriteColorFormat colorMode);
	
	bool operator==(const Texture& other) {
		return other.pointer == pointer || other.gfxSlot == gfxSlot;
	}
private:
    const Engine engine;

    u16 gfxSlot;
    u16* pointer = nullptr;
	u16 size;

    bool loaded = false;
};