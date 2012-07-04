//
//  RgbToHsb.cpp
//  ParticlesTest
//
//  Created by Itai Asseo on 6/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ofMain.h"

int getBrightness(float r, float g, float b) {
    return MAX(MAX(r,g),b);
}

int getLightness(float r, float g, float b) {
    return (r + g + b) / 3.f;
}

int getHue(float r, float g, float b) {
    float max = MAX(MAX(r, g), b);
    float min = MIN(MIN(r, g), b);
    float delta = max-min;
    if (r==max) return (0 + (g-b) / delta) * 42.5;  //yellow...magenta
    if (g==max) return (2 + (b-r) / delta) * 42.5;  //cyan...yellow
    if (b==max) return (4 + (r-g) / delta) * 42.5;  //magenta...cyan
    return 0;
}

int getSaturation(float r, float g, float b) {
    float min = MIN(MIN(r, g), b);
    float max = MAX(MAX(r, g), b);
    float delta = max-min;
    if (max!=0) return int(delta/max*255);
    return 0;
}

void ofxSetColorHSB(float h, float s, float b, float a=255) {
    if (b==0) return ofSetColor(0,0,0,a); //black
    if (s==0) return ofSetColor(b,b,b,a); //gray
    
    h *= 6.0 / 255.0;
    s *= 1.0 / 255.0;
    
    float x = b * (1-s);
    float y = b * (1-s*(h-int(h)));
    float z = b * (1-s*(1-(h-int(h))));
    
    switch (int(h)) {
        case 0: return ofSetColor(b,z,x,a); //red
        case 1: return ofSetColor(y,b,x,a); //green
        case 2: return ofSetColor(x,b,z,a); 
        case 3: return ofSetColor(x,y,b,a); //blue
        case 4: return ofSetColor(z,x,b,a); 
        case 5: return ofSetColor(b,x,y,a); //back to red
    }
}