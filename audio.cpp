//
//  audio.cpp
//  Qpt
//
//  Created by Markus on 28.05.13.
//
//

#include "audio.h"

void initSoundSystem()
{
    FMOD::System_Create(&fmodSystem);
    fmodSystem->init(8, FMOD_INIT_NORMAL, 0);
    mainTheme = loadSound("main_theme.mp3");
    soundDisconnect = loadSound("disconnect.wav");
    soundAntennaFlash = loadSound("antenna_flash.wav");
    introSound = loadSound("intro.wav");
}

FMOD::Sound* loadSound(const string& path)
{
    assert(fmodSystem);
    
    string realPath = getAssetFolder() + "audio/" + path;
    FMOD::Sound* sound = 0;
    fmodSystem->createSound(realPath.c_str(), FMOD_LOOP_OFF, 0, &sound);
    return sound;
}

void playSound(FMOD::Sound* sound)
{
    assert(fmodSystem);
    fmodSystem->playSound(FMOD_CHANNEL_FREE, sound, false, 0);
    fmodSystem->update();
}



// Global
FMOD::System* fmodSystem;
FMOD::Sound* mainTheme;
FMOD::Sound* soundDisconnect;
FMOD::Sound* soundAntennaFlash;
FMOD::Sound* introSound;
FMOD::Channel* introSoundChannel;
