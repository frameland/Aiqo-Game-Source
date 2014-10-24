//
//  hero.cpp
//  Qpt
//
//  Created by Markus on 30.04.13.
//
//

#include "hero.h"
#include "gamepad.h"
#include "level.h"

Hero::Hero()
    : position(0, 0)
    , oldPosition(0, 0)
    , velocity(0, 0)
    , acceleration(0, 0)
    , rect(0, 0, width, height)
    , edgeSlideSpeed(0.2)
    , gravityAffection(1.0)
    , TOP_SPEED(500)
    , image(loadImage(loadAsset("hero.png")))
    , imageRight(loadImage(loadAsset("hero2.png")))
    , width(30)
    , height(30)
    , faceLeft(true)
{
    width = image.getWidth();
    height = image.getHeight();
}

void Hero::update(float delta)
{
    assert(Level::getCurrent() != 0);
    Level* level = Level::getCurrent();
    
    rect.set(position.x - width * 0.5, position.y - height * 0.5, position.x + width * 0.5, position.y + height * 0.5);
    
    float maxWalkSpeed = 20;
    if (Gamepad::keyLeftIsDown && level->heroCanAct()) {
        faceLeft = true;
        if (acceleration.x > -maxWalkSpeed) {
            acceleration.x += -1;
        }
    }
    else if (Gamepad::keyRightIsDown && level->heroCanAct()) {
        faceLeft = false;
        if (acceleration.x < maxWalkSpeed) {
            acceleration.x += 1;
        }
    }
    else {
        
    }
    
    /*
    else if (Gamepad::keyUpIsDown) {
        acceleration.y += -0.05;
        acceleration.y = MAX(acceleration.y, -0.8);
    }
    else if (Gamepad::keyDownIsDown) {
        acceleration.y += 0.05;
        acceleration.y = MIN(acceleration.y, 0.8);
    }
     */

    if (Gamepad::keySpaceIsHit && level->heroCanAct()) {
        disconnectAll();
    }
    
    // update position
    acceleration *= 0.9;
    velocity *= 0.9;
    velocity += acceleration;
    
    velocity.x = MIN(velocity.x, TOP_SPEED);
    velocity.x = MAX(velocity.x, -TOP_SPEED);
    velocity.y = MIN(velocity.y, TOP_SPEED);
    velocity.y = MAX(velocity.y, -TOP_SPEED);
    
    move((velocity + Vec2f(0.0, level->getGravity() * gravityAffection)) * delta);
    
}

void Hero::render()
{
    
    /* Debug Render
    gl::color(0.0, 1.0, 0.0);
    ci::gl::drawSolidRect(rect);
    
    gl::color(1.0, 0.0, 0.0);
    
    ci::gl::drawSolidCircle(position, 3);
    ci::gl::drawSolidCircle(rect.getUpperLeft(), 3);
    ci::gl::drawSolidCircle(rect.getUpperRight(), 3);
    ci::gl::drawSolidCircle(rect.getLowerLeft(), 3);
    ci::gl::drawSolidCircle(rect.getLowerRight(), 3);
    */
    
    gl::color(1.0, 1.0, 1.0, 1.0);
    if (faceLeft) {
        gl::draw(image, position - Vec2f(width/2, height/2));
    }
    else {
        gl::draw(imageRight, position - Vec2f(width/2, height/2));
    }
    
}

void Hero::move(Vec2f amount)
{
    float x = amount.x;
    float y = amount.y;
    
    if (amount.length() < 0.1) {
        return;
    }
    
    if (x < 0) {
        moveLeft(x);
    }
    else if (x > 0) {
        moveRight(x);
    }
    if (y > 0) {
        moveDown(y);
    }
    else if (y < 0) {
        moveUp(y);
    }
}

void Hero::moveLeft(float mSpeed)
{
    Level* level = Level::getCurrent();
    if (level) {
        Vec2f direction(1.0, 0.0);
        
        Vec2f topLeft = rect.getUpperLeft();
        topLeft += direction * mSpeed;
        
        Vec2f lowerLeft = rect.getLowerLeft();
        lowerLeft += direction * mSpeed;
        
        int topLeftId = level->getTileIdAtPosition(topLeft);
        int lowerLeftId = level->getTileIdAtPosition(lowerLeft);
        
        if (topLeftId == -1 && lowerLeftId == -1) {
            position += direction * mSpeed;
        }
        else {
            if (topLeftId == -1) {
                //direction.set(0, -edgeSlideSpeed * mSpeed);
                //position += direction * mSpeed;
            }
            else if (lowerLeftId == -1) {
                direction.set(0, edgeSlideSpeed * mSpeed);
                position += direction * mSpeed;
            }
            else {
                //int x = (int(topLeft.x) / Level::TILESIZE) * Level::TILESIZE + (width * 0.5) + 1 + Level::TILESIZE;
                //position.set(x, position.y);
            }
        }
    }
}

void Hero::moveRight(float mSpeed)
{
    Level* level = Level::getCurrent();
    if (level) {
        Vec2f direction(1.0, 0.0);
        
        Vec2f topRight = rect.getUpperRight();
        topRight += direction * mSpeed;
        
        Vec2f lowerRight = rect.getLowerRight();
        lowerRight += direction * mSpeed;
        
        int topRightId = level->getTileIdAtPosition(topRight);
        int lowerRightId = level->getTileIdAtPosition(lowerRight);
        
        if (topRightId == -1 && lowerRightId == -1) {
            position += direction * mSpeed;
        }
        else {
            if (topRightId == -1) {
                //direction.set(0, -edgeSlideSpeed * mSpeed);
                //position += direction * mSpeed;
            }
            else if (lowerRightId == -1) {
                direction.set(0, edgeSlideSpeed * mSpeed);
                position += direction * mSpeed;
            }
            else {
                //int x = (int(topRight.x) / Level::TILESIZE) * Level::TILESIZE - (width * 0.5) - 1;
                //position.set(x, position.y);
            }
        }
    }
}

void Hero::moveUp(float mSpeed)
{
    Level* level = Level::getCurrent();
    if (level) {
        Vec2f direction(0.0, 1.0);
        
        Vec2f topLeft = rect.getUpperLeft();
        topLeft += direction * mSpeed;
        
        Vec2f topRight = rect.getUpperRight();
        topRight += direction * mSpeed;
        
        int topLeftId = level->getTileIdAtPosition(topLeft);
        int topRightId = level->getTileIdAtPosition(topRight);
        
        if (topLeftId == -1 && topRightId == -1) {
            position += direction * mSpeed;
        }
        else {
            if (topLeftId == -1) {
                direction.set(-edgeSlideSpeed * mSpeed, 0);
                position += direction * mSpeed;
            }
            else if (topRightId == -1) {
                direction.set(edgeSlideSpeed * mSpeed, 0);
                position += direction * mSpeed;
            }
            else {
                //int y = (int(topLeft.y) / Level::TILESIZE) * Level::TILESIZE + (height * 0.5) + 1 + Level::TILESIZE;
                //position.set(position.x, y);
            }
        }
    }
}

void Hero::moveDown(float mSpeed)
{
    Level* level = Level::getCurrent();
    if (level) {
        Vec2f direction(0.0, 1.0);
        
        Vec2f lowerLeft = rect.getLowerLeft();
        lowerLeft += direction * mSpeed;
        
        Vec2f lowerRight = rect.getLowerRight();
        lowerRight += direction * mSpeed;
        
        int lowerLeftId = level->getTileIdAtPosition(lowerLeft);
        int lowerRightId = level->getTileIdAtPosition(lowerRight);
        
        if (lowerLeftId == -1 && lowerRightId == -1) {
            position += direction * mSpeed;
        }
        else {
            if (lowerLeftId == -1) {
                direction.set(-edgeSlideSpeed * mSpeed, 0);
                position += direction * mSpeed;
            }
            else if (lowerRightId == -1) {
                direction.set(edgeSlideSpeed * mSpeed, 0);
                position += direction * mSpeed;
            }
            else {
                //jitters if done this way
                //int y = level->toTilepos(lowerLeft.y) - int(height * 0.5) - 1;
                //position.y = y;
            }
            
        }
    }
}



Vec2f Hero::getTilePosition()
{
    int x = position.x / Level::TILESIZE;
    int y = position.y / Level::TILESIZE;
    return Vec2f(x, y);
}


void Hero::connect(Connector *withConnector)
{
    connectedTo.push_back(withConnector);
}

void Hero::disconnect(Connector* withConnector)
{
    if (!withConnector || connectedTo.size() == 0) return;
    
    std::vector<Connector*>::iterator i;
    for (i = connectedTo.begin(); i != connectedTo.end(); i++) {
        if (*i == withConnector) {
            (*i)->disconnect(false);
            connectedTo.erase(i);
            return;
        }
    }
}

void Hero::disconnectAll()
{
    if (connectedTo.size() == 0) return;
    
    playSound(soundDisconnect);
    std::vector<Connector*>::iterator i;
    for (i = connectedTo.begin(); i != connectedTo.end(); i++) {
        (*i)->disconnect(false);
    }
    
    connectedTo.clear();
}












