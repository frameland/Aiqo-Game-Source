//
//  audio.h
//  Qpt
//
//  Created by Markus on 28.05.13.
//
//

#ifndef Qpt_audio_h
#define Qpt_audio_h

#include "utility.h"
#include "FMOD/include/fmod.hpp"
#include "FMOD/include/fmod.h"

using namespace ci;
using namespace std;

FMOD::Sound* loadSound(const string& path);
void playSound(FMOD::Sound* sound);
void initSoundSystem();

// Declare the Soundsystem exactly once
extern FMOD::System* fmodSystem;
extern FMOD::Sound* mainTheme;
extern FMOD::Sound* soundDisconnect;
extern FMOD::Sound* soundAntennaFlash;
extern FMOD::Sound* introSound;
extern FMOD::Channel* introSoundChannel;

#endif
