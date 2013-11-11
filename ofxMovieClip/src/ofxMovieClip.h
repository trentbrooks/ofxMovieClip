
#pragma once


#include "ofxImageSequenceLoader.h"
#include "ofMain.h"

//#define STEP_STOP 0
//#define STEP_FORWARD 1
//#define STEP_REVERSE 2

/*
 ofxMovieClip
 - control image sequences with gotoAndPlay like controls.
 - declare as ofxFboMovieClip, ofxTextureMovieClip, or ofxPixelMovieClip (typedefs at bottom)
 */

template<typename ImageType> // ofTexture, or ofFbo, or ofPixels
class ofxMovieClip {
	public:
        
        enum PlaybackMode { STEP_STOP, STEP_FORWARD, STEP_REVERSE };
    
    
		ofxMovieClip();

		// initialisation
		void init(ofxImageSequenceLoader<ImageType>* texIs, float frameSpeed);
        

		// main ingrediant
		ofxImageSequenceLoader<ImageType>* imageSequence;
        MovieClipAssetsAndLabel<ImageType>* activeAsset;

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

        // display position
        void setPosition(int x, int y);
        ofPoint* getPositionPtr() { return &position; };
        ofPoint getPosition() { return position; };

		// drawing
		void draw();
        void draw(float x, float y);
		void draw(float x, float y, float w, float h);
        // deprecated draw methods converted to inline- keeping here for legacy projects
        void drawFrame() { draw(); };
        void drawFrame(float x, float y) { draw(x,y); };
		void drawFrame(float x, float y, float w, float h) { draw(x,y,w,h); };
        
        ofTexture* getTexturePtr();
        ofTexture* getFramePtr() { return getTexturePtr(); }; // deprecated
        
        // useful playback setters/getters
        float* getFrameSpeedPtr() { return &frameSpeed; };
        float getFrameSpeed() { return frameSpeed; };
        void setFrameSpeed(float speed) { frameSpeed = speed; };
        PlaybackMode getPlaybackMode() { return playMode; };
        float* getPlayheadPtr() { return (playMode == STEP_FORWARD) ? &playheadCount : &reversePlayheadCount; };
        float getPlayhead() { return (playMode == STEP_FORWARD) ? playheadCount : reversePlayheadCount; };
        void setPlayhead(float frameNum) { (playMode == STEP_FORWARD) ? playheadCount = frameNum : reversePlayheadCount = frameNum; };

        
	protected:

		// update methods
		void tick();
		void stepReverse();
		void stepForward();

		// draw with texture returns
		//ofTexture*& getFrameAtPercent(float percent);
        
        // display position
        ofPoint position;

		// playback speed		     
        float frameIntervalTicker;
        float defaultFrameSpeed;
        
        // only required by ofxPixelMovieClip
		ofTexture* pixelsTexture;
        int playheadCopy;
		
        // playback
        float frameSpeed; // 1.0f = 60.0f/ofGetFrameRate() (same as apps frame rate), 0.5f = 30.0f/ofGetFrameRate() (half the apps frame rate), 2.0f = 120.0f/ofGetFrameRate() (double playback speed)
        PlaybackMode playMode; // 0 = stopped, 1 = playing, 2 = reverse
        float playheadCount;
        float reversePlayheadCount;
        int frameLabelId; // corresponds with a frameLabel   
};

// use these types = ofTexture, ofFbo, ofPixels
// instead of ofxMovieClip
typedef ofxMovieClip<ofTexture> ofxTextureMovieClip;
typedef ofxMovieClip<ofFbo> ofxFboMovieClip;
typedef ofxMovieClip<ofPixels> ofxPixelsMovieClip;



