#include <random>
#include <ctime>

#include <nds.h>
#include <filesystem.h>

#include <SDSF/Exception.hpp>

#include <SDSF/Background.hpp>
#include <SDSF/ResourceCreator.hpp>
#include <SDSF/Sprite.hpp>
#include <SDSF/SoundStream.hpp>

#include <maxmod9.h>

int run() {
    // Power on all the 2D systems
    powerOn(POWER_ALL_2D);

    // Set the video mode to mode 5 for both displays
    videoSetMode(MODE_5_2D | DISPLAY_SPR_ACTIVE | DISPLAY_BG3_ACTIVE);
    videoSetModeSub(MODE_5_2D | DISPLAY_SPR_ACTIVE | DISPLAY_BG3_ACTIVE);

    // Set the main vram banks
    vramSetPrimaryBanks(VRAM_A_MAIN_BG, VRAM_B_MAIN_SPRITE, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);

    // Initialize the OAM
    Sprite::InitOAM(Engine::Main, SpriteMapping_1D_32, false);
    Sprite::InitOAM(Engine::Sub, SpriteMapping_1D_32, false);

    nitroFSInit(NULL);

    // Load the smile texture
    Texture smile(Engine::Main);
    smile.loadTexture("nitro:/smile.img.bin", SpriteSize_32x32, SpriteColorFormat_16Color);

    // Load the palette for the smile texture
    Palette smilePalette(SPRITE_PALETTE, Palette::PALETTE_SIZE_16);
    smilePalette.Load("nitro:/smile.pal.bin", 0);

    struct Smile {
        Sprite spr;
        Vector2<int16> direction;
    };

    std::vector<Smile> smiles;

    // Get some randomness for the directions of the smiles
    std::random_device random;
    std::mt19937 rng = std::mt19937(random());
    std::uniform_int_distribution<std::mt19937::result_type> randomDirection = std::uniform_int_distribution<std::mt19937::result_type>(-3, 3);

    // Initilize all the smiles
    for(int i = 0; i < OAM_SIZE; i++) {
        smiles.push_back({Sprite(Engine::Main), {randomDirection(rng), randomDirection(rng)}});

        Sprite& spr = smiles.back().spr;
        spr.Create(SpriteSize_32x32, SpriteColorFormat_16Color);
        spr.SetTexture(smile);
        spr.SetPalette(smilePalette);
        spr.SetPosition({96, 64});
    }

    // Create the background
    ExRotbackground background(Engine::Main, 3);
    background.Create(BgSize_ER_256x256, 0, 1);
    // Load the graphics
    background.LoadGfx("nitro:/simpletilemap.img.bin", -1);
    for(int x = 0, currentTile = 0; x < 32; x++) {
        for(int y = 0; y < 32; y++) {
            // Set each tile for the background, using TileMapEntry16
            TileMapEntry16 entry;
            entry.index = currentTile++;
            entry.hflip = false;
            entry.vflip = false;
            entry.palette = 0;
            background.SetTile(x, y, entry);
            if(currentTile >= 3) {
                currentTile = 0;
            }
        }
    }
    background.SetWrapEnabled(true);

    // Load the palette for the background
    Palette mainBackgroundPalette(BG_PALETTE, Palette::PALETTE_SIZE_256);
    mainBackgroundPalette.Load("nitro:/simpletilemap.pal.bin", 0);

    // Initialize maxmod
    mm_ds_system system;
    system.fifo_channel = FIFO_MAXMOD;
    system.mem_bank = 0;
    system.mod_count = 0;
    system.samp_count = 0;
    mmInit(&system);

    // Create the soundstream
    SoundStream stream;
    stream.SetFormat(MM_STREAM_16BIT_STEREO);
    stream.SetSamplingRate(48000);
    stream.SetBufferLength(10000);
    stream.SetTimer(MM_TIMER3);
    stream.SetLoop(true);
    stream.Play("nitro:/best.raw");

    // Initialize timers to count fps
    TIMER0_CR = TIMER_ENABLE|TIMER_DIV_1024;
    TIMER1_CR = TIMER_ENABLE|TIMER_CASCADE;

    float milliseconds = 0, lastMilliseconds = 0;

    consoleDemoInit();

    while(true) {
        scanKeys();

        // Scroll the background (it will wrap since wrap is enabled)
        background.Scroll({1, 1});

        // Move all the smiles (and bounce them off the walls)
        for(Smile& smile : smiles) {
            smile.spr.Move({smile.direction.x, smile.direction.y});

            Vector2<u16> position = smile.spr.GetPosition();

            if(position.x < 0 || position.x > 256 - 32) {
                smile.direction.x = -smile.direction.x;
            }if(position.y < 0 || position.y > 192 - 32) {
                smile.direction.y = -smile.direction.y;
            }
        }

        // Please avoid using floating point math in your programs (use fpm instead), we just use floating points here so that we can display the fps
        lastMilliseconds = milliseconds;
        milliseconds = ((TIMER1_DATA*(1<<16))+TIMER0_DATA) / 32.7285;
        Utils::Print("FPS: " + std::to_string(1000 / (milliseconds - lastMilliseconds)) + "\n");

        // Update the background
        bgUpdate();

        swiWaitForVBlank();

        // Update the OAM (must be called after swiWaitForVBlank)
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
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