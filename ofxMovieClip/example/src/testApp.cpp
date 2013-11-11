#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    
    // load a folder of images as image sequence -optional 2nd paramter is frame label, if empty it defaults to folder name. eg "frogs"
    imageSequence.loadAndCreateSequence("frogs");
    imageSequence.loadAndCreateSequence("horses");
    
    // create as many movieclips as you want and pass in a reference to the ofxImageSequenceLoader & desired frame rate/speed
    // 2nd parameter (frameSpeed): 1.0f or 60.0f / ofGetFrameRate() plays back at apps frame rate, 0.5f or or 30.0f / ofGetFrameRate() plays back at half speed, 2.0f or 120.0 / ofGetFrameRate() plays back twice as fast as app frame rate
    mc.init(&imageSequence, 20.0f / ofGetFrameRate()); // eg. 20fps
	mc.gotoAndPlay("frogs");
    
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(255,255,0);
    ofSetColor(255);
    mc.draw(250, 300);
    
    ofSetColor(0);
    ofDrawBitmapString("Press 1 to play frog animation",20,20);
    ofDrawBitmapString("Press 2 to play horse animation",20,40);
    ofDrawBitmapString("Press 3 to reverse",20,60);
    ofDrawBitmapString("Press 4 to stop",20,80);
    ofDrawBitmapString("Press 5 to resume",20,100);
    ofDrawBitmapString("Playhead: " + ofToString(mc.getPlayhead()),20,120);
    ofDrawBitmapString("Click/drag to adjust playback speed: " + ofToString(mc.getFrameSpeed()),20,140);
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
    
    // map mouse x position to playback speed between slow (0) and twice as fast (2.0)
    float newFrameSpeed = ofMap(x, 0, ofGetWidth(), 0, 2.0);
    mc.setFrameSpeed(newFrameSpeed);
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