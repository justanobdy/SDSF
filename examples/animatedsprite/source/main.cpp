#include <SDSF/AnimatedSprite.hpp>
#include <SDSF/Utils.hpp>

#include <filesystem.h>

int main() {
    // Turn on the hardware
    powerOn(POWER_ALL_2D);

    // Set the mode
    videoSetMode(MODE_5_2D);
    
    consoleDemoInit();

    vramSetBankA(VRAM_A_MAIN_SPRITE);

    nitroFSInit(NULL);

    // Initialize OAM
    Sprite::InitOAM(Engine::Main, SpriteMapping_1D_32);

    // Load the palette into palette 0 into the main sprite palette
    Palette palette(SPRITE_PALETTE, Palette::PALETTE_SIZE_16);
    palette.Load("nitro:/animated.pal.bin", 0);

    // Initialize the sprite and load the animation
    AnimatedSprite sprite(Engine::Main, AnimatedSprite::AnimatedSpriteType::Vram);
    sprite.Create(SpriteSize_16x16, SpriteColorFormat_16Color);
    sprite.LoadImage("nitro:/animated.img.bin", 8, SpriteSize_16x16, SpriteColorFormat_16Color);
    sprite.SetPalette(palette);
    sprite.SetSpeed(5);

    Utils::Print("Press A to play\nPress B to pause");

    while(true) {
        scanKeys();

        if(keysDown() & KEY_A) {
            sprite.Play();
        }if(keysDown() & KEY_B) {
            sprite.Stop();
        }

        swiWaitForVBlank();

        // Update the animation
        sprite.Update();

        oamUpdate(&oamMain);
    }

    return 0;
}