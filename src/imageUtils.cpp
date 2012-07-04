//
//  imageUtils.cpp
//  ParticlesTest
//
//  Created by Itai Asseo on 6/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "imageUtils.h"

 ofTexture* imageUtils::invert_color_image(ofImage* im, float color) {
    int bytes_per_pixel = im->bpp / 8;
    
    
    /*if (im->type != OF_IMAGE_COLOR_ALPHA || bytes_per_pixel != 4)
     // This function only works on RGB images.
     cout << "i guess this is the problem " << bytes_per_pixel << " " << im->type << " " << OF_IMAGE_COLOR_ALPHA << endl;
     return NULL;
     */
    int w = im->width;
    int h = im->height;
    // (t) will hold the result of the processing that we do on (im).
    ofTexture* t = new ofTexture();
    t->allocate(w, h, GL_RGBA);
    //cout << "what " << bytes_per_pixel << endl;
    unsigned char* im_pixels = im->getPixels(); // These are the pixel data of im,
    unsigned char* p = new unsigned char[w*h*bytes_per_pixel]; // and here's where we keep our processed pixel data.
    for (int i = 0; i < w*h; i++) {
        int base = i * bytes_per_pixel;
        
        // Assuming the pixels come in RGB order, here we grab the intensity
        // (a value in the range [0,255]) of each color channel of the image.
        unsigned char r = im_pixels[base + 0];
        unsigned char g = im_pixels[base + 1];
        unsigned char b = im_pixels[base + 2];
        unsigned char a = im_pixels[base + 3];
        
        unsigned char* rgb = getRGB(color, 255, 255);
        //unsigned char* rgb;
        
        p[base + 0] = rgb[0]; // Invert R.
        p[base + 1] = rgb[1]; // Invert G.
        p[base + 2] = rgb[2]; // Invert B.
        p[base + 3] = a; // keep alpha.
    }
    t->loadData(p, w, h, GL_RGBA); // This copies our processed pixel data into a texture that can be displayed.
    
    delete [] p; // Because loadData performed a copy, we no longer need to store the processed pixel data.
    
    return t;
}  

void imageUtils::invert_color_image(ofImage* im, float color, ofTexture* t) {
    int bytes_per_pixel = im->bpp / 8;
    
    
    /*if (im->type != OF_IMAGE_COLOR_ALPHA || bytes_per_pixel != 4)
     // This function only works on RGB images.
     cout << "i guess this is the problem " << bytes_per_pixel << " " << im->type << " " << OF_IMAGE_COLOR_ALPHA << endl;
     return NULL;
     */
    int w = im->width;
    int h = im->height;
    // (t) will hold the result of the processing that we do on (im).
    //ofTexture* t = new ofTexture();
    //t->allocate(w, h, GL_RGBA);
    //cout << "what " << bytes_per_pixel << endl;
    unsigned char* im_pixels = im->getPixels(); // These are the pixel data of im,
    unsigned char* p = new unsigned char[w*h*bytes_per_pixel]; // and here's where we keep our processed pixel data.
    for (int i = 0; i < w*h; i++) {
        int base = i * bytes_per_pixel;
        
        // Assuming the pixels come in RGB order, here we grab the intensity
        // (a value in the range [0,255]) of each color channel of the image.
        unsigned char r = im_pixels[base + 0];
        unsigned char g = im_pixels[base + 1];
        unsigned char b = im_pixels[base + 2];
        unsigned char a = im_pixels[base + 3];
        
        unsigned char* rgb = getRGB(color, 255, 255);
        //unsigned char* rgb;
        
        p[base + 0] = rgb[0]; // Invert R.
        p[base + 1] = rgb[1]; // Invert G.
        p[base + 2] = rgb[2]; // Invert B.
        p[base + 3] = a; // keep alpha.
    }
    t->loadData(p, w, h, GL_RGBA); // This copies our processed pixel data into a texture that can be displayed.
    
    delete [] p; // Because loadData performed a copy, we no longer need to store the processed pixel data.
    
    return;
}  

unsigned char* imageUtils::getRGB(float h, float s, float b, float a) {
    
    unsigned char* rgb= new unsigned char[4];
    
    //if (b==0) return rgb(0,0,0,a); //black
    //if (s==0) return rgb(b,b,b,a); //gray
    
    h *= 6.0 / 255.0;
    s *= 1.0 / 255.0;
    
    float x = b * (1-s);
    float y = b * (1-s*(h-int(h)));
    float z = b * (1-s*(1-(h-int(h))));
    
    switch (int(h)) {
        case 0: 
            rgb[0]=b;rgb[1]=z;rgb[2]=x;rgb[3]=a;
            return rgb; //red
            break;
        case 1:
            rgb[0]=y;rgb[1]=b;rgb[2]=x;rgb[3]=a;
            return rgb; //green
            break;
        case 2:
            rgb[0]=x;rgb[1]=b;rgb[2]=z;rgb[3]=a;
            return rgb; //blue
            break;
        case 3: 
            rgb[0]=x;rgb[1]=y;rgb[2]=b;rgb[3]=a;
            return rgb; //blue
            break;
        case 4:
            rgb[0]=z;rgb[1]=x;rgb[2]=b;rgb[3]=a;
            return rgb; //blue
            break;
        case 5:
            rgb[0]=b;rgb[1]=x;rgb[2]=y;rgb[3]=a;
            return rgb; //blue
            break;
    }
}

void imageUtils::invertGrayscale(ofImage* im)
{
    int bytes_per_pixel = im->bpp / 8;
    
    int w = im->width;
    int h = im->height;
    
    cout << bytes_per_pixel << " " << w << " " << h << endl;
    unsigned char* im_pixels = im->getPixels();
    for (int i = 0; i < w*h; i++) {
        int base = i * bytes_per_pixel;
        
        // Assuming the pixels come in RGB order, here we grab the intensity
        // (a value in the range [0,255]) of each color channel of the image.
        im_pixels[base + 0] += 255;
    }
    
    //unsigned char* p =  pix->getPixels();
    //cout << p[0] << endl;
    //for (int i = 0; i < pix->getWidth(); i++){
    //    p[i] = 255-p[i];
    //}
    //cout << p[0] << endl;
    
}