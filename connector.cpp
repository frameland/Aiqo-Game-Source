//
//  connector.cpp
//  Qpt
//
//  Created by Markus on 06.05.13.
//
//

#include "connector.h"
#include "hero.h"
#include "level.h"

Connector::Connector(float x, float y)
    : position(x, y)
    , autoConnectTime(1)
    , autoDisconnectTime(-1)
    , currentConnectTime(0)
    , connecting(false)
    , connectTimeout(1.0)
    , currentTimeout(0)
    , autoConnectRadius(200)
    , hero(0)
    , sprite(0)
{ }

Connector::Connector() {}

Connector::~Connector()
{
    delete sprite;
}

void Connector::update(float delta)
{
    if (isConnecting()) {
        currentConnectTime += delta;
        if (isConnected()) {
            connecting = false;
            currentConnectTime = autoConnectTime + connectTimeout;
        }
    }
    
    if (isConnected()) {
        if (distanceToHero(hero) > autoConnectRadius) {
            disconnect();
        }
        if (autoDisconnectTime != -1) {
            currentDisconnectTime += delta;
            if (currentDisconnectTime > autoDisconnectTime) {
                disconnect();
            }
        }
    }
    
}

void Connector::render() {}


// hero connects to soon again

void Connector::connect(Hero* withHero, float delta)
{
    if (isConnected()) return;
    
    if (currentTimeout >= connectTimeout) {
        currentTimeout = connectTimeout;
        float distance = distanceToHero(withHero);
        if (!connecting && distance < autoConnectRadius) {
            hero = withHero;
            hero->connect(this);
            connecting = true;
        }
    }
    else {
        currentTimeout += delta;
    }
}

void Connector::disconnect(bool disconnectFromHero)
{
    currentConnectTime = 0;
    currentDisconnectTime = 0;
    currentTimeout = 0;
    connecting = false;
    
    if (disconnectFromHero && hero) {
        hero->disconnect(this);
    }
    hero = 0;
}

bool Connector::isConnecting()
{
    return (connecting && hero && currentConnectTime < autoConnectTime);
}

bool Connector::isConnected()
{
    return currentConnectTime >= autoConnectTime && hero;
}


float Connector::distanceToHero(Hero* withHero)
{
    Vec2f distanceToHero(withHero->position);
    distanceToHero -= position;
    return distanceToHero.length();
}

// return float between 0..1
float Connector::connectionProgress()
{
    return currentConnectTime / autoConnectTime;
}

Vec2f& Connector::getPosition()
{
    return position;
}


/*
 ============================
    Attractor
 ============================
 */
Attractor::Attractor(float x, float y)
        : Connector(x, y)
        , attractForce(2.0)
        , maxAttractForce(10)
{
    sprite = new Sprite(Vec2f(x, y), "attractor.png");
    sprite->setMidHandled();
}

Attractor::~Attractor() {}

void Attractor::update(float delta)
{
    Connector::update(delta);
    if (isConnected()) {
        if (isConnected()) {
            hero->gravityAffection = 0.0;
            
            Vec2f direction(position - hero->position);
            direction.normalize();
            
            hero->acceleration += direction * attractForce;
            hero->acceleration.x = MIN(hero->acceleration.x, maxAttractForce);
            hero->acceleration.x = MAX(hero->acceleration.x, -maxAttractForce);
            hero->acceleration.y = MIN(hero->acceleration.y, maxAttractForce);
            hero->acceleration.y = MAX(hero->acceleration.y, -maxAttractForce);
        }
    }
}

void Attractor::render()
{
    Level* level = Level::getCurrent();
    assert(level);
    
    if (isConnected()) {
        sprite->setColor(1.0, 1.0, 1.0);
    }
    else {
        sprite->setColor(0.7, 0.7, 0.7);
    }
    if (isConnecting() || isConnected()) {
        gl::drawLine(hero->position, position);
    }
    
    sprite->rotation += 2;
    sprite->position.set(position);
    sprite->render();
    
    if (isConnecting()) {
        gl::color(1.0, 0.5, 0, 1.0);
        Vec2f progress(position - hero->position);
        float length = progress.length();
        progress.normalize();
        progress *= connectionProgress() * length;
        gl::drawLine(hero->position, hero->position + progress);
    }
    
    // draw Selection border
    /*
    if (this == level->getSelectedAttractor()) {
        gl::drawStrokedCircle(position, 35);
    }
     */
}


void Attractor::disconnect(bool disconnectFromHero)
{
    hero->gravityAffection = 1.0;
    Connector::disconnect(disconnectFromHero);
}





/*
 ============================================
    Detractor
 ============================================
*/
Detractor::Detractor(float x, float y)
    : Connector(x, y)
    , bounceForce(10.0)
    , maxBounceForce(80.0)
{
    connectTimeout = 0.2;
    sprite = new Sprite(Vec2f(x, y), "detractor.png");
    sprite->setMidHandled();
}

Detractor::~Detractor() {}

void Detractor::update(float delta)
{
    Connector::update(delta);
    if (isConnected()) {
        if (isConnected()) {
            hero->gravityAffection = 0.0;
            
            Vec2f direction(position - hero->position);
            direction.normalize();
            
            hero->acceleration -= direction * bounceForce;
            hero->acceleration.x = MIN(hero->acceleration.x, maxBounceForce);
            hero->acceleration.x = MAX(hero->acceleration.x, -maxBounceForce);
            hero->acceleration.y = MIN(hero->acceleration.y, maxBounceForce);
            hero->acceleration.y = MAX(hero->acceleration.y, -maxBounceForce);
        }
    }
}

void Detractor::render()
{
    Level* level = Level::getCurrent();
    assert(level);
    
    if (isConnected()) {
        sprite->setColor(1.0, 1.0, 1.0);
    }
    else {
        sprite->setColor(0.7, 0.7, 0.7);
    }
    if (isConnecting() || isConnected()) {
        gl::drawLine(hero->position, position);
    }
    
    sprite->rotation += 2;
    sprite->position.set(position);
    sprite->render();
    
    if (isConnecting()) {
        gl::color(0.2, 0.5, 1.0, 1.0);
        Vec2f progress(position - hero->position);
        float length = progress.length();
        progress.normalize();
        progress *= connectionProgress() * length;
        gl::drawLine(hero->position, hero->position + progress);
    }
    
    // draw Selection border
    /*
    if (this == level->getSelectedDetractor()) {
        gl::drawStrokedCircle(position, 35);
    }
     */
    
}

void Detractor::disconnect(bool disconnectFromHero)
{
    hero->gravityAffection = 1.0;
    Connector::disconnect(disconnectFromHero);
}







