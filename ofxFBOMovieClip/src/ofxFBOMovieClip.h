/**
 *  ofxFBOMovieClip.h
 *
 * Created by Trent Brooks, http://www.trentbrooks.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 *	ABOUT
 *  ofxFBOMovieClip & ofxFBOImageSequenceLoader are a set of helper 
 *  classes for loading and controlling image sequences as FBO textures. 
 *  These were created to add simple timeline control similar to Flash.  
 *  ( play(), stop(), gotoAndPlay(), etc ). ofxFBOImageSequenceLoader 
 *  loads sequenced images, and ofxFBOMovieClip requires a pointer
 *  to ofxFBOImageSequenceLoader to control the playback of the frames.
 * 
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
		
		// framerates
		float frameRate;
		float userSelectFrameRate;
		
		// timeline properties
		int frameLabelId;		
		int playheadCount; 
		int reversePlayheadCount;
		int playMode; // 0 = stopped, 1 = playing, 2 = reverse
};


