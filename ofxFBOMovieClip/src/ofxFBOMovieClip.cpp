#include "ofxFBOMovieClip.h"

//--------------------------------------------------------------
ofxFBOMovieClip::ofxFBOMovieClip(){
	playheadCount = 0;
	reversePlayheadCount = 0;
	playMode = 1;
    frameIntervalTicker = 0.0f; // cumulative
    frameSpeed = defaultFrameSpeed = 1.0f;
	frameLabelId = 0;
    position = ofPoint(0,0);
    activeAsset = NULL;
}

void ofxFBOMovieClip::init(ofxFBOImageSequenceLoader* texIs, float frameSpeed)
{
	this->textureImageSequence = texIs;
    activeAsset = textureImageSequence->assetCollections[frameLabelId];
    
    this->frameSpeed = frameSpeed;
}




//--------------------------------------------------------------
void ofxFBOMovieClip::stepForward()
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
void ofxFBOMovieClip::stepReverse()
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
void ofxFBOMovieClip::play()
{
	playMode = STEP_FORWARD;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::reverse()
{
	// need to reswap reversePlayheadCount if playing
	//reversePlayheadCount = frameInterval - playheadCount;
	playMode = STEP_REVERSE;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::stop()
{
	playMode = STEP_STOP;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::restart()
{
	playMode = STEP_FORWARD;
	playheadCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndPlay(int frameNumber)
{
	playMode = STEP_FORWARD;
    playheadCount = ofClamp(frameNumber, 0, activeAsset->imageFramesSize - 1);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndStop(int frameNumber)
{
	playMode = STEP_STOP;
    playheadCount = ofClamp(frameNumber, 0, activeAsset->imageFramesSize - 1);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndPlay(string frameLabel)
{
	playMode = STEP_FORWARD;
	frameLabelId = textureImageSequence->getAssetsId(frameLabel);
    activeAsset = textureImageSequence->assetCollections[frameLabelId];
	playheadCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndStop(string frameLabel)
{
	playMode = STEP_STOP;
	frameLabelId = textureImageSequence->getAssetsId(frameLabel);
    activeAsset = textureImageSequence->assetCollections[frameLabelId];
	playheadCount = 0;		
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndPlay(string frameLabel, int frameNumber)
{
	frameLabelId = textureImageSequence->getAssetsId(frameLabel);
    activeAsset = textureImageSequence->assetCollections[frameLabelId];
	gotoAndPlay(frameNumber);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndStop(string frameLabel, int frameNumber)
{
	frameLabelId = textureImageSequence->getAssetsId(frameLabel);
    activeAsset = textureImageSequence->assetCollections[frameLabelId];
	gotoAndStop(frameNumber);
}


//--------------------------------------------------------------
void ofxFBOMovieClip::tick()
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
void ofxFBOMovieClip::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::setPosition(ofPoint pos)
{
    position = pos;
}


//--------------------------------------------------------------
void ofxFBOMovieClip::drawFrame()
{	
	tick(); // now gets called whenever a drawFrame is requested instead of manually	
	getFrame()->draw(position.x, position.y);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::drawFrame(float x, float y)
{	
	tick(); // now gets called whenever a drawFrame is requested instead of manually	
	getFrame()->draw(x, y);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::drawFrame(float x, float y, float w, float h)
{	
	tick(); // now gets called whenever a drawFrame is requested instead of manually	
	getFrame()->draw(x, y, w, h);
}

//--------------------------------------------------------------
ofFbo*& ofxFBOMovieClip::getFrame()
{
	//return getFrameAtPercent(playheadCount / frameInterval);
    //cout << frameLabelId << "  :  " << playheadCount << endl;
    return textureImageSequence->assetCollections[frameLabelId]->imageFrames[playheadCount];
}

//--------------------------------------------------------------
/*ofTexture*& ofxFBOMovieClip::getFrameAtPercent(float percent)
 {
 //cout << (int)(percent * textureImageSequence->assetCollections[frameLabelId].size()) << endl;
 //return textureImageSequence->assetCollections[frameLabelId][(int)(percent * textureImageSequence->assetCollections[frameLabelId].size())]
 //return fboImageSequence->assetCollections[frameLabelId][(int)(percent * fboImageSequence->assetCollections[frameLabelId].size())];
 return textureImageSequence->assetCollections[frameLabelId]->imageFrames[(int)(percent * textureImageSequence->assetCollections[frameLabelId]->imageFramesSize)];
 }*/


