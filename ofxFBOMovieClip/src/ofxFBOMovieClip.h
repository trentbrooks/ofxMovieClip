
#pragma once


#include "ofxFBOImageSequenceLoader.h"
#include "ofMain.h"

#define STEP_STOP 0
#define STEP_FORWARD 1
#define STEP_REVERSE 2

class ofxFBOMovieClip
	{
	public:

		ofxFBOMovieClip();

		// initialisation
		void init(ofxFBOImageSequenceLoader* fboIs, float frameInterval);
		void setFrameInterval(float frameInterval);
        float frameInterval; // 1.0f = 1/60th of a second, 60fps, 60.0f = 60/60th, 1fps
        
		// main ingrediant
		ofxFBOImageSequenceLoader* fboImageSequence;

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
		ofFbo*& getFrameAtPercent(float percent);
		ofFbo*& getFrame();

        // timeline properties
		int frameLabelId;
		int playheadCount;
		int reversePlayheadCount;
		int playMode; // 0 = stopped, 1 = playing, 2 = reverse
};


