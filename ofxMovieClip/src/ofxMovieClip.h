
#pragma once


#include "ofxImageSequenceLoader.h"
#include "ofMain.h"

#define STEP_STOP 0
#define STEP_FORWARD 1
#define STEP_REVERSE 2

/*
 ofxMovieClip
 - control image sequences with gotoAndPlay like controls.
 - declare as ofxFboMovieClip, ofxTextureMovieClip, or ofxPixelMovieClip (typedefs at bottom)
 */

template<typename ImageType> // ofTexture, or ofFbo, or ofPixels
class ofxMovieClip
	{
	public:
        
		ofxMovieClip();

		// initialisation
		void init(ofxImageSequenceLoader<ImageType>* texIs, float frameSpeed);
        float frameSpeed; // 1.0f = 60.0f/ofGetFrameRate() (same as apps frame rate), 0.5f = 30.0f/ofGetFrameRate() (half the apps frame rate), 2.0f = 120.0f/ofGetFrameRate() (double playback speed)

		// main ingrediant
		ofxImageSequenceLoader<ImageType>* imageSequence;

		// timeline controls
		void play();
		void reverse();
		void stop();
		void restart();
		void gotoAndPlay(int frameNumber);
		void gotoAndStop(int frameNumber);
		void gotoAndPlay(string frameLabel);
		void gotoAndStop(string frameLabel);
		void gotoAndPlay(string frameLabel, int frameNumber);
		void gotoAndStop(string frameLabel, int frameNumber);

        void setPosition(int x, int y);
        void setPosition(ofPoint pos);
        ofPoint position;

		// display
		void drawFrame();
        void drawFrame(float x, float y);
		void drawFrame(float x, float y, float w, float h);

        ofTexture* getFramePtr();
        
        MovieClipAssetsAndLabel<ImageType>* activeAsset;
        
        float playheadCount;
        float reversePlayheadCount;
		int playMode; // 0 = stopped, 1 = playing, 2 = reverse
        
	protected:

		// update methods
		void tick();
		void stepReverse();
		void stepForward();

		// draw with texture returns
		//ofTexture*& getFrameAtPercent(float percent);

		// timeline properties
		int frameLabelId; // corresponds with a frameLabel        
        float frameIntervalTicker;
        float defaultFrameSpeed;
        
        // only required by ofxPixelMovieClip
		ofTexture* pixelsTexture;
        int playheadCopy;
		
};

// use these types = ofTexture, ofFbo, ofPixels
// instead of ofxMovieClip
typedef ofxMovieClip<ofTexture> ofxTextureMovieClip;
typedef ofxMovieClip<ofFbo> ofxFboMovieClip;
typedef ofxMovieClip<ofPixels> ofxPixelsMovieClip;
