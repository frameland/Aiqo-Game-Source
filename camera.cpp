//
//  camera.cpp
//  Qpt
//
//  Created by Markus on 29.04.13.
//
//

#include "camera.h"
#include "cinder/app/AppNative.h"

using namespace ci;

TCamera::TCamera()
    : position(0, 0)
    , viewArea(0, 0)
    , focus(0)
{}

void TCamera::update()
{
    if (focus) {
        float camLerp = 0.1;
        position.x += (focus->x - position.x) * camLerp;
        position.y += (focus->y - position.y) * camLerp;
    }
    
    // Bound check
    if (viewArea.x < 1) return;
    
    int width = ci::app::getWindowWidth()/2;
    int height = ci::app::getWindowHeight()/2;
    
    if (position.x < width) {
        position.x = width;
    }
    else if (position.x > MAX(viewArea.x - width, width)) {
        position.x = MAX(viewArea.x - width, width);
    }
    
    if (position.y < height) {
        position.y = height;
    }
    else if (position.y > MAX(viewArea.y - height, height)) {
        position.y = MAX(viewArea.y - height, height);
    }
}

void TCamera::setFocus(Vec2f* focus)
{
    this->focus = focus;
}

Vec2f& TCamera::getFocus()
{
    return *focus;
}

void TCamera::applyTransform()
{
    gl::translate(ci::app::getWindowWidth()/2.0, ci::app::getWindowHeight()/2.0);
    gl::translate(-position);
}