#pragma once

#include <maxmod9.h>
#include <nds/ndstypes.h>
#include <vector>

/// @brief Wrapper class for maxmod sounds (using the soundbank)
///        TODO: Implement rate changing in a way that is easy for the user to understand
class SoundEffect {
public:
    /// @brief Constructor
    /// @param soundID The sound ID to use (usually defined in soundbank.h)
    SoundEffect(int soundID);

    /// @brief Check if a sound is loaded
    /// @param soundID The soundID to check for
    /// @return If the sound is loaded
    static bool IsSoundLoaded(int soundID);
    /// @brief Load a sound
    /// @param soundID The soundID to load
    static void LoadSound(int soundID);
    /// @brief Unload a sound
    /// @param soundID The soundID to unload
    static void UnloadSound(int soundID);

    /// @brief Play the sound
    void Play();
    /// @brief Stop the sound
    void Stop();

    /// @brief Set the volume of the sound
    /// @param volume The new volume (0-255)
    void SetVolume(u8 volume);
    /// @brief Get the volume
    /// @return The current volume
    u8 GetVolume() const;

    /// @brief Set the panning
    /// @param panning The new panning (127 = center)
    void SetPanning(u8 panning);
    /// @brief Get the panning
    /// @return The panning
    u8 GetPanning() const;
private:
    void updateValues() const;

    mm_sound_effect effect;
    bool playing = false;

    static std::vector<int> loadedSounds;
    static u32 lastHandle;
};