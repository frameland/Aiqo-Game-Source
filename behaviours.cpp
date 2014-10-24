//
//  behaviours.cpp
//  Qpt
//
//  Created by Markus on 27.05.13.
//
//

#include "behaviours.h"

Behaviour::Behaviour()
    : active(true)
    , waitTime(0)
{ }

Behaviour::~Behaviour()
{}

bool Behaviour::isWaiting(float delta)
{
    if (waitTime <= 0.00001) return false;
    waitTime -= delta;
    return true;
}

// Vec2f
TweenVec2f::TweenVec2f(Vec2f* source, Vec2f target, double time, int type) : Behaviour()
{
    this->source = source;
    start = *source;
    end = target;
    this->time = time;
    currentTime = 0.0;
    this->type = type;
}

TweenVec2f::~TweenVec2f() {}

void TweenVec2f::update(float delta)
{
    if (!active || isWaiting(delta)) return;
    
    currentTime += delta;
    if (currentTime > time) {
        active = false;
    }
    else {
        float x = tween(currentTime, start.x, end.x, time, type);
        float y = tween(currentTime, start.y, end.y, time, type);
        source->set(x, y);
    }
}


// Float
TweenFloat::TweenFloat(float* source, float target, double time, int type) : Behaviour()
{
    this->source = source;
    start = *source;
    end = target;
    this->time = time;
    currentTime = 0.0;
    this->type = type;
}

TweenFloat::~TweenFloat() {}

void TweenFloat::update(float delta)
{
    if (!active || isWaiting(delta)) return;
    
    currentTime += delta;
    if (currentTime > time) {
        active = false;
    }
    else {
        *source = tween(currentTime, start, end, time, type);
    }
}



// Waiter
Waiter::Waiter(float howLong) : Behaviour()
{
    this->waitTime = howLong;
    this->duration = howLong;
}

Waiter::~Waiter() {}

void Waiter::update(float delta)
{
    if (!active) return;
    if (!isWaiting(delta)) {
        active = false;
    }
}

double Waiter::elapsedTime()
{
    return duration - waitTime;
}



