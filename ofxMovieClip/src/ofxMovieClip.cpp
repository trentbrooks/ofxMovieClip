#include "ofxMovieClip.h"


//--------------------------------------------------------------
template<typename ImageType>
ofxMovieClip<ImageType>::ofxMovieClip(){
	playheadCount = 0;
	playMode = STEP_FORWARD;
    previousFrameTimeElapsed = 0;
    frameDelayInSeconds = 1.0 / 30.0; // default around 30fps
	frameLabelId = 0;
    width= height =-1;
    isCustomSize = false;
    playheadCopy = -1;
    loopCount = 0;
    loopOnFinish = true;
    loopComplete = false;
    activeAsset = NULL;
    pixelsTexture = NULL;
}


//--------------------------------------------------------------
// template specialisation: ofTexture
template<>
void ofxMovieClip<ofTexture>::init(ofxImageSequenceLoader<ofTexture>* imageSequence, float frameDelay)
{
	this->imageSequence = imageSequence;
    activeAsset = imageSequence->assetCollections[frameLabelId];
    
    this->frameDelayInSeconds = frameDelay;
    
    // auto grab the width and height of the first asset
    if(width == -1 && height == -1 && activeAsset->imageFrames.size() > 0) {
        width = activeAsset->imageFrames[0]->getWidth();
        height = activeAsset->imageFrames[0]->getHeight();
    }
}


// template specialisation: ofPixels
template<>
void ofxMovieClip<ofPixels>::init(ofxImageSequenceLoader<ofPixels>* imageSequence, float frameDelay)
{
    pixelsTexture = new ofTexture(); // create a texture container for the ofPixels movieclip
    //pixelsTexture->allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
	this->imageSequence = imageSequence;
    activeAsset = imageSequence->assetCollections[frameLabelId];
    
    this->frameDelayInSeconds = frameDelay;
    
    // auto grab the width and height of the first asset
    if(width == -1 && height == -1 && activeAsset->imageFrames.size() > 0) {
        width = activeAsset->imageFrames[0]->getWidth();
        height = activeAsset->imageFrames[0]->getHeight();
    }
}



// Play head controls
//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::play() {
    loopComplete = false;
	playMode = STEP_FORWARD;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::reverse() {
    loopComplete = false;
	// need to reswap reversePlayheadCount if playing
	//reversePlayheadCount = frameInterval - playheadCount;
	playMode = STEP_REVERSE;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::stop() {
    loopComplete = false;
	playMode = STEP_STOP;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::restart() {
    loopComplete = false;
	playMode = STEP_FORWARD;
	playheadCount = 0;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(int frameNumber) {
    loopComplete = false;
	playMode = STEP_FORWARD;
	playheadCount = ofClamp(frameNumber, 0, activeAsset->imageFramesSize - 1);
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(int frameNumber) {
    loopComplete = false;
	playMode = STEP_STOP;
	playheadCount = ofClamp(frameNumber, 0, activeAsset->imageFramesSize - 1);
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(string frameLabel) {
    loopComplete = false;
	playMode = STEP_FORWARD;
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	playheadCount = 0;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(string frameLabel) {
    loopComplete = false;
	playMode = STEP_STOP;
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	playheadCount = 0;		
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(string frameLabel, int frameNumber) {
    loopComplete = false;
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	gotoAndPlay(frameNumber);
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(string frameLabel, int frameNumber) {
    loopComplete = false;
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	gotoAndStop(frameNumber);
}



//--------------------------------------------------------------
// Drawing
template<typename ImageType>
void ofxMovieClip<ImageType>::draw() { 
    
    if(isCustomSize)
        getTexturePtr()->draw(position.x, position.y, width, height);
    else
        getTexturePtr()->draw(position.x, position.y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

template<typename ImageType>
void ofxMovieClip<ImageType>::draw(float x, float y) {
    
    if(isCustomSize)
        getTexturePtr()->draw(x, y, width, height);
    else
        getTexturePtr()->draw(x, y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

template<typename ImageType>
void ofxMovieClip<ImageType>::draw(float x, float y, float w, float h) {
    
	getTexturePtr()->draw(x, y, w, h);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}



// Updates
//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::tick()
{
	switch (playMode) 
	{
		case STEP_FORWARD:
			stepForward();
			break;
		case STEP_REVERSE:
			stepReverse();
			break;
		case STEP_STOP:
			break;
		default:
			break;
	}
}

template<typename ImageType>
void ofxMovieClip<ImageType>::stepForward() {
    float elapsedFrameTimeDiff = ofGetElapsedTimef()-previousFrameTimeElapsed;
    if(elapsedFrameTimeDiff >= frameDelayInSeconds) {
        
        playheadCount++;
        if(playheadCount >= activeAsset->imageFramesSize ) {
            if(loopOnFinish) {
                playheadCount = 0; // default behaviour
            } else if(loopCount > 1) {
                loopCount--;
                playheadCount = 0;
            } else {
                playheadCount = activeAsset->imageFramesSize -1;
            }
            loopComplete = true;
        }
        previousFrameTimeElapsed = ofGetElapsedTimef();
    }
    
}

template<typename ImageType>
void ofxMovieClip<ImageType>::stepReverse() {
    
    float elapsedFrameTimeDiff = ofGetElapsedTimef()-previousFrameTimeElapsed;
    if(elapsedFrameTimeDiff >= frameDelayInSeconds) {
        
        playheadCount--;
        if(playheadCount < 0) {
            if(loopOnFinish) {
                playheadCount = activeAsset->imageFramesSize -1; // default behaviour
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


// Display position
//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::setSize(float w, float h) {
    
    width = w;
    height = h;
    isCustomSize = true;
}

//--------------------------------------------------------------
// only clears the internal pixels texture
template<typename ImageType>
void ofxMovieClip<ImageType>::clearTexture() {
    if(pixelsTexture) pixelsTexture->clear();
}

//--------------------------------------------------------------
// template specialisation: ofTexture
template<>
ofTexture* ofxMovieClip<ofTexture>::getTexturePtr() {
    return activeAsset->imageFrames[playheadCount];
}

// template specialisation: ofPixels
template<>
ofTexture* ofxMovieClip<ofPixels>::getTexturePtr() {
    
    // only allocate + load if the playhead has changed, otherwise return cached copy
    if(playheadCopy != playheadCount) {

        ofPixels* px = activeAsset->imageFrames[playheadCount];
        playheadCopy = playheadCount;
        if(!px->isAllocated()) {
            
            // thread has not finished loading image yet - will not draw
            // should probably also hold the playhead until this has occured? atm, the playhead can be too fast for the loader
			//ofLog() << "1. Wrong px here: " << playheadCount << ", " << playheadCopy;
			return pixelsTexture;
        }
        if(!pixelsTexture->isAllocated()) pixelsTexture->allocate(*px);
        pixelsTexture->loadData(*px);
    }
    
	// always stops here unless the playhead is moving
	// try to allocate from loading assets (thread is prob still loading)
	if (!pixelsTexture->isAllocated()) {

		ofPixels* px = activeAsset->imageFrames[playheadCount];
		if (px->isAllocated()) {
			pixelsTexture->allocate(*px);
			pixelsTexture->loadData(*px);
		}
		
	}
    return pixelsTexture;
}



template class ofxMovieClip<ofTexture>;
template class ofxMovieClip<ofPixels>;
