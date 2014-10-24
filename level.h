//
//  level.h
//  Qpt
//
//  Created by Markus on 24.04.13.
//
//

#ifndef Qpt_level_h
#define Qpt_level_h

#include "scene.h"
#include "hero.h"
#include "gamepad.h"
#include "intro.h"

#include "utility.h"
#include <vector>
#include <algorithm>
#include "cinder/Xml.h"
#include "behaviours.h"

#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include "audio.h"


using namespace std;
using namespace ci::app;

class Level : public Scene {
private:
    double time;
    double deltaTime;
    
    int width;
    int height;
    vector< vector<int> > Map;
    
    string id;
    
    gl::Texture* Tile;
    int numberOfTiles;
    gl::Texture bg;
    gl::Texture level;
    gl::Texture antenna;
    
    Vec2f antennaPosition;
    Vec2f heroStartPosition;
    
    Hero hero;
    float gravity;
    
    vector<Connector*> Connectors;
    vector<Behaviour*> Behaviours;
    
    // Antenna animation
    bool heroGotAntenna;
    bool goingToNextLevel;
    Waiter* waitAntennaAnimation;
    Rectf fullscreenRect;
    float rectAlpha;
    
    
    // Level Editor
    int currentMode;
    Vec2f freeCam;
    Attractor* selectedAttractor;
    Detractor* selectedDetractor;
    
public:
    static const int TILESIZE = 32;
    static const int EMPTY = -1;

//  Constructors
    Level();
    Level(int width, int height);
    ~Level();
    
//  Time
    void updateLevelTime();
    
//  Overriden Methods
    void onUpdate();
    void onRender();
    void onInit();
    void onExit();
    
//  Tiles
    void setTileAtPosition(int x, int y, int tileId);
    void setTileAtPosition(int index, int tileId);
    void setTileAtMousePos(int x, int y, int tileId);
    int getTileIdAtPosition(Vec2f position, bool isTilePosition = false);
    
//  Connectors
    void addConnector(Connector* connector);
    void removeConnector(Connector* connector);
    void updateConnectors();
    void renderConnectors();
    
//  Behaviours
    void updateBehaviours();
    void addBehaviour(Behaviour* behaviour);
    
//  Level loading/saving
    void loadLevelFromFile(string pathToLevel);
    void saveLevel();
    
//  Helpers
    static Level* getCurrent();
    void nextLevel();
    bool heroCanAct();
    
//  Level Editor Stuff
    void updateLevelEditor();
    void renderLevelEditor();
    void switchLevelEditorMode(int toMode);
    void updateFreeMovingCam();
    void handleMouseDown(MouseEvent event);
    void handleMouseDrag(MouseEvent event);
    
//  Connectors
    Attractor* getSelectedAttractor();
    Detractor* getSelectedDetractor();
    Connector* pickConnector(Vec2f atPosition, vector<Connector*>& connectors);

//  Antenna
    void updateAntenna();
    void startAntennaAnimation();
    
    void updateParameters();
    
//  other
    Hero& getHero();
    float getGravity();
    string getId() const;
    int toTilepos(float pixelPos);
    
private:
    void loadTiles();
    void renderTiles();
    void renderBg();
    void renderLevel();
    void renderAntenna();
};

#endif
