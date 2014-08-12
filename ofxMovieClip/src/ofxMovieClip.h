#pragma once


#include "ofMain.h"
#include "ofxPixelsSequenceLoaderThread.h"
#include "ofxImageSequenceLoader.h"

/*
 ofxMovieClip
 - control image sequences with gotoAndPlay like controls.
 - declare as ofxTextureMovieClip, or ofxPixelMovieClip (typedefs at bottom)
 */

template<typename ImageType> // ofTexture, or ofPixels
class ofxMovieClip {
	public:
        
        enum PlaybackMode { STEP_STOP, STEP_FORWARD, STEP_REVERSE };
    
		ofxMovieClip();

		// initialisation
		void init(ofxImageSequenceLoader<ImageType>* texIs, float frameDelay);
        

		// main ingrediant
		ofxImageSequenceLoader<ImageType>* imageSequence;
        MovieClipData<ImageType>* activeAsset;

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
        void setPosition(float x, float y);
        ofPoint* getPositionPtr() { return &position; };
        ofPoint getPosition() { return position; };
        void setSize(float w, float h);
        float getWidth() { return width; };
        float getHeight() { return height; };

		// drawing
		void draw();
        void draw(float x, float y);
		void draw(float x, float y, float w, float h);

        // texture
        ofTexture* getTexturePtr();
    
        // looping options
        bool isLastFrame() { return (playheadCount == activeAsset->imageFramesSize-1); };
        void setLooping(bool bLoop) { loopOnFinish = bLoop; };
        void setLoopCount(int bLoopCount) { loopCount = bLoopCount; };
    
        // playback setters/getters
        float* getFrameDelayPtr() { return &frameDelayInSeconds; };
        float getFrameDelay() { return frameDelayInSeconds; };
        void setFrameDelay(float frameDelay) { frameDelayInSeconds = frameDelay; };
    
        int* getPlayheadPtr() { return &playheadCount; };
        int getPlayhead() { return playheadCount; };
        void setPlayhead(float frameNum) { playheadCount = frameNum ; };

        PlaybackMode getPlaybackMode() { return playMode; };
    
    
	protected:

		// update methods
		void tick();
		void stepReverse();
		void stepForward();
    
        // display position
        ofPoint position;
        float width, height;
        bool isCustomSize;
    
        // only required by ofxPixelMovieClip
		ofTexture* pixelsTexture;
        int playheadCopy;
		
        // playback
        float frameDelayInSeconds;
        float previousFrameTimeElapsed;
    
        PlaybackMode playMode; // 0 = stopped, 1 = playing, 2 = reverse
        int playheadCount;
        int frameLabelId; // corresponds with a frameLabel
    
        // looping
        bool loopOnFinish;
        int loopCount;
};

// use these types = ofTexture, ofPixels
// instead of ofxMovieClip
typedef ofxMovieClip<ofTexture> ofxTextureMovieClip;
typedef ofxMovieClip<ofPixels> ofxPixelsMovieClip;



