//
//  gamepad.h
//  Qpt
//
//  Created by Markus on 29.04.13.
//
//

#ifndef Qpt_gamepad_h
#define Qpt_gamepad_h

class Gamepad {
public:
    static bool keyRightIsDown;
    static bool keyLeftIsDown;
    static bool keyDownIsDown;
    static bool keyUpIsDown;
    
    static bool keyWIsDown;
    static bool keyAIsDown;
    static bool keySIsDown;
    static bool keyDIsDown;
    
    static bool keySpaceIsHit;
    static int spaceDownTime;
};

#endif
