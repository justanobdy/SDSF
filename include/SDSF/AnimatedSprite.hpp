#pragma once

#include <SDSF/fpm/fixed.hpp>

#include <SDSF/Sprite.hpp>

class AnimatedSprite : public Sprite {
public:
    /// @brief Enum for types of AnimatedSprites
    ///        Ram = store frames in ram, and load each frame into vram when needed
    ///        Vram = store frames in vram, and simply switch the texture when needed
    enum AnimatedSpriteType {
        Ram,
        Vram
    };

    /// @brief Constructor
    /// @param engine The engine to use
    /// @param type The type of sprite to use
    AnimatedSprite(Engine engine, AnimatedSpriteType type);

    /// @brief Set the speed of the AnimatedSprite (how many frames will pass before the next Sprite frame is set)
    /// @param framesPerFrame The speed of the sprite
    void SetSpeed(u16 framesPerFrame);

    /// @brief Load an image to use for the animation
    /// @param filename The filename of the image
    /// @param frames The amount of frames to load
    /// @param spriteSize The size of the sprite
    /// @param format The color format
    void LoadImage(const std::string& filename, u16 frames, SpriteSize spriteSize, SpriteColorFormat format);
    /// @brief Load an image to use for the animation (from a vector)
    /// @param vector The vector to load from
    /// @param frames The amount of frames to load
    /// @param spriteSize The size of the sprite
    /// @param format The color format
    void LoadImage(const std::vector<u16>& vector, u16 frames, SpriteSize spriteSize, SpriteColorFormat format);
    /// @brief Load an image to use for the animatino (from a raw C array)
    /// @param data The data to use
    /// @param length The length of the data (in half-words)
    /// @param frames The amount of frames to load
    /// @param spriteSize The size of the sprite
    /// @param format The color format
    void LoadImage(const u16* const data, u32 length, u16 frames, SpriteSize spriteSize, SpriteColorFormat format);

    /// @brief Set the total amount of frames
    /// @param frameCount The total amount of frames
    void SetFrames(u16 frameCount);

    /// @brief Update the sprite (call this every frame)
    void Update();

    /// @brief Stop the sprite animation
    void Stop();
    /// @brief Play the sprite animation
    void Play();

    /// @brief Set the current frame
    /// @param frame The current frame
    void SetFrame(u16 frame);
private:
    using Sprite::SetTexture;

    const AnimatedSpriteType type;

    /// @brief Only used for Ram type, stores sprite data in memory
    std::vector<std::vector<u16>> spriteData;

    /// @brief When in Vram mode, this stores all the textures needed. When in Ram mode, this only has 1 texture.
    std::vector<Texture> textures;

    SpriteSize size;
    SpriteColorFormat colorFormat;

    u16 frameCount;
    u16 fpf;
    u16 frames;
    u16 currentTexture = 0;

    bool playing = true;
};