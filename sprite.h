//
//  sprite.h
//  Qpt
//
//  Created by Markus on 27.05.13.
//
//

#ifndef Qpt_sprite_h
#define Qpt_sprite_h

#include "utility.h"
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace app;

class Sprite {
public:
    Vec2f position;
    Vec2f scale;
    Vec2f handle;
    gl::Texture image;
    Color color;
    float alpha;
    float rotation;
    
    Sprite(Vec2f atPosition, const string& path);
    
    void setColor(float r, float g, float b);
    void setAlpha(float a);
    void setScale(float x, float y);
    void setMidHandled(bool midHandle = true);
    
    void render();
};

#endif
