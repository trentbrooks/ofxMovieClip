#pragma once

#include "ofxFBOTexture.h"
#include "ofMain.h"

#define STEP_STOP 0
#define STEP_FORWARD 1
#define STEP_REVERSE 2

class ofxFBOImageSequenceLoader 
{
	public:
		ofxFBOImageSequenceLoader();
		
		void loadAndCreateSequence(string frameLabel, int nImages, string filenamePrefix, string filetype, int numDigits);
		
		void dispose();			
		
		vector< vector<ofxFBOTexture*> > assetCollections;
		vector<string>assetFrameLabels;
		
		int getAssetsId(string frameLabel);	
	
	protected:
		
		ofImage	loader;			
		bool imageSizeAllocated;
		int imageWidths;
		int imageHeights;
	
		
		
};

