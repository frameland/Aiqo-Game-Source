//
//  scene.cpp
//  Qpt
//
//  Created by Markus on 24.04.13.
//
//

#include "scene.h"
using namespace ci;
using namespace std;

Scene::Scene()
{}

Scene::~Scene()
{
}

void Scene::onInit(){}
void Scene::onExit(){}
void Scene::onUpdate(){}
void Scene::onRender(){}


TCamera& Scene::getCamera()
{
    return camera;
}


void Scene::updateCamera()
{
    camera.update();
}


Director& Director::getInstance()
{
    static Director instance;
    return instance;
}

void Director::changeScene(Scene *toScene, bool withoutFade)
{
    if (currentScene) {
        fadingOut = true;
        fadingIn = false;
        nextScene = toScene;
    }
    else {
        currentScene = toScene;
        currentScene->onInit();
        fadingIn = true;
        fadingOut = false;
    }
    fadeTime = 0.0;
    
    if (withoutFade) {
        currentScene->onExit();
        currentScene = toScene;
        currentScene->onInit();
        nextScene = 0;
        fadeTime = setFadeTime;
        fadingIn = false;
        fadingOut = false;
        delete currentScene;
    }
}

Scene* Director::getScene() const
{
    return currentScene;
}

void Director::onUpdate()
{
    if (currentScene) {
        currentScene->onUpdate();
    }
}

void Director::onRender()
{
    if (!currentScene) return;
    
    gl::clear(Color(0, 0, 0));
    gl::enableAlphaBlending();
    currentScene->onRender();
    
    if (fadingOut) {
        if (onFadeOut()) {
            fadingOut = false;
            currentScene->onExit();
            delete currentScene;
            currentScene = nextScene;
            currentScene->onInit();
            fadingIn = true;
            fadeTime = 0.0;
        }
    }
    
    if (fadingIn) {
        if (onFadeIn()) {
            fadingIn = false;
            fadeTime = 0.0;
        }
    }
}

bool Director::onFadeIn()
{
    fadeTime += 1.0/60.0;
    if (fadeTime <= setFadeTime) {
        float alpha = fadeTime/setFadeTime;
        gl::color(0, 0, 0, 1 - alpha);
        gl::drawSolidRect(Rectf(0, 0, app::getWindowWidth(), app::getWindowHeight()));
        return false;
    }
    return true;
}

bool Director::onFadeOut()
{
    fadeTime += 1.0/60.0;
    if (fadeTime <= setFadeTime) {
        float alpha = fadeTime/setFadeTime;
        gl::color(0, 0, 0, alpha);
        gl::drawSolidRect(Rectf(0, 0, app::getWindowWidth(), app::getWindowHeight()));
        return false;
    }
    return true;
}


void Director::setTheFadeTime(double value)
{
    setFadeTime = value;
}












