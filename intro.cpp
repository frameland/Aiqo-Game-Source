//
//  intro.cpp
//  Qpt
//
//  Created by Markus on 27.05.13.
//
//

#include "intro.h"
using namespace ci;
using namespace app;

IntroScene::IntroScene()
    : Scene()
    , Behaviours(0)
    , deltaTime(0)
    , time(getElapsedSeconds())
    , pressedSpace(false)
    , waitPhase1(0)
    , phase2Started(false)
{
    bg = new Sprite(Vec2f(0, 0), "intro/intro_bg.png");
    earth = new Sprite(Vec2f(15, 20), "intro/earth.png");
    moon = new Sprite(Vec2f(257, 57), "intro/moon.png");
    moon->setScale(0.14, 0.14);
    moon->setAlpha(0.15);
    title = new Sprite(Vec2f(418, 121), "intro/title.png");
    title->alpha = 0.7;
    pressSpace = new Sprite(Vec2f(447, 376), "intro/press_space.png");
}

IntroScene::~IntroScene()
{
    delete bg;
    delete earth;
    delete moon;
    delete title;
    delete pressSpace;
    delete waitPhase1;
    
    //delete introSoundChannel;
    //delete introSound;
    
    for (vector<Behaviour*>::iterator i = Behaviours.begin(); i != Behaviours.end(); i++) {
        Behaviours.erase(i);
    }
    Behaviours.clear();
}


void IntroScene::onInit()
{
}

void IntroScene::onExit()
{
    playSound(mainTheme);
}

void IntroScene::onUpdate()
{
    fmodSystem->update();
    
    updateLevelTime();
    
    if (!pressedSpace) {
        pressSpace->alpha = sin(getElapsedFrames()/25.0) + 1.1;
        pressSpace->alpha = MIN(pressSpace->alpha, 0.6);
        if (Gamepad::keySpaceIsHit) {
            startAnimation();
        }
    }
    else {
        if (waitPhase1 && !waitPhase1->active && !phase2Started) {
            startFirstLevel();
            phase2Started = true;
        }
    }
    
    vector<Behaviour*>::iterator i;
    for (i = Behaviours.begin(); i != Behaviours.end(); i++) {
        (*i)->update(deltaTime);
    }
}

void IntroScene::startAnimation()
{
    // start playing music
    playSound(introSound);
    
    // fade out press space
    pressedSpace = true;
    TweenFloat* spaceAlpha = new TweenFloat(&pressSpace->alpha, -1.0, 1.0, EASE_LINEAR);
    Behaviours.push_back(spaceAlpha);
    
    // move out Title
    TweenVec2f* move = new TweenVec2f(&title->position, Vec2f(500, 0), 3.0, EASE_IN_CIRC);
    Behaviours.push_back(move);
    TweenFloat* titleAlpha = new TweenFloat(&title->alpha, -0.7, 3.0, EASE_LINEAR);
    Behaviours.push_back(titleAlpha);
    
    // fly past earth ...
    TweenVec2f* scaleEarth = new TweenVec2f(&earth->scale, Vec2f(3.0, 3.0), 5.0, EASE_IN_CIRC);
    Behaviours.push_back(scaleEarth);
    TweenVec2f* flyToMoon = new TweenVec2f(&earth->position, Vec2f(0, 400), 5.0, EASE_IN_CIRC);
    Behaviours.push_back(flyToMoon);
    TweenVec2f* scaleBg = new TweenVec2f(&bg->scale, Vec2f(1.0, 1.0), 8.0, EASE_IN_CIRC);
    Behaviours.push_back(scaleBg);
    
    // ... to the moon
    TweenVec2f* scaleMoon = new TweenVec2f(&moon->scale, Vec2f(0.2, 0.2), 5.0, EASE_IN_CIRC);
    Behaviours.push_back(scaleMoon);
    TweenFloat* alphaMoon = new TweenFloat(&moon->alpha, 0.85, 5.0, EASE_IN_CIRC);
    Behaviours.push_back(alphaMoon);
    
    // wait for 5 secs
    waitPhase1 = new Waiter(4.0);
    Behaviours.push_back(waitPhase1);
}

void IntroScene::startFirstLevel()
{
    TweenVec2f* scaleMoon = new TweenVec2f(&moon->scale, Vec2f(3.0, 3.0), 5.0, EASE_IN_CIRC);
    Behaviours.push_back(scaleMoon);
    
    TweenVec2f* flyToMoon = new TweenVec2f(&moon->position, Vec2f(-500, 100), 5.0, EASE_IN_CIRC);
    Behaviours.push_back(flyToMoon);
    
    TweenVec2f* scaleBg = new TweenVec2f(&bg->scale, Vec2f(1.0, 1.0), 5.0, EASE_IN_CIRC);
    Behaviours.push_back(scaleBg);
    
    // switch to Level 1
    Level* level = new Level;
    level->loadLevelFromFile("lv1.xml");
    Director::getInstance().changeScene(level);
}


void IntroScene::onRender()
{
    bg->render();
    moon->render();
    earth->render();
    title->render();
    pressSpace->render();
}


void IntroScene::updateLevelTime()
{
    double lastTime = time;
    time = getElapsedSeconds();
    deltaTime = time - lastTime;
}

