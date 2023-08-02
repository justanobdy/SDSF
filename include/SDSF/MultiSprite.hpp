#pragma once

#include <SDSF/Sprite.hpp>

/// @brief Simple class for displaying a large texture using multiple sprites (TODO: add rotation functionality)
class MultiSprite {
public:
	/// @brief Default constructor
	/// @param engine The engine this MultiSprite will use
	MultiSprite(Engine engine);

	/// @brief Create the MultiSprite
	/// @param filename The filename
	/// @param sizePerSprite The size per sprite
	/// @param colorFormat The color format to use
	/// @param spriteNumber The number of sprites in each dimension
	void Create(const std::string& filename, SpriteSize sizePerSprite, SpriteColorFormat colorFormat, Vector2<u8> spriteNumber);

	/// @brief Set the position of the MultiSprite
	/// @param newPosition The new position
	void SetPosition(Vector2<u16> newPosition);
	/// @brief Move the MultiSprite
	/// @param amount The amount to move
	void Move(Vector2<u16> amount);

	/// @brief Get the position of the MultiSprite
	/// @return The position
	Vector2<u16> GetPosition() const;

	/// @brief Set the palette the MultiSprite will use
	/// @param palIndex The new palette
	void SetPalette(u16 palIndex);

	/// @brief Set if the MultiSprite is visible
	/// @param visible If the MultiSprite is visible
	void SetVisible(bool visible);

	//void SetVFlip(bool vFlip);
	//void SetHFlip(bool hFlip);
private:
	const Engine engine;
	Vector2<u8> spriteNumber;
	Vector2<u8> size;
	Vector2<u16> position;
	
	std::vector<Sprite> sprites;
	std::vector<Texture> textures;
};