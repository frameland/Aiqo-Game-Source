//
//  intro.h
//  Qpt
//
//  Created by Markus on 27.05.13.
//
//

#ifndef Qpt_intro_h
#define Qpt_intro_h

#include "scene.h"
#include "behaviours.h"
#include <vector>
#include "sprite.h"
#include "gamepad.h"
#include "level.h"
#include "audio.h"

using namespace std;

class IntroScene : public Scene {
private:
    vector<Behaviour*> Behaviours;
    Sprite* earth;
    Sprite* moon;
    Sprite* bg;
    Sprite* title;
    Sprite* pressSpace;
    
public:
    IntroScene();
    ~IntroScene();
    void onUpdate();
    void onRender();
    void onInit();
    void onExit();

private:
    void startAnimation();
    void startFirstLevel();
    void updateLevelTime();
    
    double time;
    double deltaTime;
    
    bool pressedSpace;
    
    Waiter* waitPhase1;
    bool phase2Started;
};

#endif
