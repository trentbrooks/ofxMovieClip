#pragma once


#include "ofMain.h"
#include "ofxMovieClip.h"
#include "ofxBufferedSequenceLoaderThread.h"

/*
 ofxBufferedMovieClip
 - to keep memory footprint low for large image sequences
 - works like a video streamer
 - signals ofxPixelsSequenceLoaderThread to load a buffer of images (eg. 30 at a time)
 - as the playhead moves, old images are deleted
 */


class ofxBufferedMovieClip : public ofxMovieClip<ofPixels> {
	public:
    
		ofxBufferedMovieClip();

        void init(ofxBufferedSequenceLoaderThread* loader, float frameDelay);
    
        ofxBufferedSequenceLoaderThread* imageSequenceThread;
    
        // drawing
        void draw();
        void draw(float x, float y);
        void draw(float x, float y, float w, float h);
    
        virtual ofTexture* getTexturePtr();
        bool checkBuffersReady();

	protected:

        void tick();
    
    
};




