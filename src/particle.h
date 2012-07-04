#ifndef _OF_PARTICLE // if this class hasn't been defined, the program can define it
#define _OF_PARTICLE // by using this if statement you prevent the class to be called more 

#include "ofMain.h"
#include "imageUtils.h"

class Particle {
    double _x, _y;
    double _vel, _dir;
    
    
public:
    Particle(int x, int y, double vel, ofTexture* tex, float &particleHue);
    
    ofTexture* getTexture();
    
    void updateColor(float particleHue);
    void draw();
    void setup();
    void update();
    bool outOfBounds();
    void getYPos();
    void reset (int x, int y, double vel);
   // ofTexture* invert_color_image(ofImage* im);
    
    ofTexture* _texture;
    int _size;
    float _r;
    float _g;
    float _b;
    float _sineSpeed;
    float _radius;
    float _originX;
    float *_particleHue;
    
    ofColor _c;
    
    ofImage* _img;
};

#endif 