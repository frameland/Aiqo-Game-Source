//
//  sprite.cpp
//  Qpt
//
//  Created by Markus on 27.05.13.
//
//

#include "sprite.h"

Sprite::Sprite(Vec2f atPosition, const string& path)
{
    position = atPosition;
    image = loadImage(loadAsset(path));
    alpha = 1.0;
    color.set(1.0, 1.0, 1.0);
    scale.set(1.0, 1.0);
    rotation = 0;
}

void Sprite::setColor(float r, float g, float b)
{
    color.set(r, g, b);
}

void Sprite::setAlpha(float a)
{
    alpha = a;
}

void Sprite::setScale(float x, float y)
{
    scale.set(x, y);
}

void Sprite::setMidHandled(bool midHandle)
{
    assert(image);
    if (midHandle) {
        handle.set(image.getWidth()/2.0, image.getHeight()/2.0);
    }
    else {
        handle.set(0.0, 0.0);
    }
}

void Sprite::render()
{
    assert(image);
    
    gl::color(color.r, color.g, color.b, alpha);
    gl::pushMatrices();
        gl::translate(position);
        gl::rotate(rotation);
        gl::scale(scale);
        gl::translate(-handle);
        gl::draw(image, Vec2f(0, 0));
    gl::popMatrices();
}











