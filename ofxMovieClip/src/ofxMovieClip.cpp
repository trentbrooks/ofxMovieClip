#include "ofxMovieClip.h"

//--------------------------------------------------------------
template<typename ImageType>
ofxMovieClip<ImageType>::ofxMovieClip(){
	playheadCount = 0;
	reversePlayheadCount = 0;
	playMode = 1;
    frameIntervalTicker = 0.0f; // cumulative
    frameSpeed = defaultFrameSpeed = 1.0f;
	frameLabelId = 0;
    position = ofPoint(0,0);
    activeAsset = NULL;
}

template<typename ImageType>
void ofxMovieClip<ImageType>::init(ofxImageSequenceLoader<ImageType>* imageSequence, float frameSpeed)
{
	this->imageSequence = imageSequence;
    activeAsset = imageSequence->assetCollections[frameLabelId];
    
    this->frameSpeed = frameSpeed;
}

//--------------------------------------------------------------
// template specialisation: ofPixels only
template<>
void ofxMovieClip<ofPixels>::init(ofxImageSequenceLoader<ofPixels>* imageSequence, float frameSpeed)
{
    pixelsTexture = new ofTexture();
    pixelsTexture->allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
	this->imageSequence = imageSequence;
    activeAsset = imageSequence->assetCollections[frameLabelId];
    
    this->frameSpeed = frameSpeed;
}


//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::stepForward()
{
    // skipping frames to fast forward
    if(frameSpeed >= defaultFrameSpeed) {
        if( (playheadCount += frameSpeed) >= activeAsset->imageFramesSize ) playheadCount = 0;
    }
    
    // slower frame speed requires a ticker
    else if( (frameIntervalTicker += frameSpeed ) >= defaultFrameSpeed) {
        frameIntervalTicker = 0;
        if( (playheadCount += defaultFrameSpeed) >= activeAsset->imageFramesSize ) playheadCount = 0;
    }
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::stepReverse()
{
    
    // skipping frames to fast forward
    if(frameSpeed >= defaultFrameSpeed) {
        if( (playheadCount -= frameSpeed) < 0) playheadCount = activeAsset->imageFramesSize - 1;
    }
    
    // slower frame speed requires a ticker
    else if( (frameIntervalTicker += frameSpeed ) >= defaultFrameSpeed) {
        frameIntervalTicker = 0;
        if( (playheadCount -= defaultFrameSpeed) < 0 ) playheadCount = activeAsset->imageFramesSize - 1;
    }
}



//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::play()
{
	playMode = STEP_FORWARD;
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::reverse()
{
	// need to reswap reversePlayheadCount if playing
	//reversePlayheadCount = frameInterval - playheadCount;
	playMode = STEP_REVERSE;
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::stop()
{
	playMode = STEP_STOP;
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::restart()
{
	playMode = STEP_FORWARD;
	playheadCount = 0;
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(int frameNumber)
{
	playMode = STEP_FORWARD;
	playheadCount = ofClamp(frameNumber, 0, activeAsset->imageFramesSize - 1);
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(int frameNumber)
{
	playMode = STEP_STOP;
	playheadCount = ofClamp(frameNumber, 0, activeAsset->imageFramesSize - 1);
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(string frameLabel)
{
	playMode = STEP_FORWARD;
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	playheadCount = 0;
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(string frameLabel)
{
	playMode = STEP_STOP;
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	playheadCount = 0;		
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndPlay(string frameLabel, int frameNumber)
{
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	gotoAndPlay(frameNumber);
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::gotoAndStop(string frameLabel, int frameNumber)
{
	frameLabelId = imageSequence->getAssetsId(frameLabel);
    activeAsset = imageSequence->assetCollections[frameLabelId];
	gotoAndStop(frameNumber);
}


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

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::setPosition(ofPoint pos)
{
    position = pos;
}


//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::drawFrame(){
	getFramePtr()->draw(position.x, position.y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::drawFrame(float x, float y){	
	getFramePtr()->draw(x, y);
	tick(); // now gets called whenever a drawFrame is requested instead of manually
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxMovieClip<ImageType>::drawFrame(float x, float y, float w, float h){	
	getFramePtr()->draw(x, y, w, h);
	tick(); // now gets called whenever a drawFrame is requested instead of manually	
}


//--------------------------------------------------------------
// template specialisation: ofTexture
template<>
ofTexture* ofxMovieClip<ofTexture>::getFramePtr() {
    return activeAsset->imageFrames[playheadCount];
}

// template specialisation: ofFbo
template<>
ofTexture* ofxMovieClip<ofFbo>::getFramePtr() {
    return &activeAsset->imageFrames[playheadCount]->getTextureReference();
}

// template specialisation: ofPixels
template<>
ofTexture* ofxMovieClip<ofPixels>::getFramePtr() {
    pixelsTexture->clear();
    ofPixels* px = activeAsset->imageFrames[playheadCount];
    pixelsTexture->allocate(px->getWidth(), px->getHeight(), GL_RGBA);
    pixelsTexture->loadData(*px);
    return pixelsTexture;
}


template class ofxMovieClip<ofTexture>;
template class ofxMovieClip<ofFbo>;
template class ofxMovieClip<ofPixels>;
