#pragma once

#include <string>
#include <vector>
#include <array>

#include <nds.h>

#include "Common.hpp"

#include "Texture.hpp"

#include "Vector2.hpp"

#include "fpm/fpm.hpp"

/// TODO: Add shearing support, and inline some functions

constexpr u8 OAM_SIZE = 128;

/// @brief Wrapper for libnds sprites
class Sprite {
public:
	/// @brief Initialize the OAM (use this instead of oamInit from libnds if you are going to use this class)
	/// @param engine The engine to initialize
	/// @param mapping The sprite mapping to use (use SpriteMapping_1D_32 if you are unsure of what to use)
	/// @param extPalette Is the extended palette enabled?
	static void InitOAM(Engine engine, SpriteMapping mapping, bool extPalette = false);
    /// @brief Allocate a sprite index for the given engine
    /// @param engine The engine to use
    /// @return The allocated sprite index, or u8(-1) if it failed (eg: all 128 sprites have already been used up) make sure to call Sprite::FreeOAMIndex once you are done using the sprite!
    static u8 AllocateOAMIndex(Engine engine);
	/// @brief Free an OAM index
	/// @param engine The engine to use
	/// @param index The index that will be freed
	static void FreeOAMIndex(Engine engine, u8 index);

    /// @brief Default constructor
    /// @param engine The engine that this sprite will use
    Sprite(Engine engine);

    /// @brief Create a sprite (and allocate a sprite index for this sprite)
    /// @param size The size of the sprite
    /// @param shape The shape of the sprite
    /// @param colorFormat The color format this sprite will use
    void Create(ObjSize size, ObjShape shape, ObjColMode colorFormat);
	/// @brief Create a sprite (and allocate a sprite index for this sprite)
	/// @param size The size of the sprite
	/// @param colorFormat The color format of the sprite
	void Create(SpriteSize size, SpriteColorFormat colorFormat);

	/// @brief Set a graphics index for this sprite
	/// @param gfxIndex The graphics index to use
	void SetTexture(u16 gfxIndex);
	/// @brief Set a texture for this sprite
	/// @param texture The texture to use
	void SetTexture(const Texture& texture);

    /// @brief Set a palette for this sprite
    /// @param palIndex The palette index to use
    void SetPalette(u16 palIndex);

    /// @brief Reset this sprite (and free the allocated index, if there is one)
    void Reset();
	/// @brief Reset the sprite entry values
	void ResetEntry();

    /// @brief Set the position of the sprite
    /// @param position The new position
    void SetPosition(Vector2<u16> position);
    /// @brief Move the sprite
    /// @param amount The amount to move the sprite
    void Move(Vector2<u16> amount);

	/// @brief Get the position of the sprite
	/// @return The position
	Vector2<u16> GetPosition() const;

    /// @brief Set the rotation of the sprite
    /// @param rotation The new rotation
    void SetRotation(fixed8 rotation);
    /// @brief Rotate the sprite
    /// @param amount The amount to rotate it by
    void Rotate(fixed8 amount);

    /// @brief Set the scale of the sprite (make sure to use EnableAffineMatrix before using this)
    /// @param scale The new scale
    void SetScale(Vector2<fixed8> scale);
    /// @brief Set the inverse scale of the sprite 
    /// (This is here if you want to optimize a little, because with SetScale the inverse has to be calculated, but with this it can be set directly. To calculate the inverse, simply do 1/scale.x, 1/scale.y)
    /// (make sure to use EnableAffineMatrix before using this)
    /// @param scale The inverse scale to set
    void SetInverseScale(Vector2<fixed8> scale);

    /// @brief Enable the matrix
    /// @param matrixID The matrix ID that this sprite will use
    void EnableAffineMatrix(u8 matrixID);
    /// @brief Disable the matrix for this sprite
    void DisableAffineMatrix();

    /// @brief Set if this sprite is double size (useful for scaling)
    /// @param sizeDouble If this sprite is double size
    void SetSizeDouble(bool sizeDouble);

	/// @brief Set if the sprite is visible
	/// @param visible If the sprite is visible
	void SetVisible(bool visible);

    /// @brief Update the matrix use this after calling a Rotate/Scale function
    void UpdateMatrix();

	/// @brief Set if mosaic is enabled for this sprite
	/// @param mosaicEnabled If mosaic is enabled
	void SetMosaic(bool mosaicEnabled);

	/// @brief Set if this sprite is flipped vertically
	/// @param vFlip If the sprite is flipped vertically
	void SetVFlip(bool vFlip);
	/// @brief Set if this sprite is flipped horizontally
	/// @param hFlip If the sprite is flipped horizontally
	void SetHFlip(bool hFlip);

	/// @brief Get the graphics index this sprite uses
	/// @return The graphics index this sprite uses
	u16 getGfxIndex() const;

    /// @brief Get a SpriteEntry with default settings
    /// @return A SpriteEntry with default settings
    static SpriteEntry DefaultEntry();

    /// @brief Default destructor make sure this called if you used Create() (eg: don't delete this using a void pointer, if you don't do anything dumb you should be fine)
    ~Sprite();
protected:
    OamState& oam;
	const Engine engine;

    SpriteEntry* entry;
	u8 index = -1;
	Vector2<u8> size;

    bool created = false;

    u16 gfxIndex;

    fixed8 rotation = fixed8(0);
    Vector2<fixed8> inverseScale;

	static std::array<bool, OAM_SIZE> indexesMain;
	static std::array<bool, OAM_SIZE> indexesSub;
};