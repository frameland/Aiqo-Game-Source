//
//  All.h
//  Qpt
//
//  Created by Markus on 27.05.13.
//
//

#ifndef Qpt_All_h
#define Qpt_All_h

#include "Back.h"
#include "Bounce.h"
#include "Circ.h"
#include "Cubic.h"
#include "Elastic.h"
#include "Expo.h"
#include "Linear.h"
#include "Quad.h"
#include "Quart.h"
#include "Quint.h"
#include "Sine.h"

const int EASE_LINEAR		 = 0;

const int EASE_IN_QUAD		 = 1;
const int EASE_OUT_QUAD		 = 2;
const int EASE_IN_OUT_QUAD	 = 3;

const int EASE_IN_CUBIC		 = 4;
const int EASE_OUT_CUBIC	 = 5;
const int EASE_IN_OUT_CUBIC	 = 6;

const int EASE_IN_QUART		 = 7;
const int EASE_OUT_QUART	 = 8;
const int EASE_IN_OUT_QUART	 = 9;

const int EASE_IN_QUINT		 = 10;
const int EASE_OUT_QUINT	 = 11;
const int EASE_IN_OUT_QUINT	 = 12;

const int EASE_IN_SINE		 = 13;
const int EASE_OUT_SINE		 = 14;
const int EASE_IN_OUT_SINE	 = 15;

const int EASE_IN_EXPO		 = 16;
const int EASE_OUT_EXPO		 = 17;
const int EASE_IN_OUT_EXPO	 = 18;

const int EASE_IN_CIRC		 = 19;
const int EASE_OUT_CIRC		 = 20;
const int EASE_IN_OUT_CIRC	 = 21;

const int EASE_IN_BACK		 = 22;
const int EASE_OUT_BACK		 = 23;
const int EASE_IN_OUT_BACK	 = 24;

const int EASE_IN_BOUNCE	 = 25;
const int EASE_OUT_BOUNCE	 = 26;
const int EASE_IN_OUT_BOUNCE = 27;

const int EASE_IN_ELASTIC	 = 28;
const int EASE_OUT_ELASTIC	 = 29;
const int EASE_IN_OUT_ELASTIC = 30;


float tween(float t,float b , float c, float d, int type);

#endif
