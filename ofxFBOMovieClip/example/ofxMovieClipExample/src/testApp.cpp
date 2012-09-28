#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(60);
    
    
    // load a folder of images as image sequence -optional 2nd paramter is frame label, if empty it defaults to folder name. eg "frogs"
    imageSequence.loadAndCreateSequence("frogs");
    imageSequence.loadAndCreateSequence("horses");
    
    // below would load 11 images from "horses/frame00.png" - "horses/frame10.png"
    // parameters: frame label, total images, path to images, image type, number of digits for the count (eg. 3 = 001, 2 = 01), count start (if your first image file is "horses/frame00.png" then start at 0) 
    //fis.loadAndCreateSequence("frogs", 40, "frogs/PIMP_FROG", "png", 4, 0);
    //fis.loadAndCreateSequence("horseWalk", 11, "horses/frame", "png", 2);
    
    
    // create as many movieclips as you want and pass in a reference to the ofxFBOImageSequenceLoader & desired frame rate/speed
    // 2nd parameter (frameSpeed): 1.0f or 60.0f / ofGetFrameRate() plays back at apps frame rate, 0.5f or or 30.0f / ofGetFrameRate() plays back at half speed, 2.0f or 120.0 / ofGetFrameRate() plays back twice as fast as app frame rate
    mc.init(&imageSequence, 20.0f / ofGetFrameRate()); // eg. 20fps
	mc.gotoAndPlay("frogs");
    
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
        
    ofSetColor(255);
    ofEnableAlphaBlending();
    mc.drawFrame(250, 300);
    ofDisableAlphaBlending();
    
    ofSetColor(0);
    ofDrawBitmapString("Press 1 to play frog animation",20,20);
    ofDrawBitmapString("Press 2 to play horse animation",20,40);
    ofDrawBitmapString("Press 3 to reverse",20,60);
    ofDrawBitmapString("Press 4 to stop",20,80);
    ofDrawBitmapString("Press 5 to resume",20,100);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case '1':
            mc.gotoAndPlay("frogs");
            break;
        case '2':
            mc.gotoAndPlay("horses");
            break;
        case '3':
            mc.reverse();
            break;
        case '4':
            mc.stop();
            break;
        case '5':
            mc.play();
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