//
//  particles.h
//  Qpt
//
//  Created by Markus on 09.04.13.
//
//

#ifndef Qpt_particles_h
#define Qpt_particles_h
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace std;

class Particle {
    Vec2f position;
    Vec2f direction;
    float velocity;
    float size;
    Color color;
    
public:
    Particle(Vec2f setPosition, Vec2f setDiretion, float setVelocity, float setSize, Color setColor);
    Particle(Vec2f setPosition, float setSize);
    void update();
    void draw();
    friend class ParticleController;
};


class ParticleController {
    std::list<Particle>* particles;
    
public:
    ParticleController();
    void draw();
    void draw(Channel32f& withChannel, Vec2f cursorPos);
    void update();
    void addParticle(int amount, Vec2f atPosition);
    void addFixedParticle(int x, int y);
    void removeParticle(int amount);
};

#endif
