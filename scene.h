//
//  scene.h
//  Qpt
//
//  Created by Markus on 24.04.13.
//
//

#ifndef Qpt_scene_h
#define Qpt_scene_h

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "camera.h"

using namespace ci;
class QptApp;

// Scene is the base game state from which every other state inherits
class Scene {
protected:
    TCamera camera;
    
public:
    Scene();
    virtual ~Scene();
    virtual void onUpdate();
    virtual void onRender();
    virtual void onInit();
    virtual void onExit();
        
    void updateCamera();
    TCamera& getCamera();
};


// The Director manages scenes. Use getInstance() instead of creating an instance
class Director {
private:
    Scene* currentScene;
    Scene* nextScene;
    bool fadingIn;
    bool fadingOut;
    double fadeTime;
    double setFadeTime;
    
public:
    static Director& getInstance();
    void changeScene(Scene* toScene, bool withoutFade = false);
    void onUpdate();
    void onRender();
    
    Scene* getScene() const;
    
    void setTheFadeTime(double value);
    
private:
    bool onFadeIn();
    bool onFadeOut();
};

#endif
