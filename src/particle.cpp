#include "particle.h"


Particle::Particle(int x, int y, double vel, ofTexture* tex, float &particleHue) {
    _x = x;
    //_img = img;
    _particleHue = &particleHue;
     setup();
    
    _c.setHsb(_r, 255, 255);
    
    _texture = tex; //imageUtils::invert_color_image(_img, _r);
       
    _y = y;
    _vel = vel;
    
   // cout << _texture << endl;
    
}

ofTexture* Particle::getTexture()
{
    return _texture;
}

void Particle::updateColor(float particleHue)
{
    //*_particleHue = particleHue;
    //cout << *_particleHue << endl;
    
}


void Particle::setup()
{
    _dir = 0;
    _radius = ofRandom(50,100);
    _sineSpeed = ofRandom(-0.04,0.04);
    _size = ofRandom(5,20);
    _r = ofRandom(*_particleHue, *_particleHue+20);
    //cout << *_particleHue << endl;
    _g = ofRandom(0,255);
    _b = ofRandom(0,255);
    
    _originX = _x;
}

void Particle::draw()
{
    //ofSetColor(_r, _g, _b);
    
    
    //ofSetColor(c);
    //ofCircle(_x, _y, _size);
    ofPoint p;
    p.set(_x, _y);
    ofEnableAlphaBlending();
    //cout << _texture << endl;
    _texture->draw(p, _size, _size);
    //_img.draw(p, _size, _size);
    
    ofDisableAlphaBlending();;
}

void Particle::update()
{	
    
    //cout << "_particleHue " << _particleHue << endl;
    _dir+=_sineSpeed;
	_x=_originX + sin(_dir)*_radius;
	_y-=_vel;
}

void Particle::getYPos()
{
    printf("pos: %f", _y);
}

bool Particle::outOfBounds()
{
    
    if (_y<0 || _y>ofGetHeight() || _x<0 || _x>ofGetWidth()){
        return true;
    }else{
        return false;
    }
}

void Particle::reset(int x, int y, double vel) {
    _x = x;
    _y = y;
    _vel = vel;
   
    setup();
    //_texture = imageUtils::invert_color_image(_img, _r);
}