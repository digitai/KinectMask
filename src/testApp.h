#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

#define NUM_IMAGES 10

class testApp : public ofBaseApp{
    
    
    
	public:
        
		void setup();
		void update();
		void draw();
    
        void create_particles();
        void update_particles();
        void reap_particles();
    
    
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void drawPointCloud();
        void exit(); 
    
    
    
    
    
        
private:
    
    //methods:
    
    void initGUI();
    void guiEvent(ofxUIEventArgs &e);
    void swapMessage();
    void setupKinect();
    void setupGLMask();
    void setupTextures();
    
    //members:
    
    ofColor _bgColor1,_bgColor2;
    float _particleHue1,particleHue2;
    float color1,color2;
    float _particle_hue;
    float dThroshNear;
    float dThroshFar;
    //ofTexture* textureArray[NUM_IMAGES];
    
    unsigned char* topLayerPixels;
    unsigned char* dPixels;
    
    ofTexture* _texture;
    bool showTexture;
    
    float a1;
    float b1;
    float r1;
    
    int numParticles;
    int numParticles_2;
    int particle1_shape;
    int particle2_shape;
    ofTexture* particleTexture1;
    ofTexture* particleTexture2;
    float particle1Color;
    float particle2Color;
    
    ofImage img;
    
    ofxUICanvas *gui;
    
    void initImages();
    
    vector <Particle*> myParticles;
    vector <Particle*> myParticles2;
    vector <ofImage> imageArray;
    
    ofImage depthImage;
    ofShader maskShader;
    ofImage topLayer;
    ofImage mask;
    ofImage bottomLayer;
    ofxCvGrayscaleImage cvImage;
    
    //KINECT STUFF:
    ofxKinect kinect;
	
#ifdef USE_TWO_KINECTS
	ofxKinect kinect2;
#endif
	
	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
	
	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	
	int nearThreshold;
	int farThreshold;
	
	int angle;
	
	// used for viewing the point cloud
	ofEasyCam easyCam;
};
