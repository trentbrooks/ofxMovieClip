/**
 *  ofxFBOMovieClip.h
 *
 * Created by Trent Brooks, http://www.trentbrooks.com
 *
 * ----------------------
 *
 *	ABOUT
 *  ofxFBOMovieClip & ofxFBOImageSequenceLoader are a set of helper 
 *  classes for loading and controlling image sequences as FBO textures. 
 *  These were created to add simple timeline control similar to Flash  
 *  ( play(), stop(), gotoAndPlay(), etc ). ofxFBOImageSequenceLoader 
 *  loads sequenced images, and ofxFBOMovieClip requires a pointer
 *  to ofxFBOImageSequenceLoader to control the playback of the frames.
 *
 *  ofxFBOImageSequenceLoader is similar to and inspired by 
 *  FlightPhase's http://www.flightphase.com ofxImageSequence.
 *
 *
 *	TO DOS:
 *  - make ofxFBOImageSequenceLoader static
 *  - ask for help to check memory leaks and optimisations
 *
 */

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
		void init(ofxFBOImageSequenceLoader* fboIs, float fRate);
		void setFrameRate(float fr);
		
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
		
		// display
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
		ofxFBOTexture* getFrameAtPercent(float percent);
		ofxFBOTexture* getFrame();
		
		// framerates
		float frameRate;
		float userSelectFrameRate;
		
		// timeline properties
		int frameLabelId;		
		int playheadCount; 
		int reversePlayheadCount;
		int frameRateCount;
		int playMode; // 0 = stopped, 1 = playing, 2 = reverse
		
		
		
};


