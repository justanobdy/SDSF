#include <SDSF/Sprite.hpp>
#include <SDSF/Texture.hpp>
#include <SDSF/ResourceCreator.hpp>
#include <SDSF/Exception.hpp>
#include <SDSF/Utils.hpp>

#include <filesystem.h>

int run() {
    // Make sure all the 2d hardware is on
    powerOn(POWER_ALL_2D);

    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_5_2D);

    // Set VRAM bank A to main engine sprite
    vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_MAIN_SPRITE, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);

    nitroFSInit(NULL);

    // Initalize the OAM, make sure you use Sprite::InitOAM instead of oamInit
    Sprite::InitOAM(Engine::Main, SpriteMapping_1D_32, false);
    Sprite::InitOAM(Engine::Sub, SpriteMapping_1D_32, false);

    // Load the texture into VRAM
    Texture texture(Engine::Main);
    texture.loadTexture("nitro:/smile.img.bin", SpriteSize_32x32, SpriteColorFormat_16Color);

    // Now load it into Sub engine VRAM
    Texture subTexture(Engine::Sub);
    subTexture.loadTexture("nitro:/smile.img.bin", SpriteSize_32x32, SpriteColorFormat_16Color);

    // Create the sprite
    Sprite sprite(Engine::Main);
    sprite.Create(SpriteSize_32x32, SpriteColorFormat_16Color);
    sprite.SetTexture(texture);
    sprite.SetPalette(0);

    // Create the sub sprite
    Sprite subSprite(Engine::Sub);
    subSprite.Create(SpriteSize_32x32, SpriteColorFormat_16Color);
    subSprite.SetTexture(subTexture);
    subSprite.SetPalette(0);

    // Load the sprite to the first index of the sprite palette
    ResourceCreator::LoadPalette("nitro:/smile.pal.bin", -1, &SPRITE_PALETTE[0]);
    ResourceCreator::LoadPalette("nitro:/smile.pal.bin", -1, &SPRITE_PALETTE_SUB[0]);

    while(true) {
        // Scan the keys
        scanKeys();

        // Move the sprite when a key is pressed
        if(keysHeld() & KEY_RIGHT) {
            sprite.Move({1, 0});
        }if(keysHeld() & KEY_LEFT) {
            sprite.Move({-1, 0});
        }if(keysHeld() & KEY_UP) {
            sprite.Move({0, -1});
        }if(keysHeld() & KEY_DOWN) {
            sprite.Move({0, 1});
        }

        // Make the sub sprite be at the same position as the main sprite
        subSprite.SetPosition(sprite.GetPosition());

        swiWaitForVBlank();

        // Update the OAM (must be called after swiWaitForVBlank)
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }

    return 0;
}

int main() {
    // Try to run the program, but catch any exceptions that may pop up
    try {
        return run();
    }catch(const Exception& e) {
        // Print the exception if something went wrong
        consoleDemoInit();

        Utils::Print(e.what());

        while(1);
    }
}