#pragma once

#include "ofMain.h"

class ofxFBOImageSequenceLoader 
{
	public:
		ofxFBOImageSequenceLoader();
		
        // loads image sequence  by folder
        void loadAndCreateSequence(string folderPath, string frameLabel = ""); 
    
        // loads image sequence by set/range
		void loadAndCreateSequence(string frameLabel, int nImages, string filenamePrefix, string filetype, int numDigits, int startFrom=0);
        
		
		void dispose();			
		
        vector< vector<ofFbo*> > assetCollections;
		vector<string>assetFrameLabels;
		
		int getAssetsId(string frameLabel);	
		
		ofImage	loader;			

		
};

