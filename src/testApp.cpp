#include "testApp.h"


#define RADIUS 10
#define NUM_PARTICLES 800
#define NUM_PARTICLES_2 800
#define X_RANGE 500
#define BG_VALUE "Background Color"
#define SET_PARTICLE_HUE "Set Particle Color"
#define SET_PARTICLE_HUE2 "Set Particle 2 Color"
#define THRESH_NEAR "threshHold Near"
#define THRESH_FAR "Threshhold Far"

#define TEXTURE_OFFSET_X 40
#define TEXTURE_OFFSET_Y 40
#define SET_PARTICLE1_SHAPE "Particle 1 Shape"
#define SET_PARTICLE2_SHAPE "Particle 2 Shape"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetBackgroundAuto(false);
    ofEnableSmoothing();
    ofSetFrameRate(120);
    ofSetVerticalSync(true);
    
    
    _bgColor1.set(0, 0, 0, 80);
    _bgColor2.set(40, 40, 40, 60);
    //textureArray[NUM_IMAGES];
    numParticles = 0;
    numParticles_2 = 0;
    particle2_shape = 0;
    particle1_shape = 0;
    a1 = mouseX - ofGetWidth()/2;
    b1 = mouseY - ofGetHeight()/2;
    r1 = atan2(a1, b1) / (PI / 180); 
    initImages();
    setupTextures();
    setupKinect();
    setupGLMask();
    initGUI();
    
}

void testApp::setupGLMask()
{
    cvImage.allocate(kinect.width, kinect.height);    
    topLayer.allocate(kinect.width, kinect.height, OF_IMAGE_COLOR);
	mask.loadImage("mask.png");
	bottomLayer.loadImage("bottomLayer.png");
    
    topLayerPixels = new unsigned char[kinect.width*kinect.height*3];
    for(int i = 0; i < kinect.width*kinect.height; i++){
        int base = i*3;
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        topLayerPixels[base] = r;
        topLayerPixels[base+1] = g;
        topLayerPixels[base+2] = b;
    }
    topLayer.setFromPixels(topLayerPixels, kinect.width, kinect.height, OF_IMAGE_COLOR);
    
	maskShader.load("composite");
	maskShader.begin();
	maskShader.setUniformTexture("Tex0", topLayer.getTextureReference(), 0);
	maskShader.setUniformTexture("Tex1", depthImage.getTextureReference(), 1);
	maskShader.end();
}

void testApp::initGUI()
{
    gui = new ofxUICanvas(0,40,320,420);		
    gui->addWidgetDown(new ofxUILabel("THE AVATAR PROJECT", OFX_UI_FONT_LARGE)); 
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,255.0,100.0,BG_VALUE));
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,235.0,100.0,SET_PARTICLE_HUE));
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,9.0,100.0,SET_PARTICLE1_SHAPE));
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,235.0,100.0,SET_PARTICLE_HUE2));
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,9.0,100.0,SET_PARTICLE2_SHAPE));
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,4000.0,100.0,THRESH_NEAR));
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,4000.0,100.0,THRESH_FAR));
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent); 
    
    gui->loadSettings("GUI/guiSettings.xml");
}

void testApp::initImages()
{
    for(int i = 0; i <NUM_IMAGES; i++){
        ofImage tempImage;
        string str = "p";

        str = "p"+ofToString(i)+".png"; 
        tempImage.loadImage(str);
        imageArray.push_back(tempImage);
    }
}

void testApp::setupTextures()
{
    ofImage img;
    img.loadImage("156x156.png");
    particleTexture1 = imageUtils::invert_color_image(&img, ofRandom(20, 40));
    particleTexture2 = imageUtils::invert_color_image(&img, ofRandom(20, 40));
}

void testApp::setupKinect()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
	
    
    
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();
	
#ifdef USE_TWO_KINECTS
	kinect2.init();
	kinect2.open();
#endif
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = false;
	
	//ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
    dThroshNear = 1000;
    dThroshFar = 1500;
    kinect.setDepthClipping(dThroshNear,dThroshFar);
	kinect.enableDepthNearValueWhite(false);
    kinect.setUseTexture(true);
    
	// start from the front
	bDrawPointCloud = false;
    
    depthImage.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
}

void testApp::guiEvent(ofxUIEventArgs &e)
{
	if(e.widget->getName() == BG_VALUE)	
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;    
        _bgColor1.set(slider->getScaledValue(), slider->getScaledValue(), slider->getScaledValue(), 80);
    }else if(e.widget->getName() == SET_PARTICLE_HUE){
        ofxUISlider *slider = (ofxUISlider *) e.widget; 
        _particle_hue = slider->getScaledValue();
        float c = slider->getScaledValue();
        particle1Color = c;
        imageUtils::invert_color_image(&imageArray[particle1_shape], particle1Color, particleTexture1);
                
    }else if(e.widget->getName() == SET_PARTICLE1_SHAPE){
        ofxUISlider *slider = (ofxUISlider *) e.widget; 
        
        if (round(slider->getScaledValue())!=particle1_shape){
            particle1_shape = round(slider->getScaledValue());
            imageUtils::invert_color_image(&imageArray[particle1_shape], particle1Color, particleTexture1);
            cout << "particle shape: " << particle1_shape << " particle color: " << particle1Color << endl;
        }
                    
    }else if(e.widget->getName() == SET_PARTICLE_HUE2){
        ofxUISlider *slider = (ofxUISlider *) e.widget; 
        _particle_hue = slider->getScaledValue();
        float c = slider->getScaledValue();
        particle2Color = c;
        imageUtils::invert_color_image(&imageArray[particle2_shape],particle2Color, particleTexture2);
        
    }else if(e.widget->getName() == SET_PARTICLE2_SHAPE){
        ofxUISlider *slider = (ofxUISlider *) e.widget; 
        if (round(slider->getScaledValue())!=particle2_shape){
            particle2_shape = round(slider->getScaledValue());
            imageUtils::invert_color_image(&imageArray[particle2_shape], particle2Color, particleTexture2);
            cout << "particle shape: " << particle2_shape << " particle color: " << particle2Color << endl;
        }
        
    }else if (e.widget->getName() == THRESH_NEAR){
        ofxUISlider *slider = (ofxUISlider *) e.widget; 
        dThroshNear = slider->getScaledValue();
         kinect.setDepthClipping(dThroshNear,dThroshFar);
    }else if (e.widget->getName() == THRESH_FAR){
        ofxUISlider *slider = (ofxUISlider *) e.widget; 
        dThroshFar = slider->getScaledValue();
         kinect.setDepthClipping(dThroshNear,dThroshFar);
    }
}

//--------------------------------------------------------------
void testApp::update(){
    
   
        
    //topLayer.setFromPixels(kinect.getPixels(), kinect.width,kinect.height, OF_IMAGE_COLOR);
    //topLayer.grabScreen(0, 0, ofGetWidth(), ofGetHeight());

    
    
    /*a1 = mouseX - ofGetWidth()/2;
    b1 = mouseY - ofGetHeight()/2;
    r1 = atan2(a1, b1) / (PI / 180);
    */
    //cout << "angle =  " << r1 << endl;
    
    if (numParticles<NUM_PARTICLES){
        float myX = ofRandom(0,ofGetWidth());
        float myY = ofGetHeight();
       // ofTexture* t = textureArray[particle1_shape];
        myParticles.push_back(new Particle(myX, myY, ofRandom(2,6), particleTexture1, _particle_hue));
        numParticles++; 
    }
    
    if (numParticles_2<NUM_PARTICLES_2){
        float myX = ofRandom(0,ofGetWidth());
        float myY = ofGetHeight();
        //ofTexture* t2 = textureArray[particle2_shape];
        myParticles2.push_back(new Particle(myX, myY, ofRandom(2,6), particleTexture2, _particle_hue));
        numParticles_2++; 
    }
    
   // cout << "numParticles =  " << numParticles << endl;
    
    update_particles();
    //kinect.getDepthPixelsRef().swapRgb();
    kinect.update();
    
    unsigned char *p = kinect.getDepthPixels();
    //depthImage.getPixelsRef() = kinect.getPixelsRef();
    
    unsigned char* pi = new unsigned char[kinect.width*kinect.height];
    for(int i = 0; i < kinect.width*kinect.height; i++){
        unsigned char r = p[i];
        pi[i] = 255-r;
    }
    depthImage.setFromPixels(pi, kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
    depthImage.getPixelsRef().mirror(false, true);
    depthImage.update();
    delete [] pi;
    //depthImage.getPixelsRef().swapRgb();
    
    
    //depthImage.update();
    //imageUtils::invertGrayscale(&depthImage);
    //depthImage.getPixelsRef().swapRgb();
    //cvImage.allocate(kinect.width, kinect.height);
   // cvImage.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
    
    //cvImage.threshold(10,true);
//    depthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height, GL_LUMINANCE);
	//kinect.setDepthClipping((float)nearThreshold,(float)farThreshold);
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		kinect.getPixels();
        
	}
	
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif
}

void testApp::update_particles()
{
    
    
    for (int i=0; i<numParticles; i++){
        
        if (myParticles[i]->outOfBounds()){
            float myX = ofRandom(0,ofGetWidth());
            float myY = ofGetHeight();
            myParticles[i]->reset(myX,myY, ofRandom(2,6));
        }else{
            myParticles[i]->update();
        }
    }
    
    for (int j=0; j<numParticles_2; j++){
        
        if (myParticles2[j]->outOfBounds()){
            float myX = ofRandom(0,ofGetWidth());
            float myY = ofGetHeight();
            myParticles2[j]->reset(myX,myY, ofRandom(2,6));
        }else{
            myParticles2[j]->update();
        }
    }
}


//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackgroundGradient(_bgColor1,_bgColor2, OF_GRADIENT_CIRCULAR);
    ofPoint point;
    
    point.x = 0;
    point.y = 0;
    ofSetColor(255, 255, 255);

	for (int i=0; i<numParticles; i++){
        
        myParticles[i]->draw();
    }
    
    for (int j=0; j<numParticles; j++){
        
        myParticles2[j]->draw();

    }
    
    ofEnableAlphaBlending();

	//then draw a quad for the top layer using our composite shader to set the alpha
	maskShader.begin();
	
	//our shader uses two textures, the top layer and the alpha
	//we can load two textures into a shader using the multi texture coordinate extensions
	glActiveTexture(GL_TEXTURE0_ARB);
	topLayer.getTextureReference().bind();
    
	glActiveTexture(GL_TEXTURE1_ARB);
	depthImage.getTextureReference().bind();
    
	//draw a quad the size of the frame
	glBegin(GL_QUADS);
	
	//move the mask around with the mouse by modifying the texture coordinates
	
	glMultiTexCoord2d(GL_TEXTURE0_ARB, TEXTURE_OFFSET_X, TEXTURE_OFFSET_Y);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, TEXTURE_OFFSET_X, TEXTURE_OFFSET_Y);		
	glVertex2f( 0, 0);
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, topLayer.getWidth()-TEXTURE_OFFSET_X, TEXTURE_OFFSET_Y);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, depthImage.getWidth()-TEXTURE_OFFSET_X, TEXTURE_OFFSET_Y);		
	glVertex2f( ofGetWidth(), 0);
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, topLayer.getWidth()-TEXTURE_OFFSET_X, topLayer.getHeight()-TEXTURE_OFFSET_Y);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, depthImage.getWidth()-TEXTURE_OFFSET_X, depthImage.getHeight()-TEXTURE_OFFSET_Y);
	glVertex2f( ofGetWidth(), ofGetHeight());
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, TEXTURE_OFFSET_X, topLayer.getHeight()-TEXTURE_OFFSET_Y);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, TEXTURE_OFFSET_X, depthImage.getHeight()-TEXTURE_OFFSET_Y);		
	glVertex2f( 0, ofGetHeight() );
	
	glEnd();
	
	//deactive and clean up
	glActiveTexture(GL_TEXTURE1_ARB);
	depthImage.getTextureReference().unbind();
	
	glActiveTexture(GL_TEXTURE0_ARB);
	topLayer.getTextureReference().unbind();
	
	maskShader.end();

    
    
    
   // ofPoint point;
    
    //point.x = 0;
    //point.y = 0;
   // depthImage.draw(point, ofGetWidth(), ofGetHeight());
    
    
    //kinect.drawDepth(0, 0, 400, 300);
    //cout << kinect.getRawDepthPixels() << endl;
	//ofDisableAlphaBlending();
   
    
    
	ofSetColor(255, 255, 255);     
    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 15);

    ofPoint p;
    p.set(200, 200);
    if (showTexture) _texture->draw(p, 400, 300);

	//drawPointCloud();
}

void testApp::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	glEnable(GL_DEPTH_TEST);
	mesh.drawVertices();
	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();
}

void testApp::keyPressed(int key){
    switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'm':
            ofTexture* pt = myParticles[0]->getTexture();
            cout  << particleTexture1 << " " << particle1_shape << pt <<  endl;
            break;
        
	}
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void testApp::exit()
{
	gui->saveSettings("GUI/guiSettings.xml");     
    delete gui;
}



