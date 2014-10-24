//
//  hero.h
//  Qpt
//
//  Created by Markus on 30.04.13.
//
//

#ifndef Qpt_hero_h
#define Qpt_hero_h

#include "connector.h"

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include <vector>
#include "audio.h"

using namespace cinder;

class Hero {
public:
    Hero();
    
    void update(float delta);
    void render();
    
    //if mSpeed == 1024 then mSpeed = this->speed
    void move(Vec2f amount);
    void moveLeft(float speed);
    void moveRight(float speed);
    void moveUp(float speed);
    void moveDown(float speed);
    
    void applyGravity();
    
    void connect(Connector* withConnector);
    void disconnect(Connector* withConnector);
    void disconnectAll();
    
    Vec2f getTilePosition();
    
    Rectf rect;
    Vec2f position;
    Vec2f oldPosition;
    Vec2f velocity;
    Vec2f acceleration;
    
    gl::Texture image;
    gl::Texture imageRight;
    
    float width;
    float height;
    float edgeSlideSpeed;
    float gravityAffection;
    float TOP_SPEED = 800.0;
    bool faceLeft;
    
    std::vector<Connector*> connectedTo;
};

#endif
