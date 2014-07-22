#pragma once

#include "ofMain.h"
#include "ofxMovieClip.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        // movieclip from textures
        ofxTextureImageSequenceLoader imageSequence;
        ofxTextureMovieClip mc;
        
        // movieclip from pixels (good for large quantities of images - CPU instead of GPU)
        //ofxPixelsImageSequenceLoader imageSequence;
        //ofxPixelsMovieClip mc;
		
};
