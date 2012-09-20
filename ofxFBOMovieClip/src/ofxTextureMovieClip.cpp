#include "ofxTextureMovieClip.h"

//--------------------------------------------------------------
ofxTextureMovieClip::ofxTextureMovieClip(){
	playheadCount = 0;
	reversePlayheadCount = 0;
	playMode = 1;
	frameInterval = 30.0f;
	frameLabelId = 0;
    position = ofPoint(0,0);
}

void ofxTextureMovieClip::init(ofxTextureImageSequenceLoader* texIs, float frameInterval)
{
	this->textureImageSequence = texIs;
	setFrameInterval(frameInterval);
}

//--------------------------------------------------------------
void ofxTextureMovieClip::setFrameInterval(float frameInterval)
{
    this->frameInterval = frameInterval;
}



//--------------------------------------------------------------
void ofxTextureMovieClip::stepForward()
{
	playheadCount = (playheadCount + 1) % (int)frameInterval; 
}

//--------------------------------------------------------------
void ofxTextureMovieClip::stepReverse()
{
	reversePlayheadCount = (reversePlayheadCount + 1) % (int)frameInterval;
	playheadCount = frameInterval - reversePlayheadCount - 1;
}



//--------------------------------------------------------------
void ofxTextureMovieClip::play()
{
	playMode = STEP_FORWARD;
}

//--------------------------------------------------------------
void ofxTextureMovieClip::reverse()
{
	// need to reswap reversePlayheadCount if playing
	reversePlayheadCount = frameInterval - playheadCount;
	playMode = STEP_REVERSE;
}

//--------------------------------------------------------------
void ofxTextureMovieClip::stop()
{
	playMode = STEP_STOP;
}

//--------------------------------------------------------------
void ofxTextureMovieClip::restart()
{
	playMode = STEP_FORWARD;
	playheadCount = 0;
}

//--------------------------------------------------------------
void ofxTextureMovieClip::gotoAndPlay(int frameNumber)
{
	playMode = STEP_FORWARD;
	playheadCount = (ofClamp(float(frameNumber-1), 0.0f,textureImageSequence->assetCollections[frameLabelId].size()) / textureImageSequence->assetCollections[frameLabelId].size()) * frameInterval;
}

//--------------------------------------------------------------
void ofxTextureMovieClip::gotoAndStop(int frameNumber)
{
	playMode = STEP_STOP;
	playheadCount = (ofClamp(float(frameNumber), 0.0f, textureImageSequence->assetCollections[frameLabelId].size()) / textureImageSequence->assetCollections[frameLabelId].size()) * frameInterval;
}

//--------------------------------------------------------------
void ofxTextureMovieClip::gotoAndPlay(string frameLabel)
{
	playMode = STEP_FORWARD;
	frameLabelId = textureImageSequence->getAssetsId(frameLabel);
	playheadCount = 0;
}

//--------------------------------------------------------------
void ofxTextureMovieClip::gotoAndStop(string frameLabel)
{
	playMode = STEP_STOP;
	frameLabelId = textureImageSequence->getAssetsId(frameLabel);
	playheadCount = 0;		
}

//--------------------------------------------------------------
void ofxTextureMovieClip::gotoAndPlay(string frameLabel, int frameNumber)
{
	frameLabelId = textureImageSequence->getAssetsId(frameLabel);
	gotoAndPlay(frameNumber);
}

//--------------------------------------------------------------
void ofxTextureMovieClip::gotoAndStop(string frameLabel, int frameNumber)
{
	frameLabelId = textureImageSequence->getAssetsId(frameLabel);
	gotoAndStop(frameNumber);
}


//--------------------------------------------------------------
void ofxTextureMovieClip::tick()
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
void ofxTextureMovieClip::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}

//--------------------------------------------------------------
void ofxTextureMovieClip::setPosition(ofPoint pos)
{
    position = pos;
}


//--------------------------------------------------------------
void ofxTextureMovieClip::drawFrame()
{	
	getFrame()->draw(position.x, position.y);
}

//--------------------------------------------------------------
void ofxTextureMovieClip::drawFrame(float x, float y)
{	
	getFrame()->draw(x, y);
}

//--------------------------------------------------------------
void ofxTextureMovieClip::drawFrame(float x, float y, float w, float h)
{	
	getFrame()->draw(x, y, w, h);
}

//--------------------------------------------------------------
ofTexture*& ofxTextureMovieClip::getFrame()
{
	tick(); // now gets called whenever a getFrame is requested instead of manually	
	return getFrameAtPercent(playheadCount / frameInterval);
}

//--------------------------------------------------------------
ofTexture*& ofxTextureMovieClip::getFrameAtPercent(float percent)
{
    //cout << (int)(percent * textureImageSequence->assetCollections[frameLabelId].size()) << endl;
	return textureImageSequence->assetCollections[frameLabelId][(int)(percent * textureImageSequence->assetCollections[frameLabelId].size())];
}


//--------------------------------------------------------------
void ofxTextureMovieClip::dispose()
{
	delete this;
}
