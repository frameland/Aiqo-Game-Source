//
//  connector.h
//  Qpt
//
//  Created by Markus on 06.05.13.
//
//

#ifndef Qpt_connector_h
#define Qpt_connector_h

#include "cinder/Vector.h"
#include "sprite.h"

using namespace ci;

class Hero;


class Connector {
public:
    Vec2f position;
    
    float autoConnectTime;
    float currentConnectTime;
    
    float autoDisconnectTime;
    float currentDisconnectTime;
    
    bool connecting;
    
    float connectTimeout; // timeout before connecting again, after disconnect
    float currentTimeout;
    
    float autoConnectRadius;
    
    Sprite* sprite;
    Hero* hero;
    
//  Methods
    Connector();
    Connector(float x, float y);
    virtual ~Connector();
    
    virtual void update(float delta);
    virtual void render();
    
    void connect(Hero* withHero, float delta);
    virtual void disconnect(bool disconnectFromHero = true);
    
    bool isConnecting();
    bool isConnected();
    
    float distanceToHero(Hero* withHero);
    float connectionProgress();
    
    Vec2f& getPosition();
};



// ================================================
// Attactor
// ================================================
class Attractor : public Connector {
public:
    Attractor(float x, float y);
    ~Attractor();
    
    void update(float delta);
    void render();
    
    void disconnect(bool disconnectFromHero);
    
    float attractForce;
    float maxAttractForce;
};


// ================================================
// Detractor
// ================================================
class Detractor : public Connector {
public:
    Detractor(float x, float y);
    ~Detractor();
    
    void update(float delta);
    void render();
    
    void disconnect(bool disconnectFromHero);
    
    float bounceForce; // how much will the hero be thrown off
    float maxBounceForce;
};

#endif
