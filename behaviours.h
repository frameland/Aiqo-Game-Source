 //
//  behaviours.h
//  Qpt
//
//  Created by Markus on 27.05.13.
//
//

#ifndef Qpt_behaviours_h
#define Qpt_behaviours_h

#include "cinder/Vector.h"
#include "PennerEasing/Easing.h"

using namespace ci;

class Behaviour {
public:
    Behaviour();
    virtual ~Behaviour();
    virtual void update(float delta) = 0;
    bool isWaiting(float delta);
    bool active;
    double waitTime;
};

class TweenFloat : public Behaviour {
    float start;
    float end;
    float* source;
    double time;
    double currentTime;
    int type;
public:
    TweenFloat(float* source, float target, double time, int type);
    ~TweenFloat();
    void update(float delta);
};

class TweenVec2f : public Behaviour {
    Vec2f start;
    Vec2f end;
    Vec2f* source;
    double time;
    double currentTime;
    int type;
public:
    TweenVec2f(Vec2f* source, Vec2f target, double time, int type);
    ~TweenVec2f();
    void update(float delta);
};

class Waiter : public Behaviour {
private:
    double duration;
public:
    Waiter(float howLong);
    ~Waiter();
    void update(float delta);
    double elapsedTime();
};


#endif
