//
//  ease.cpp
//  Qpt
//
//  Created by Markus on 27.05.13.
//
//

#include "Easing.h"

float tween(float t,float b , float c, float d, int type)
{
    assert(type >= 0 && type <= 30);
    
    switch (type) {
        case EASE_LINEAR:
            return Linear::easeNone(t, b, c, d); break;

        case 1:
            return Quad::easeIn(t, b, c, d); break;
        case 2:
            return Quad::easeOut(t, b, c, d); break;
        case 3:
            return Quad::easeInOut(t, b, c, d); break;
            
        case 4:
            return Cubic::easeIn(t, b, c, d); break;
        case 5:
            return Cubic::easeOut(t, b, c, d); break;
        case 6:
            return Cubic::easeInOut(t, b, c, d); break;
        
        case 7:
            return Quart::easeIn(t, b, c, d); break;
        case 8:
            return Quart::easeOut(t, b, c, d); break;
        case 9:
            return Quart::easeInOut(t, b, c, d); break;
        
        case 10:
            return Quint::easeIn(t, b, c, d); break;
        case 11:
            return Quint::easeOut(t, b, c, d); break;
        case 12:
            return Quint::easeInOut(t, b, c, d); break;
        
        case 13:
            return Sine::easeIn(t, b, c, d); break;
        case 14:
            return Sine::easeOut(t, b, c, d); break;
        case 15:
            return Sine::easeInOut(t, b, c, d); break;
            
        case 16:
            return Expo::easeIn(t, b, c, d); break;
        case 17:
            return Expo::easeOut(t, b, c, d); break;
        case 18:
            return Expo::easeInOut(t, b, c, d); break;
         
        case 19:
            return Circ::easeIn(t, b, c, d); break;
        case 20:
            return Circ::easeOut(t, b, c, d); break;
        case 21:
            return Circ::easeInOut(t, b, c, d); break;
        
        case 22:
            return Back::easeIn(t, b, c, d); break;
        case 23:
            return Back::easeOut(t, b, c, d); break;
        case 24:
            return Back::easeInOut(t, b, c, d); break;
        
        case 25:
            return Bounce::easeIn(t, b, c, d); break;
        case 26:
            return Bounce::easeOut(t, b, c, d); break;
        case 27:
            return Bounce::easeInOut(t, b, c, d); break;
        
        case 28:
            return Elastic::easeIn(t, b, c, d); break;
        case 29:
            return Elastic::easeOut(t, b, c, d); break;
        case 30:
            return Elastic::easeInOut(t, b, c, d); break;
            
        default:
            break;
    }
    
    return 0.0;
}


