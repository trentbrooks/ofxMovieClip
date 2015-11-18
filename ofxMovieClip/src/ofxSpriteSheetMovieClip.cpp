#include "ofxSpriteSheetMovieClip.h"

//--------------------------------------------------------------
ofxSpriteSheetMovieClip::ofxSpriteSheetMovieClip(){
    sliceWidth= sliceHeight = -1;
}


//--------------------------------------------------------------
void ofxSpriteSheetMovieClip::init(ofxSpriteSheetLoader* spriteSheetLoader, float frameDelay)
{
	this->spriteSheet = spriteSheetLoader;
    activeAsset = spriteSheet->assetCollections[frameLabelId];
    
    this->frameDelayInSeconds = frameDelay;
    
    // auto grab the width and height of the first asset
    if(width == -1 && height == -1 && activeAsset->imageFrames.size() > 0) {
        width = sliceWidth = activeAsset->sliceWidth; // activeAsset->imageFrames[0]->getWidth();
        height = sliceHeight = activeAsset->sliceHeight; //activeAsset->imageFrames[0]->getWidth();
    }
}

//--------------------------------------------------------------
// Drawing
void ofxSpriteSheetMovieClip::draw() {
    
    ofPoint& textureOffset = getTextureOffset();
    if(isCustomSize)
        getTexturePtr()->drawSubsection(position.x, position.y, sliceWidth, sliceHeight, textureOffset.x, textureOffset.y, width, height);
    else
        getTexturePtr()->drawSubsection(position.x, position.y, sliceWidth, sliceHeight, textureOffset.x, textureOffset.y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

void ofxSpriteSheetMovieClip::draw(float x, float y) {
    
    ofPoint& textureOffset = getTextureOffset();
    if(isCustomSize)
        getTexturePtr()->drawSubsection(x, y, sliceWidth, sliceHeight, textureOffset.x, textureOffset.y, width, height);
    else
        getTexturePtr()->drawSubsection(x, y, sliceWidth, sliceHeight, textureOffset.x, textureOffset.y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

void ofxSpriteSheetMovieClip::draw(float x, float y, float w, float h) {
    
	//getTexturePtr()->draw(x, y, w, h);
    ofPoint& textureOffset = getTextureOffset();
    getTexturePtr()->drawSubsection(x,y, sliceWidth, sliceHeight, textureOffset.x, textureOffset.y, w, h);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}



// Updates
//--------------------------------------------------------------
void ofxSpriteSheetMovieClip::stepForward() {
    float elapsedFrameTimeDiff = ofGetElapsedTimef()-previousFrameTimeElapsed;
    if(elapsedFrameTimeDiff >= frameDelayInSeconds) {
        
        // changing from imageFramesSize to gridPositionsSize
        playheadCount++;
        if(playheadCount >= activeAsset->gridPositionsSize) {
            if(loopOnFinish) {
                playheadCount = 0; // default behaviour
            } else if(loopCount > 1) {
                loopCount--;
                playheadCount = 0;
            } else {
                playheadCount = activeAsset->gridPositionsSize -1;
            }
            loopComplete = true;
        }
        
        previousFrameTimeElapsed = ofGetElapsedTimef();
    }
    
}


void ofxSpriteSheetMovieClip::stepReverse() {
    
    float elapsedFrameTimeDiff = ofGetElapsedTimef()-previousFrameTimeElapsed;
    if(elapsedFrameTimeDiff >= frameDelayInSeconds) {
        
        playheadCount--;
        if(playheadCount < 0) {
            if(loopOnFinish) {
                playheadCount = activeAsset->gridPositionsSize -1; // default behaviour
            } else if(loopCount > 1) {
                loopCount--;
                playheadCount = 0;
            } else {
                playheadCount = 0;
            }
            loopComplete = true;
        }
        previousFrameTimeElapsed = ofGetElapsedTimef();
    }
}




//--------------------------------------------------------------
// only 1 texture to return for spritesheet
ofTexture* ofxSpriteSheetMovieClip::getTexturePtr() {
    return activeAsset->imageFrames[0];//playheadCount];
}

ofPoint& ofxSpriteSheetMovieClip::getTextureOffset() {
    return activeAsset->gridPositions[playheadCount];
}