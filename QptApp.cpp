#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "particles.h"
#include "level.h"
#include "intro.h"
#include "gamepad.h"

using namespace ci;
using namespace ci::app;
using namespace std;


// Setup
class QptApp : public AppNative {
  public:
    void setup();
    void mouseDown( MouseEvent event );
    void mouseDrag(MouseEvent event);
    void keyDown( KeyEvent event );
    void keyUp(KeyEvent event);
    void update();
    void updateKeySpace();
    void draw();
    void drawInfo();
    void prepareSettings(Settings* settings);
    
private:
    bool mouseIsDown;
    bool shouldReload;
};

void QptApp::setup()
{
    initSoundSystem();
    
    Director& director = Director::getInstance();
    director.setTheFadeTime(5.0);
    
    bool startIntro = false;
    if (startIntro) {
        IntroScene* intro = new IntroScene;
        director.changeScene(intro);
    }
    else {
        Level* level = new Level;
        level->loadLevelFromFile("lv3.xml");
        level->getCamera().setFocus(&level->getHero().position);
        Director::getInstance().changeScene(level);
    }
    
    mouseIsDown = false;
    shouldReload = false;
}

void QptApp::prepareSettings(Settings* settings)
{
    settings->setWindowSize(800, 533);
    settings->setFrameRate(60.0);
    gl::enableVerticalSync();
}



// Key Handling
void QptApp::keyDown(KeyEvent event)
{   
    if (event.getCode() == KeyEvent::KEY_ESCAPE) {
        quit();
    }
    
    if (event.getCode() == KeyEvent::KEY_RIGHT) {
        Gamepad::keyRightIsDown = true;
    }
    else if (event.getCode() == KeyEvent::KEY_LEFT) {
        Gamepad::keyLeftIsDown = true;
    }
    else if (event.getCode() == KeyEvent::KEY_UP) {
        Gamepad::keyUpIsDown = true;
    }
    else if (event.getCode() == KeyEvent::KEY_DOWN) {
        Gamepad::keyDownIsDown = true;
    }
    
    if (event.getCode() == KeyEvent::KEY_SPACE && Gamepad::spaceDownTime == 0) {
        Gamepad::keySpaceIsHit = true;
        Gamepad::spaceDownTime = getElapsedFrames();
    }
    
    // Level Editor
    if (event.getChar() == 'w') {
        Gamepad::keyWIsDown = true;
    }
    else if (event.getChar() == 's') {
        Gamepad::keySIsDown = true;
    }
    else if (event.getChar() == 'a') {
        Gamepad::keyAIsDown = true;
    }
    else if (event.getChar() == 'd') {
        Gamepad::keyDIsDown = true;
    }
    
    Level* currentLevel = Level::getCurrent();
    if (currentLevel) {
        if (event.getChar() >= '0' && event.getChar() <= '9') {
            int mode = event.getChar() - '0';
            currentLevel->switchLevelEditorMode(mode);
        }
    }
}

void QptApp::keyUp(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_RIGHT) {
        Gamepad::keyRightIsDown = false;
    }
    else if (event.getCode() == KeyEvent::KEY_LEFT) {
        Gamepad::keyLeftIsDown = false;
    }
    else if (event.getCode() == KeyEvent::KEY_UP) {
        Gamepad::keyUpIsDown = false;
    }
    else if (event.getCode() == KeyEvent::KEY_DOWN) {
        Gamepad::keyDownIsDown = false;
    }
    
    if (event.getCode() == KeyEvent::KEY_SPACE) {
        Gamepad::keySpaceIsHit = false;
        Gamepad::spaceDownTime = 0;
    }
    
    // Level Editor
    if (event.getCode() == KeyEvent::KEY_RETURN) {
        Level* currentLevel = Level::getCurrent();
        if (currentLevel) {
            currentLevel->saveLevel();
        }
    }
    
    if (event.getChar() == 'w') {
        Gamepad::keyWIsDown = false;
    }
    else if (event.getChar() == 's') {
        Gamepad::keySIsDown = false;
    }
    else if (event.getChar() == 'a') {
        Gamepad::keyAIsDown = false;
    }
    else if (event.getChar() == 'd') {
        Gamepad::keyDIsDown = false;
    }
    
    
    // Reload Level
    if (event.getChar() == 'r') {
        Level* level = new Level();
        level->loadLevelFromFile(Level::getCurrent()->getId());
        level->getCamera().setFocus(&level->getHero().position);
        Director::getInstance().changeScene(level, true);
        
    }
}



// Update Director
void QptApp::update()
{
    updateKeySpace();
    Director::getInstance().onUpdate();
}

void QptApp::updateKeySpace()
{
    if (Gamepad::keySpaceIsHit) {
        if (Gamepad::spaceDownTime != getElapsedFrames()) {
            Gamepad::keySpaceIsHit = false;
        }
    }
}
void QptApp::draw()
{
    Director::getInstance().onRender();
    drawInfo();
}

void QptApp::drawInfo()
{
    Level* level = Level::getCurrent();
    if (!level) return;
    
    gl::color(1.0, 1.0, 1.0, 1.0);
}




// --------------------------------
// Level Editor Stuff
// --------------------------------
void QptApp::mouseDown( MouseEvent event )
{
    Level* currentLevel = Level::getCurrent();
    if (currentLevel) {
        currentLevel->handleMouseDown(event);
    }
}

void QptApp::mouseDrag(MouseEvent event)
{
    Level* currentLevel = Level::getCurrent();
    if (currentLevel) {
        currentLevel->handleMouseDrag(event);
    }
}

CINDER_APP_NATIVE( QptApp, RendererGl )









