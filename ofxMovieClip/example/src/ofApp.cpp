#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // load a folder of images as image sequence -optional 2nd paramter is frame label, if empty it defaults to folder name. eg "frogs"
    imageSequence.loadSequence("frogs");
    imageSequence.loadSequence("horses");
    //imageSequence.loadSequence("end_anim");
    //imageSequence.loadSequence("ufo");
    
    // required for threaded loader
    //imageSequence.startThread();
    
    // create a movieclip and pass in a reference to the ofxImageSequenceLoader & desired frame delay
    mc.init(&imageSequence, 1.0/30.0f); // eg. around 30fps
	mc.gotoAndPlay("horses");
    
    
    // image spritesheet - smurf walking
    imageSpriteSheet.loadSpriteSheet("spritesheet/smurf_sprite.png", 128, 128, 16, "smurf");
    ssMc.init(&imageSpriteSheet, 1.0/30.0);
    
    // threaded pixel movieclip
    threadedLoader.loadSequence("ufo");
    threadedLoader.startThread();
    bufferMc.init(&threadedLoader, 1/30.0f);
    //bufferMc.setLooping(false);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //ofBackground(255,255,0);
    ofSetColor(255);
    mc.draw(0,0,ofGetWidth()/2,ofGetHeight()/2);
    
    ssMc.draw(0, ofGetHeight()-ssMc.getHeight());
    
    bufferMc.draw(0,0);//ofGetWidth()/2+1,0,ofGetWidth()/2,ofGetHeight()/2);
    //if(ofGetKeyPressed())
    //threadedLoader.clearBufferFromPlayhead(pxMc.activeAsset, 30, pxMc.getPlayhead());
    
    
    ofSetColor(0);
    ofDrawBitmapString("Press 1 to play frog animation",20,20);
    ofDrawBitmapString("Press 2 to play horse animation",20,40);
    ofDrawBitmapString("Press 3 to reverse",20,60);
    ofDrawBitmapString("Press 4 to stop",20,80);
    ofDrawBitmapString("Press 5 to resume",20,100);
    ofDrawBitmapString("Playhead: " + ofToString(bufferMc.getPlayhead())+"/"+ofToString(bufferMc.getFrameCount()),20,120);
    ofDrawBitmapString("Click/drag to adjust frame delay: " + ofToString(mc.getFrameDelay()),20,140);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
        case 'x':
            threadedLoader.clearImageData();
            //threadedLoader.startThread();
            bufferMc.gotoAndPlay(0);
            break;
    }
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    float newFrameDelay = ofMap(x, 0, ofGetWidth(), 0, 0.05);
    mc.setFrameDelay(newFrameDelay);
    ssMc.setFrameDelay(newFrameDelay);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}