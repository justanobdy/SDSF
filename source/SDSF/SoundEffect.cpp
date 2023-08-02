#include <SDSF/SoundEffect.hpp>

std::vector<int> SoundEffect::loadedSounds = {};
u32 SoundEffect::lastHandle = 0;

SoundEffect::SoundEffect(int soundID)
{
    effect.id = soundID;
    effect.handle = mm_sfxhand(-1);
    effect.panning = 127;
    effect.volume = 255;
    effect.rate = 1024;

    LoadSound(soundID);
}

void SoundEffect::Play() {
    if(effect.handle == mm_sfxhand(-1)) {
        effect.handle = mmEffect(effect.id);
    }else {
        effect.handle = mmEffectEx(&effect);
    }
    
    playing = true;
}

void SoundEffect::Stop() {
    mmEffectCancel(effect.handle);
    playing = false;
    effect.handle = mm_sfxhand(-1);
}

void SoundEffect::SetVolume(u8 volume) {
    effect.volume = volume;
    updateValues();
}

u8 SoundEffect::GetVolume() const {
    return effect.volume;
}

void SoundEffect::SetPanning(u8 panning) {
    effect.panning = panning;
    updateValues();
}

u8 SoundEffect::GetPanning() const {
    return effect.panning;
}

void SoundEffect::LoadSound(int soundID)
{
    if(!IsSoundLoaded(soundID)) {
        mmLoadEffect(soundID);

        loadedSounds.push_back(soundID);
    }
}

bool SoundEffect::IsSoundLoaded(int soundID) {
    for(int i : loadedSounds) {
        if(i == soundID) {
            return true;
        }
    }

    return false;
}

void SoundEffect::UnloadSound(int soundID) {
    if(IsSoundLoaded(soundID)) {
        mmUnloadEffect(soundID);

        for(int i = 0; i < loadedSounds.size(); i++) {
            if(loadedSounds[i] == soundID) {
                loadedSounds.erase(loadedSounds.begin() + i);
                break;
            }
        }
    }
}

void SoundEffect::updateValues() const {
    if(!playing) return;

    mmEffectPanning(effect.handle, effect.panning);
    mmEffectRate(effect.handle, effect.rate);
    mmEffectVolume(effect.handle, effect.volume);
}