#include <SDSF/AnimatedSprite.hpp>

#include <SDSF/FileReader.hpp>

#include <iostream>

AnimatedSprite::AnimatedSprite(Engine engine, AnimatedSpriteType type)
    :   Sprite(engine), type(type)
{

}

void AnimatedSprite::SetSpeed(u16 framesPerFrame) {
    fpf = framesPerFrame;
}

void AnimatedSprite::LoadImage(const std::string& filename, u16 frames, SpriteSize spriteSize, SpriteColorFormat format) {
    FileReader reader(filename);

    std::vector<u16> data = reader.ReadEntireFile<u16>();

    reader.Close();

    LoadImage(data, frames, spriteSize, format);
}

void AnimatedSprite::LoadImage(const std::vector<u16>& vector, u16 frames, SpriteSize spriteSize, SpriteColorFormat format)
{
    LoadImage(&vector[0], vector.size(), frames, spriteSize, format);
}

void AnimatedSprite::LoadImage(const u16* const data, u32 length, u16 frames, SpriteSize spriteSize, SpriteColorFormat format) {
    size = spriteSize;
    colorFormat = format;
    frameCount = frames;

    const u32 sizePerSprite = Texture::CalculateTextureSize(spriteSize, format);

    // The expected size of the data, in half words
    const u32 expectedDataSize = (sizePerSprite * frames) >> 1;

    //std::cout << sizePerSprite >> 1 << std::endl;        

    if(length < expectedDataSize) {
        throw Exception("Data given to AnimatedSprite is too short!");
    }

    if(type == AnimatedSpriteType::Ram) {
        // Split the data into different vectors

        for(u32 i = 0, position = 0; i < frames; i++) {
            spriteData.push_back(std::vector<u16>());

            std::vector<u16>& currentData = spriteData.back();

            currentData.insert(currentData.end(), &data[position], &data[position + (sizePerSprite >> 1)]);

            position += sizePerSprite >> 1;
        }

        textures.push_back(Texture(engine));

        Texture& texture = textures.front();
        texture.loadTextureFromMemory(&spriteData[currentTexture][0], spriteSize, format);

        SetTexture(texture);
    }else {
        nocashMessage(std::to_string(frames).c_str());

        // putting this in its own loops seems to make it work for some reason
        for(u32 i = 0; i < frames; i++) {
            textures.push_back(Texture(engine));
        }

        for(u32 i = 0, position = 0; i < frames; i++) {
            Texture& texture = textures[i];

            texture.loadTextureFromMemory(&data[position], spriteSize, format);

            position += sizePerSprite >> 1;
        }

        SetTexture(textures.front());
    }
}

void AnimatedSprite::Update() {
    if(frames >= fpf) {
        frames = 0;

        currentTexture++;

        if(currentTexture >= frameCount) {
            currentTexture = 0;
        }

        if(type == AnimatedSpriteType::Ram) {
            Texture& texture = textures.back();
            texture.loadTextureFromMemory(&spriteData[currentTexture][0], size, colorFormat);

            SetTexture(texture);
        }if(type == AnimatedSpriteType::Vram) {
            SetTexture(textures[currentTexture]);
        }
    }
    
    frames++;
}

void AnimatedSprite::SetFrames(u16 frameCount)
{
    this->frameCount = frameCount;
}