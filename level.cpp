//
//  level.cpp
//  Qpt
//
//  Created by Markus on 25.04.13.
//
//

#include "level.h"
#include "cinder/app/App.h"

using namespace std;
using namespace ci;
using namespace ci::app;

Level::Level()
        : Scene()
        , width(0)
        , height(0)
        , numberOfTiles(1)
        , gravity(30)
        , Connectors(0)
        , time(0)
        , deltaTime(0)
        , currentMode(1)
        , selectedAttractor(0)
        , selectedDetractor(0)
        , antennaPosition(0, 0)
        , heroStartPosition(0, 0)
        , heroGotAntenna(false)
        , goingToNextLevel(false)
        , waitAntennaAnimation(0)
        , fullscreenRect(0.0, 0.0, 800, 533)
        , rectAlpha(0.0)
{ }

Level::Level(int width, int height)
        : Scene()
        , width(width)
        , height(height)
        , numberOfTiles(1)
        , Map(0)
        , Connectors(0)
        , Tile(0)
        , gravity(4)
        , time(0)
        , deltaTime(0)
        , currentMode(1)
        , selectedAttractor(0)
        , selectedDetractor(0)
        , antennaPosition(0, 0)
        , heroStartPosition(0, 0)
        , heroGotAntenna(false)
        , goingToNextLevel(false)
        , waitAntennaAnimation(0)
        , fullscreenRect(0.0, 0.0, 800, 533)
        , rectAlpha(0.0)
{
    Map.resize(width, vector<int>(height, -1));
}

Level::~Level()
{
    delete waitAntennaAnimation;
    delete selectedAttractor;
    delete selectedDetractor;
    
    for (vector<Behaviour*>::iterator i = Behaviours.begin(); i != Behaviours.end(); i++) {
//        Behaviours.erase(i);
        delete *i;
    }
    Behaviours.clear();
    
    for (vector<Connector*>::iterator i = Connectors.begin(); i != Connectors.end(); i++) {
        delete *i; 
    }
    Connectors.clear();
}


/*
 ------------------------------------------------
 Update
 ------------------------------------------------
 */
void Level::onUpdate()
{
    updateLevelTime();
    
    updateCamera();
    updateBehaviours();
    hero.update(deltaTime);
    updateAntenna();
    updateConnectors();
    
    updateLevelEditor();
}

void Level::updateLevelTime()
{
    double lastTime = time;
    time = getElapsedSeconds();
    deltaTime = time - lastTime;
}

void Level::updateConnectors()
{
    if (!heroCanAct()) return;
    
    vector<Connector*>::iterator connectIterator;
    for (connectIterator = Connectors.begin(); connectIterator != Connectors.end(); connectIterator++) {
        (*connectIterator)->connect(&hero, deltaTime);
        (*connectIterator)->update(deltaTime);
    }
}

void Level::updateAntenna()
{
    if (heroGotAntenna) {
        if (!waitAntennaAnimation) {
            waitAntennaAnimation = new Waiter(2.0);
        }
        waitAntennaAnimation->update(deltaTime);
        if (!waitAntennaAnimation->active && !goingToNextLevel) {
            goingToNextLevel = true;
            nextLevel();
        }
        else if (waitAntennaAnimation->active) {
            double time = waitAntennaAnimation->elapsedTime();
            if (time < 0.2) {
                rectAlpha = time * 4;
            }
            else if (time > 0.2 && time < 0.4) {
                rectAlpha = 1.0 - ((time-0.2) * 4);
            }
            else {
                rectAlpha = 0.0;
            }
        }
    }
    
    else { // check for collision with hero
        float x = antennaPosition.x;
        float y = antennaPosition.y;
        Rectf antennaRect(x, y, x + antenna.getWidth(), y + antenna.getHeight());
        if (antennaRect.intersects(hero.rect)) {
            heroGotAntenna = true;
            startAntennaAnimation();
        }
    }
}

void Level::updateBehaviours()
{
    vector<Behaviour*>::iterator i;
    for (i = Behaviours.begin(); i != Behaviours.end(); i++) {
        (*i)->update(deltaTime);
    }
}

void Level::addBehaviour(Behaviour *behaviour)
{
    Behaviours.push_back(behaviour);
}

void Level::startAntennaAnimation()
{
    assert(heroGotAntenna);
    playSound(soundAntennaFlash);
}


/*
 ------------------------------------------------
 Render
 ------------------------------------------------
 */
void Level::onRender()
{
    gl::pushMatrices();
        renderBg();
        camera.applyTransform();
        renderLevel();
        renderAntenna();
        renderTiles();
        renderConnectors();
        hero.render();
    gl::popMatrices();
    
    // flash by antenna
    if (rectAlpha > 0.01) {
        gl::color(1.0, 1.0, 1.0, rectAlpha);
        gl::drawSolidRect(fullscreenRect);
    }
    
    renderLevelEditor();
}

void Level::renderBg()
{
    gl::color(1, 1, 1, 1.0);
    gl::draw(bg, Vec2f(0, 0));
}

void Level::renderLevel()
{
    gl::color(1, 1, 1, 1.0);
    gl::draw(level, Vec2f(0, 0));
}

void Level::renderAntenna()
{
    gl::color(1, 1, 1);
    gl::draw(antenna, antennaPosition);
}

void Level::renderTiles()
{
    if (currentMode != 2) return;
    
    gl::color(1, 1, 1, 0.5);
    
    int beginX = (camera.position.x - getWindowWidth() / 2.0) / TILESIZE;
    int beginY = (camera.position.y - getWindowHeight() / 2.0) / TILESIZE;
    beginX = max(beginX, 0);
    beginY = max(beginY, 0);
    
    int endX = beginX + (getWindowWidth()/TILESIZE) + 1;
    int endY = beginY + (getWindowHeight()/TILESIZE) + 1;
    endX = min(width, endX);
    endY = min(height, endY);
    
    for (unsigned y = beginY; y < endY; y++) {
        for (unsigned x = beginX; x < endX; x++) {
            int currentId = Map[x][y];
            if (currentId != -1) {
                Vec2f position(x * TILESIZE, y * TILESIZE);
                gl::draw(Tile[currentId], position);
            }
        }
    }
}

void Level::renderConnectors()
{
    vector<Connector*>::iterator connectIterator;
    for (connectIterator = Connectors.begin(); connectIterator != Connectors.end(); connectIterator++) {
        (*connectIterator)->render();
    }
}



/*
 ------------------------------------------------
 Init/Exit
 ------------------------------------------------
 */
void Level::onInit()
{
    loadTiles();
    
    // load textures
    string idWithoutExtension = id.substr(0, id.length()-4);
    level = loadImage(loadAsset("levels/" + idWithoutExtension + ".png"));
    antenna = loadImage(loadAsset("antenna.png"));
    bg = loadImage(loadAsset("level_bg.png"));
    
    // setup camera
    camera.viewArea.set(width * TILESIZE, height * TILESIZE);
    
    // other
    Director::getInstance().setTheFadeTime(2.0);
}

void Level::onExit()
{}

void Level::nextLevel()
{
    assert(goingToNextLevel);
    
    // calculate next level number
    int dot = id.find_last_of(".");
    assert(dot > 2);
    string levelNrString = id.substr(2, dot-2); // 2 because lv has 2 chars
    int levelNr = atoi(levelNrString.c_str()) + 1;
    
    // Prevent loading too many levels
    if (levelNr > 3) {
        // last level reached
        Director::getInstance().setTheFadeTime(3.0);
        Director::getInstance().changeScene(new IntroScene);
        return;
    }
        
    Director::getInstance().setTheFadeTime(3.0);
    Level* level = new Level;
    level->loadLevelFromFile("lv" + to_string(levelNr) + ".xml");
    level->getCamera().setFocus(&level->getHero().position);
    Director::getInstance().changeScene(level);
}



/*
 ------------------------------------------------
 Tiles
 ------------------------------------------------
 */
void Level::setTileAtPosition(int x, int y, int tileId)
{
    /*
     assert(x >= 0 && x < width);
     assert(y >= 0 && y < height);
     assert(tileId >= -1 && tileId < numberOfTiles);
     */
    x = std::min(x, width-1);
    x = std::max(x, 0);
    y = std::min(y, height-1);
    y = std::max(y, 0);
    tileId = std::min(tileId, numberOfTiles-1);
    
    Map[x][y] = tileId;
}

void Level::setTileAtPosition(int index, int tileId)
{
    int x = index % width;
    int y = index / width;
    setTileAtPosition(x, y, tileId);
}

void Level::setTileAtMousePos(int x, int y, int tileId)
{
    int trueX = (x + camera.position.x) / TILESIZE;
    int trueY = (y + camera.position.y) / TILESIZE;
    setTileAtPosition(trueX, trueY, tileId);
}

int Level::getTileIdAtPosition(Vec2f position, bool isTilePosition)
{
    int x = position.x;
    int y = position.y;
    if (! isTilePosition) {
        x /= TILESIZE;
        y /= TILESIZE;
    }
    
    x = std::min(x, width-1);
    x = std::max(x, 0);
    y = std::min(y, height-1);
    y = std::max(y, 0);
    
    return Map[x][y];
}


void Level::addConnector(Connector* connector)
{
    Connectors.push_back(connector);
}

void Level::removeConnector(Connector *connector)
{
    if (Connectors.size() < 1) return;
    
    vector<Connector*>::iterator iter;
    for (iter = Connectors.begin(); iter != Connectors.end(); iter++) {
        Connector* c = (*iter);
        if (c == connector) {
            hero.disconnect(connector);
            Connectors.erase(iter);
            return;
        }
    }
}

int Level::toTilepos(float pixelPos)
{
    return int(pixelPos/TILESIZE);
}


/*
------------------------------------------------
    Level Loading + Saving
------------------------------------------------
*/

void Level::loadTiles()
{
    numberOfTiles = 1;
    Tile = new gl::Texture[numberOfTiles];
    
    for (unsigned i = 0; i < numberOfTiles; i++) {
        ostringstream path;
        path << "tiles/" << i << ".png";
        gl::Texture image = loadImage( loadAsset(path.str()) );
        Tile[i] = image;
    }
}

void Level::loadLevelFromFile(string pathToLevel)
{   
    // root
    id = pathToLevel;
    string realPath = "levels/" + pathToLevel;
    realPath = getAssetPath(realPath).string();
    XmlTree doc((loadFile(realPath)));
    XmlTree root = doc.getChild("level");
    
    // width + height
    width = root.getChild("width").getValue<int>();
    height = root.getChild("height").getValue<int>();
    Map.resize(width, vector<int>(height, -1));
    
    // hero start position
    int hx = root.getChild("heroX").getValue<int>();
    int hy = root.getChild("heroY").getValue<int>();
    hero.position.set(hx * TILESIZE, hy * TILESIZE);
    
    // antenna
    int ax = root.getChild("antennaX").getValue<int>();
    int ay = root.getChild("antennaY").getValue<int>();
    antennaPosition.set(ax * TILESIZE, ay * TILESIZE);
    
    // level-data
    stringstream stream;
    stream << root.getChild("data").getValue();
    int index = 0;
    while (stream.good() && index < width * height) {
        int tileId;
        stream >> tileId;
        setTileAtPosition(index, tileId);
        index++;
    }
    
    // attractors
    if (root.hasChild("attractors")) {
        XmlTree firstAttractor = root.getChild("attractors");
        for (XmlTree::Iter child = firstAttractor.begin(); child != firstAttractor.end(); child++) {
            float x = child->getChild("x").getValue<float>();
            float y = child->getChild("y").getValue<float>();
            Attractor* attractor = new Attractor(x, y);
            attractor->autoConnectTime = child->getChild("autoConnectTime").getValue<float>(attractor->autoConnectTime);
            attractor->autoDisconnectTime = child->getChild("autoDisconnectTime").getValue<float>();
            attractor->autoConnectRadius = child->getChild("autoConnectRadius").getValue<float>();
            attractor->connectTimeout = child->getChild("connectTimeout").getValue<float>();
            attractor->attractForce = child->getChild("attractForce").getValue<float>(attractor->attractForce);
            attractor->maxAttractForce = child->getChild("maxAttractForce").getValue<float>(attractor->maxAttractForce);
            
            addConnector(attractor);
        }
    }
    
    // detractors
    if (root.hasChild("detractors")) {
        XmlTree firstDetractor = root.getChild("detractors");
        for (XmlTree::Iter child = firstDetractor.begin(); child != firstDetractor.end(); child++) {
            float x = child->getChild("x").getValue<float>();
            float y = child->getChild("y").getValue<float>();
            Detractor* detractor = new Detractor(x, y);
            detractor->autoConnectTime = child->getChild("autoConnectTime").getValue<float>(detractor->autoConnectTime);
            detractor->autoDisconnectTime = child->getChild("autoDisconnectTime").getValue<float>();
            detractor->autoConnectRadius = child->getChild("autoConnectRadius").getValue<float>();
            detractor->connectTimeout = child->getChild("connectTimeout").getValue<float>();
            detractor->bounceForce = child->getChild("bounceForce").getValue<float>(detractor->bounceForce);
            detractor->maxBounceForce = child->getChild("maxBounceForce").getValue<float>(detractor->maxBounceForce);
            
            addConnector(detractor);
        }
    }
    
}

void Level::saveLevel()
{
    XmlTree level("level", "");
    level.push_back(XmlTree ("width", to_string(width)));
    level.push_back(XmlTree ("height", to_string(height)));
    level.push_back(XmlTree ("heroX", to_string(toTilepos(heroStartPosition.x))));
    level.push_back(XmlTree ("heroY", to_string(toTilepos(heroStartPosition.y))));
    level.push_back(XmlTree ("antennaX", to_string(toTilepos(antennaPosition.x))));
    level.push_back(XmlTree ("antennaY", to_string(toTilepos(antennaPosition.y))));
    
    stringstream levelData;
    levelData << "\n";
    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            levelData << Map[x][y] << ' ';
        }
        levelData << "\n";
    }
    level.push_back(XmlTree ("data", levelData.str()));
    
    // save Connectors
    XmlTree attractors("attractors", "");
    XmlTree detractors("detractors", "");
    vector<Connector*>::iterator iter;
    for (iter = Connectors.begin(); iter != Connectors.end(); iter++) {
        Attractor* attractor = dynamic_cast<Attractor*>(*iter);
        Detractor* detractor = dynamic_cast<Detractor*>(*iter);
        
        if (attractor) {
            XmlTree attr("attractor", "");
            
            attr.push_back(XmlTree ("x", to_string(attractor->getPosition().x)));
            attr.push_back(XmlTree ("y", to_string(attractor->getPosition().y)));
            attr.push_back(XmlTree ("autoConnectTime", to_string(attractor->autoConnectTime)));
            attr.push_back(XmlTree ("autoDisconnectTime", to_string(attractor->autoDisconnectTime)));
            attr.push_back(XmlTree ("autoConnectRadius", to_string(attractor->autoConnectRadius)));
            attr.push_back(XmlTree ("connectTimeout", to_string(attractor->connectTimeout)));
            attr.push_back(XmlTree ("attractForce", to_string(attractor->attractForce)));
            attr.push_back(XmlTree ("maxAttractForce", to_string(attractor->maxAttractForce)));
            
            attractors.push_back(attr);
        }
        else if (detractor) {
            XmlTree detr("detractor", "");
            
            detr.push_back(XmlTree ("x", to_string(detractor->getPosition().x)));
            detr.push_back(XmlTree ("y", to_string(detractor->getPosition().y)));
            detr.push_back(XmlTree ("autoConnectTime", to_string(detractor->autoConnectTime)));
            detr.push_back(XmlTree ("autoDisconnectTime", to_string(detractor->autoDisconnectTime)));
            detr.push_back(XmlTree ("autoConnectRadius", to_string(detractor->autoConnectRadius)));
            detr.push_back(XmlTree ("connectTimeout", to_string(detractor->connectTimeout)));
            detr.push_back(XmlTree ("bounceForce", to_string(detractor->bounceForce)));
            detr.push_back(XmlTree ("maxBounceForce", to_string(detractor->maxBounceForce)));
            
            detractors.push_back(detr);
        }
    }
    level.push_back(attractors);
    level.push_back(detractors);
    
    level.write(writeFile(getAssetFolder() + "levels/" + id));
}



/*
 ------------------------------------------------
    Helpers
 ------------------------------------------------
 */
Hero& Level::getHero()
{
    return hero;
}

float Level::getGravity()
{
    return gravity;
}

Level* Level::getCurrent()
{
    Level* currentLevel = dynamic_cast<Level*>(Director::getInstance().getScene());
    if (currentLevel) {
        return currentLevel;
    }
    return 0;
}

string Level::getId() const
{
    return id;
}

bool Level::heroCanAct()
{
    return !heroGotAntenna;
}


/*
 ------------------------------------------------
    Level Editor
 ------------------------------------------------
 */
void Level::updateLevelEditor()
{
    if (currentMode != 1) {
        updateFreeMovingCam();
    }
    updateParameters();
}

void Level::updateParameters()
{
    
} 

void Level::updateFreeMovingCam()
{
    float moveSpeed = 10;
    if (Gamepad::keyWIsDown) {
        freeCam.y += -moveSpeed;
    }
    else if (Gamepad::keySIsDown) {
        freeCam.y += moveSpeed;
    }
    if (Gamepad::keyAIsDown) {
        freeCam.x += -moveSpeed;
    }
    else if (Gamepad::keyDIsDown) {
        freeCam.x += moveSpeed;
    }
}

void Level::renderLevelEditor()
{
    gl::drawString("Mode:", Vec2f(getWindowWidth()-120, 4));
    Vec2f modePosition(getWindowWidth()-85, 4);
    switch (currentMode) {
        case 1: // Play Mode
            gl::drawString("Playing", modePosition);
            break;
            
        case 2: // Tile Editing Mode
            gl::drawString("Edit Tiles", modePosition);
            break;
            
        case 3: // Attractor Mode
            gl::drawString("Attractor", modePosition);
            break;
        
        case 4: // Detractor Mode
            gl::drawString("Detractor", modePosition);
            break;
        
        case 9: // Antenna
            gl::drawString("Antenna", modePosition);
            break;
            
        case 0: // Hero Startpoint
            gl::drawString("Hero Startpoint", modePosition);
            break;
            
        default:
            break;
    }
    
}

void Level::switchLevelEditorMode(int toMode)
{
    if (toMode < 0 || toMode > 9) {
        return;
    }
    
    currentMode = toMode;
    
    if (toMode != 1) {
        if (&camera.getFocus() != &freeCam) { // only change focus when adresses differ
            freeCam = hero.position;
            camera.setFocus(&freeCam);
            camera.viewArea.set(0.0, 0.0);
        }
    }
    
    // Play Mode
    else if (toMode == 1) {
        camera.setFocus(&hero.position);
        camera.viewArea.set(width * TILESIZE, height * TILESIZE);
    }
}


void Level::handleMouseDown(MouseEvent event)
{
    // Hero Reposition, while playing
    if (currentMode == 1) {
        int mouseX = event.getX() - getWindowWidth() / 2.0;
        int mouseY = event.getY() - getWindowHeight() / 2.0;
        if (event.isLeft()) {
            hero.position.set(mouseX + camera.position.x, mouseY + camera.position.y);
        }
        return;
    }
    
    // Hero Startpoint
    if (currentMode == 0) {
        int mouseX = event.getX() - getWindowWidth() / 2.0;
        int mouseY = event.getY() - getWindowHeight() / 2.0;
        if (event.isLeft()) {
            heroStartPosition.set(mouseX + camera.position.x, mouseY + camera.position.y);
        }
        return;
    }
    
    // Tile Edit Mode
    if (currentMode == 2) {
        int mouseX = event.getX() - getWindowWidth() / 2.0;
        int mouseY = event.getY() - getWindowHeight() / 2.0;
        if (event.isLeft()) {
            setTileAtMousePos(mouseX, mouseY, 0);
        }
        else if (event.isRight()) {
            setTileAtMousePos(mouseX, mouseY, -1);
        }
        return;
    }
    
    // Antenna Position Mode
    if (currentMode == 9) {
        int mouseX = event.getX() - getWindowWidth() / 2.0;
        int mouseY = event.getY() - getWindowHeight() / 2.0;
        if (event.isLeft()) {
            antennaPosition.set(mouseX + camera.position.x, mouseY + camera.position.y);
        }
        return;
    }
    
    // Attractor
    if (currentMode == 3) {
        int mouseX = (event.getX() - getWindowWidth() / 2.0) + camera.position.x;
        int mouseY = (event.getY() - getWindowHeight() / 2.0) + camera.position.y;
        
        if (event.isLeft()) {
            Connector* connector = pickConnector(Vec2f(mouseX, mouseY), Connectors);
            if (connector) { // select Attractor
                Attractor* attractor = dynamic_cast<Attractor*>(connector);
                selectedAttractor = attractor;
            }
            else { // add Attractor
                Attractor* attractorToAdd = new Attractor(mouseX, mouseY);
                addConnector(attractorToAdd);
                selectedAttractor = attractorToAdd;
            }
        }
        else if (event.isRight()) { // remove Attractor
            Connector* connector = pickConnector(Vec2f(mouseX, mouseY), Connectors);
            if (!connector) return;
            Attractor* attractor = dynamic_cast<Attractor*>(connector);
            if (attractor) {
                removeConnector(attractor);
            }
        }
        return;
    }
    
    // Detractor
    if (currentMode == 4) {
        int mouseX = (event.getX() - getWindowWidth() / 2.0) + camera.position.x;
        int mouseY = (event.getY() - getWindowHeight() / 2.0) + camera.position.y;
        
        if (event.isLeft()) {
            Connector* connector = pickConnector(Vec2f(mouseX, mouseY), Connectors);
            if (connector) { // select Attractor
                Detractor* detractor = dynamic_cast<Detractor*>(connector);
                selectedDetractor = detractor;
            }
            else { // add Detractor
                Detractor* detractorToAdd = new Detractor(mouseX, mouseY);
                addConnector(detractorToAdd);
                selectedDetractor = detractorToAdd;
            }
        }
        else if (event.isRight()) { // remove Detractor
            Connector* connector = pickConnector(Vec2f(mouseX, mouseY), Connectors);
            if (!connector) return;
            Detractor* detractor = dynamic_cast<Detractor*>(connector);
            if (detractor) {
                removeConnector(detractor);
            }
        }
        return;
    }
    
    
}

void Level::handleMouseDrag(MouseEvent event)
{
    if (currentMode == 2) {
        int mouseX = event.getX() - getWindowWidth() / 2.0;
        int mouseY = event.getY() - getWindowHeight() / 2.0;
        if (event.isLeft()) {
            setTileAtMousePos(mouseX, mouseY, 0);
        }
        else if (event.isRight()) {
            setTileAtMousePos(mouseX, mouseY, -1);
        }
        return;
    }   
}


Attractor* Level::getSelectedAttractor()
{
    return selectedAttractor;
}

Detractor* Level::getSelectedDetractor()
{
    return selectedDetractor;
}

Connector* Level::pickConnector(Vec2f atPosition, vector<Connector*> &connectors)
{
    if (connectors.size() > 0) {
        vector<Connector*>::iterator iter;
        for (iter = connectors.begin(); iter != connectors.end(); iter++) {
            Connector* connector = (*iter);
            Vec2f distance = connector->getPosition() - Vec2f(atPosition.x, atPosition.y);
            float radius = 25;
            if (distance.length() < radius) {
                return connector;
            }
        }
    }
    return 0;
}








