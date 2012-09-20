
#pragma once


#include "ofxTextureImageSequenceLoader.h"
#include "ofMain.h"

#define STEP_STOP 0
#define STEP_FORWARD 1
#define STEP_REVERSE 2

class ofxTextureMovieClip
	{
	public:

		ofxTextureMovieClip();

		// initialisation
		void init(ofxTextureImageSequenceLoader* texIs, float frameInterval);
		void setFrameInterval(float frameInterval);
        float frameInterval; // 1.0f = 1/60th of a second, 60fps, 60.0f = 60/60th, 1fps

		// main ingrediant
		ofxTextureImageSequenceLoader* textureImageSequence;

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

		// cleanup
		void dispose();
        
	protected:

		// update methods
		void tick();
		void stepReverse();
		void stepForward();

		// draw with texture returns
		ofTexture*& getFrameAtPercent(float percent);
		ofTexture*& getFrame();

		// timeline properties
		int frameLabelId;
		int playheadCount;
		int reversePlayheadCount;
		int playMode; // 0 = stopped, 1 = playing, 2 = reverse
};


