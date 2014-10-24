//
//  camera.h
//  Qpt
//
//  Created by Markus on 29.04.13.
//
//

#ifndef Qpt_camera_h
#define Qpt_camera_h

#include "cinder/Vector.h"
using namespace cinder;

class TCamera {
private:
    Vec2f* focus;
    
public:
    Vec2f position;
    Vec2f viewArea;
    
    TCamera();
    void update();
    void setFocus(Vec2f* focus);
    Vec2f& getFocus();
    
    void applyTransform();
};

#endif
