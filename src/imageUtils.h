//
//  imageUtils.h
//  ParticlesTest
//
//  Created by Itai Asseo on 6/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ParticlesTest_imageUtils_h
#define ParticlesTest_imageUtils_h

#include "ofMain.h"


class imageUtils
{
    public:
        static ofTexture* invert_color_image(ofImage* im, float color);
        static void invert_color_image(ofImage* im, float color, ofTexture* t);
        static unsigned char* getRGB(float h, float s, float b, float a=255);
        static void invertGrayscale(ofImage* im);
};

#endif
