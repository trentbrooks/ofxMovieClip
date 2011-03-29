#include "ofxFBOMovieClip.h"

//--------------------------------------------------------------
ofxFBOMovieClip::ofxFBOMovieClip(){
	playheadCount = 0;
	reversePlayheadCount = 0;
	playMode = 1;
	frameRate = 30.0f;
	frameRateCount = 0;
	frameLabelId = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::init(ofxFBOImageSequenceLoader* fboIs, float fRate)
{
	this->fboImageSequence = fboIs;
	setFrameRate(fRate);
}

//--------------------------------------------------------------
void ofxFBOMovieClip::setFrameRate(float fr)
{
	userSelectFrameRate = fr;
	//frameRate = (ofGetFrameRate() / userSelectFrameRate) * ofGetFrameRate();
	
	// framerate cannot be higher that app or lower than 1.
	frameRate = ofClamp(ofGetFrameRate() - userSelectFrameRate, 1.0f, ofGetFrameRate()); 
}


//--------------------------------------------------------------
void ofxFBOMovieClip::stepForward()
{
	
	//playheadCount = (playheadCount + 1) % (int)frameRate; 
	
	frameRateCount = (frameRateCount + 1) % (int)frameRate;
	if(frameRateCount == (int)frameRate - 1)
	{
		playheadCount = (playheadCount + 1) % (int)fboImageSequence->assetCollections[frameLabelId].size();
	}
	
	


}

//--------------------------------------------------------------
void ofxFBOMovieClip::stepReverse()
{
	//reversePlayheadCount = (reversePlayheadCount + 1) % (int)frameRate;
	//playheadCount = frameRate - reversePlayheadCount - 1;
	
	frameRateCount = (frameRateCount + 1) % (int)frameRate;
	if(frameRateCount == (int)frameRate - 1)
	{
		reversePlayheadCount = (reversePlayheadCount + 1) % (int)fboImageSequence->assetCollections[frameLabelId].size();
		playheadCount = (int)fboImageSequence->assetCollections[frameLabelId].size() - reversePlayheadCount - 1;
	}
}



//--------------------------------------------------------------
void ofxFBOMovieClip::play()
{
	playMode = STEP_FORWARD;
	frameRateCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::reverse()
{
	// need to reswap reversePlayheadCount if playing
	//reversePlayheadCount = frameRate - playheadCount;
	reversePlayheadCount = (int)fboImageSequence->assetCollections[frameLabelId].size() - playheadCount;
	playMode = STEP_REVERSE;
	frameRateCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::stop()
{
	playMode = STEP_STOP;
	frameRateCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::restart()
{
	playMode = STEP_FORWARD;
	playheadCount = 0;
	frameRateCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndPlay(int frameNumber)
{
	playMode = STEP_FORWARD;
	playheadCount = (ofClamp(float(frameNumber-1), 0.0f,fboImageSequence->assetCollections[frameLabelId].size()) / fboImageSequence->assetCollections[frameLabelId].size()) * frameRate;
	frameRateCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndStop(int frameNumber)
{
	playMode = STEP_STOP;
	playheadCount = (ofClamp(float(frameNumber), 0.0f, fboImageSequence->assetCollections[frameLabelId].size()) / fboImageSequence->assetCollections[frameLabelId].size()) * frameRate;
	frameRateCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndPlay(string frameLabel)
{
	playMode = STEP_FORWARD;
	frameLabelId = fboImageSequence->getAssetsId(frameLabel);
	playheadCount = 0;
	frameRateCount = 0;
}

//--------------------------------------------------------------
void ofxFBOMovieClip::gotoAndStop(string frameLabel)
{
	playMode = STEP_STOP;
	frameLabelId = fboImageSequence->getAssetsId(frameLabel);
	playheadCount = 0;		
	frameRateCount = 0;
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
ofxFBOTexture* ofxFBOMovieClip::getFrame()
{
	tick(); // now gets called whenever a getFrame is requested instead of manually	
	//return getFrameAtPercent(playheadCount / frameRate); // what if playhead is way more than frame rate dick?
	return getFrameAtPercent(playheadCount / float(fboImageSequence->assetCollections[frameLabelId].size()));
}

//--------------------------------------------------------------
ofxFBOTexture* ofxFBOMovieClip::getFrameAtPercent(float percent)
{
	//printf("mc percent: %f ", percent); //, playheadCount, float(fboImageSequence->assetCollections[frameLabelId].size()) );
	//printf("mc playhead: %i \n", playheadCount);
	return fboImageSequence->assetCollections[frameLabelId][(int)(percent * fboImageSequence->assetCollections[frameLabelId].size())];
}


//--------------------------------------------------------------
void ofxFBOMovieClip::dispose()
{
	delete this;
}
