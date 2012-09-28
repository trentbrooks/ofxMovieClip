#include "ofxFBOMovieClip.h"

//--------------------------------------------------------------
ofxFBOMovieClip::ofxFBOMovieClip(){
	playheadCount = 0;
	reversePlayheadCount = 0;
	playMode = 1;
	frameInterval = 30.0f;
	frameLabelId = 0;
    position = ofPoint(0,0);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::init(ofxFBOImageSequenceLoader* fboIs, float frameInterval)
{
	this->fboImageSequence = fboIs;
	setFrameInterval(frameInterval);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::setFrameInterval(float frameInterval)
{
    this->frameInterval = frameInterval;
}


//--------------------------------------------------------------
void ofxFBOMovieClip::stepForward()
{
	playheadCount = (playheadCount + 1) % (int)frameInterval; 
}

//--------------------------------------------------------------
void ofxFBOMovieClip::stepReverse()
{
	reversePlayheadCount = (reversePlayheadCount + 1) % (int)frameInterval;
	playheadCount = frameInterval - reversePlayheadCount - 1;
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
	reversePlayheadCount = frameInterval - playheadCount;
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
	//layheadCount = (ofClamp(float(frameNumber-1), 0.0f,fboImageSequence->assetCollections[frameLabelId].size()) / fboImageSequence->assetCollections[frameLabelId].size()) * frameInterval;
    playheadCount = (ofClamp(float(frameNumber-1), 0.0f,fboImageSequence->assetCollections[frameLabelId]->imageFramesSize) / fboImageSequence->assetCollections[frameLabelId]->imageFramesSize) * frameInterval;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndStop(int frameNumber)
{
	playMode = STEP_STOP;
	//playheadCount = (ofClamp(float(frameNumber), 0.0f, fboImageSequence->assetCollections[frameLabelId].size()) / fboImageSequence->assetCollections[frameLabelId].size()) * frameInterval;
    playheadCount = (ofClamp(float(frameNumber), 0.0f, fboImageSequence->assetCollections[frameLabelId]->imageFramesSize) / fboImageSequence->assetCollections[frameLabelId]->imageFramesSize) * frameInterval;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndPlay(string frameLabel)
{
	playMode = STEP_FORWARD;
	frameLabelId = fboImageSequence->getAssetsId(frameLabel);
	playheadCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndStop(string frameLabel)
{
	playMode = STEP_STOP;
	frameLabelId = fboImageSequence->getAssetsId(frameLabel);
	playheadCount = 0;		
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndPlay(string frameLabel, int frameNumber)
{
	frameLabelId = fboImageSequence->getAssetsId(frameLabel);
	gotoAndPlay(frameNumber);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndStop(string frameLabel, int frameNumber)
{
	frameLabelId = fboImageSequence->getAssetsId(frameLabel);
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
	getFrame()->draw(position.x, position.y);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::drawFrame(int x, int y)
{	
	getFrame()->draw(x, y);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::drawFrame(int x, int y, float w, float h)
{	
	getFrame()->draw(x, y, w, h);
}

//--------------------------------------------------------------
ofFbo*& ofxFBOMovieClip::getFrame()
{
	tick(); // now gets called whenever a getFrame is requested instead of manually	
	return getFrameAtPercent(playheadCount / frameInterval);
}

//--------------------------------------------------------------
ofFbo*& ofxFBOMovieClip::getFrameAtPercent(float percent)
{
	//return fboImageSequence->assetCollections[frameLabelId][(int)(percent * fboImageSequence->assetCollections[frameLabelId].size())];
    return fboImageSequence->assetCollections[frameLabelId]->imageFrames[(int)(percent * fboImageSequence->assetCollections[frameLabelId]->imageFramesSize)];
}


