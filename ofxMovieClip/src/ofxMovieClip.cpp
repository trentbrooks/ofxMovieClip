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
    activeAsset = NULL;
    pixelsTexture = NULL;
}


template<typename ImageType>
void ofxMovieClip<ImageType>::init(ofxImageSequenceLoader<ImageType>* imageSequence, float frameDelay)
{
	this->imageSequence = imageSequence;
    activeAsset = imageSequence->assetCollections[frameLabelId];
    
    this->frameDelayInSeconds = frameDelay;
    
    // auto grab the width and height of the first asset
    if(width == -1 && height == -1 && activeAsset->imageFrames.size() > 0) {
        width = activeAsset->imageFrames[0]->getWidth();
        height = activeAsset->imageFrames[0]->getWidth();
    }
}

//--------------------------------------------------------------
// template specialisation: ofPixels only
template<>
void ofxMovieClip<ofPixels>::init(ofxImageSequenceLoader<ofPixels>* imageSequence, float frameDelay)
{
    pixelsTexture = new ofTexture();
    //pixelsTexture->allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
	this->imageSequence = imageSequence;
    activeAsset = imageSequence->assetCollections[frameLabelId];
    
    this->frameDelayInSeconds = frameDelay;
    
    // auto grab the width and height of the first asset
    if(width == -1 && height == -1 && activeAsset->imageFrames.size() > 0) {
        width = activeAsset->imageFrames[0]->getWidth();
        height = activeAsset->imageFrames[0]->getWidth();
    }
}



// Play head controls
//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::play()
{
	playMode = STEP_FORWARD;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::reverse()
{
	// need to reswap reversePlayheadCount if playing
	//reversePlayheadCount = frameInterval - playheadCount;
	playMode = STEP_REVERSE;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::stop()
{
	playMode = STEP_STOP;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::restart()
{
	playMode = STEP_FORWARD;
	playheadCount = 0;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(int frameNumber)
{
	playMode = STEP_FORWARD;
	playheadCount = ofClamp(frameNumber, 0, activeAsset->imageFramesSize - 1);
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(int frameNumber)
{
	playMode = STEP_STOP;
	playheadCount = ofClamp(frameNumber, 0, activeAsset->imageFramesSize - 1);
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(string frameLabel)
{
	playMode = STEP_FORWARD;
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	playheadCount = 0;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(string frameLabel)
{
	playMode = STEP_STOP;
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	playheadCount = 0;		
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(string frameLabel, int frameNumber)
{
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	gotoAndPlay(frameNumber);
}

template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(string frameLabel, int frameNumber)
{
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	gotoAndStop(frameNumber);
}



//--------------------------------------------------------------
// Drawing
template<typename ImageType>
void ofxMovieClip<ImageType>::draw(){
    
    if(isCustomSize)
        getTexturePtr()->draw(position.x, position.y, width, height);
    else
        getTexturePtr()->draw(position.x, position.y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

template<typename ImageType>
void ofxMovieClip<ImageType>::draw(float x, float y){
    
    if(isCustomSize)
        getTexturePtr()->draw(x, y, width, height);
    else
        getTexturePtr()->draw(x, y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

template<typename ImageType>
void ofxMovieClip<ImageType>::draw(float x, float y, float w, float h){
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
// template specialisation: ofTexture
template<>
ofTexture* ofxMovieClip<ofTexture>::getTexturePtr() {
    return activeAsset->imageFrames[playheadCount];
}

// template specialisation: ofFbo
template<>
ofTexture* ofxMovieClip<ofFbo>::getTexturePtr() {
    return &activeAsset->imageFrames[playheadCount]->getTextureReference();
}

// template specialisation: ofPixels
template<>
ofTexture* ofxMovieClip<ofPixels>::getTexturePtr() {
    
    // only allocate + load if the playhead has changed, otherwise return cached copy
    if(playheadCopy != playheadCount) {
        //pixelsTexture->clear();
        ofPixels* px = activeAsset->imageFrames[playheadCount];
        if(!pixelsTexture->isAllocated()) pixelsTexture->allocate(*px); //->getWidth(), px->getHeight(), GL_RGB);        
        pixelsTexture->loadData(*px);
        playheadCopy = playheadCount;
    }
    
    
    return pixelsTexture;
}


template class ofxMovieClip<ofTexture>;
template class ofxMovieClip<ofFbo>;
template class ofxMovieClip<ofPixels>;
