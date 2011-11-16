#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(30);
    
    
    // load all the images using ofxFBOImageSequenceLoader
    // frameLabel, number of images, path, image type, number of zeros to append to path before number, start count from
    fis.loadAndCreateSequence("frogWalk", 40, "frogs/PIMP_FROG", "png", 4, 1);
    fis.loadAndCreateSequence("horseWalk", 11, "horses/frame", "png", 2);
    
    // create as many movieclips as you want and pass in a reference to the ofxFBOImageSequenceLoader & desired frame rate
    mc.init(&fis, 30.0f);
	mc.gotoAndPlay("frogWalk");

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(255);
    
    ofSetColor(0);
    ofDrawBitmapString("Press 1 to play frog animation",20,20);
    ofDrawBitmapString("Press 2 to play horse animation",20,40);
    ofDrawBitmapString("Press 3 to reverse",20,60);
    ofDrawBitmapString("Press 4 to stop",20,80);
    ofDrawBitmapString("Press 5 to resume",20,100);
     
    
    ofSetColor(255);
    ofEnableAlphaBlending();
    mc.drawFrame(250, 300);
    ofDisableAlphaBlending();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case '1':
            mc.gotoAndPlay("frogWalk");
            break;
        case '2':
            mc.gotoAndPlay("horseWalk");
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