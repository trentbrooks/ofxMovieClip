
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
		void init(ofxTextureImageSequenceLoader* texIs, float fRate);
		void setFrameRate(float fr);

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
        void drawFrame(int x, int y);
		void drawFrame(int x, int y, float w, float h);

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

		// framerates
		float frameRate;
		float userSelectFrameRate;

		// timeline properties
		int frameLabelId;
		int playheadCount;
		int reversePlayheadCount;
		int playMode; // 0 = stopped, 1 = playing, 2 = reverse
};


