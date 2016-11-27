#pragma once


#include "ofMain.h"
#include "ofxSpriteSheetLoader.h"
#include "ofxMovieClip.h"

/*
 ofxSpriteSheetMovieClip
 - each spritesheet should be a single full animation (1 image)
 - using the playheadCount to control the grid position of a single image instead of frame number of multiple
 */


class ofxSpriteSheetMovieClip : public ofxMovieClip<ofTexture> {
	public:
    
		ofxSpriteSheetMovieClip();

		// initialisation
		void init(ofxSpriteSheetLoader* spriteSheetLoader, float frameDelay);

		// main ingrediant
		ofxSpriteSheetLoader* spriteSheet;
        SpriteSheetData* activeAsset;

		// drawing
		void draw();
        void draw(float x, float y);
		void draw(float x, float y, float w, float h);

        // texture
        ofTexture* getTexturePtr();

	protected:

		// update methods
		void stepReverse();
		void stepForward();
    
        // spritesheet
        float sliceWidth, sliceHeight;
        ofPoint& getTextureOffset();
    
    
};




