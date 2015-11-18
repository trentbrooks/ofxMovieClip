#include "ofxBufferedMovieClip.h"

//--------------------------------------------------------------
ofxBufferedMovieClip::ofxBufferedMovieClip(){

}


//--------------------------------------------------------------
void ofxBufferedMovieClip::init(ofxBufferedSequenceLoaderThread* imageSequence, float frameDelay)
{
    ofxMovieClip<ofPixels>::init(imageSequence, frameDelay);
    imageSequenceThread = imageSequence; // just another pointer ref to avoid casting
    //ofLogError() << "* Error: cannot initiate ofxBufferedMovieClip from external image sequence loader. Use intenal loadSequence() instead.";
    //return;
}

// Drawing
//--------------------------------------------------------------
void ofxBufferedMovieClip::draw() {
    
    if(isCustomSize)
        getTexturePtr()->draw(position.x, position.y, width, height);
    else
        getTexturePtr()->draw(position.x, position.y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

void ofxBufferedMovieClip::draw(float x, float y) {
    
    if(isCustomSize)
        getTexturePtr()->draw(x, y, width, height);
    else
        getTexturePtr()->draw(x, y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

void ofxBufferedMovieClip::draw(float x, float y, float w, float h) {
    
	getTexturePtr()->draw(x, y, w, h);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

ofTexture* ofxBufferedMovieClip::getTexturePtr() {
    
    // only allocate + load if the playhead has changed, otherwise return cached copy
    if(playheadCopy != playheadCount) {
        
        ofPixels* px = activeAsset->imageFrames[playheadCount];
        //ofLog() << "Old playhead: " << playheadCopy << ", New: " << playheadCount;
        //imageSequenceThread->clearImageFromPlayhead(activeAsset,playheadCopy);
        
        playheadCopy = playheadCount;
        imageSequenceThread->updateLoadFromPlayhead(playheadCopy);
        if(!px->isAllocated()) {
            
            // thread has not finished loading image yet - will not draw
            // should probably also hold the playhead until this has occured? atm, the playhead can be too fast for the loader
            return pixelsTexture;
        }
        if(!pixelsTexture->isAllocated()) pixelsTexture->allocate(*px);
        pixelsTexture->loadData(*px);
    }
    
    return pixelsTexture;
}

// Updates
//--------------------------------------------------------------
void ofxBufferedMovieClip::tick()
{
	ofxMovieClip<ofPixels>::tick();
    
}

