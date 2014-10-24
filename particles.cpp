//
//  particles.cpp
//  Qpt
//
//  Created by Markus on 09.04.13.
//
//

#include "particles.h"
#include <iostream>

using namespace std;
using namespace cinder;

// Particle

Particle::Particle(Vec2f setPosition, Vec2f setDiretion, float setVelocity, float setSize, Color setColor)
{
    position = setPosition;
    direction = setDiretion;
    velocity = setVelocity;
    size = setSize;
    color = setColor;
}

Particle::Particle(Vec2f setPosition, float setSize)
{
    position = setPosition;
    size = setSize;
}

void Particle::update()
{
    position += (velocity * direction);
    
    /*
    if (position.x < 0 || position.x > 800) {
        direction.x = -direction.x;
    }
    if (position.y < 0) {
        position.y = 800/16 * 9;
    }
    else if (position.y > 800/16 * 9) {
        position.y = 0;
    }
     */
}

void Particle::draw()
{
    //gl::drawSolidCircle(position, size);
    gl::drawSolidRect(Rectf(position.x, position.y, position.x + 2, position.y +2));
}



// Particle Controller

ParticleController::ParticleController()
{
    particles = new list<Particle>;
}

void ParticleController::update()
{
    for (list<Particle>::iterator p = particles->begin(); p != particles->end(); p++) {
        p->update();
    }
}

void ParticleController::draw()
{
    for (list<Particle>::iterator p = particles->begin(); p != particles->end(); p++) {
        p->draw();
    }
}

void ParticleController::draw(Channel32f& withChannel, Vec2f cursorPos)
{
    for (list<Particle>::iterator p = particles->begin(); p != particles->end(); p++) {
        Vec2f DirToCursor = cursorPos - p->position;
        DirToCursor.normalize();
        
        Vec2f newLoc = p->position + DirToCursor * 10.0f;
        newLoc.x = constrain( newLoc.x, 0.0f, withChannel.getWidth() - 1.0f );
        newLoc.y = constrain( newLoc.y, 0.0f, withChannel.getHeight() - 1.0f );
        p->position = newLoc;
        
        float greyValue = withChannel.getValue(p->position);
        gl::color(Color(greyValue, greyValue, greyValue));
        p->draw();
        
    }
}

void ParticleController::addParticle(int amount, Vec2f atPosition) {
    for (unsigned i = 0; i < amount; i++) {
        Vec2f direction = Vec2f(Rand::randFloat(-1.0, 1.0), Rand::randFloat(-1.0, 1.0));
        direction.normalize();
        float velocity = Rand::randFloat(7.0);
        float size = velocity * 1.5;
        Color randColor = Color((velocity + 1)/7.0, 0, 0);
        Particle* p = new Particle(atPosition, direction, velocity, size, randColor);
        particles->push_back(*p);
    }
}

void ParticleController::addFixedParticle(int xIndex, int yIndex) {
    float x = (xIndex + 0.5) * 4.0;
    float y = (yIndex + 0.5) * 4.0;
    float size = 2; //(sin(x*y)) * 5 + 1;//Rand::randFloat(1, 4);
    Particle* p = new Particle(Vec2f(x, y), size);
    particles->push_back(*p);
    //cout << x << ", " << y << endl;
}





