
#pragma once

#include "ofMain.h"

struct ofxFBOAssetsAndLabel {
    
    vector<ofFbo*> imageFrames;
    int imageFramesSize;
    string frameLabel;
};

class ofxFBOImageSequenceLoader 
{
	public:
		ofxFBOImageSequenceLoader();
		
        // loads image sequence  by folder
        void loadAndCreateSequence(string folderPath, string frameLabel = "", int resizeWidth = 0, int resizeHeight = 0); 
    
        // loads image sequence by set/range
		void loadAndCreateSequence(string frameLabel, int nImages, string filenamePrefix, string filetype, int numDigits, int startFrom=0, int resizeWidth = 0, int resizeHeight = 0);
        
				
        vector<ofxFBOAssetsAndLabel*> assetCollections;
        int assetCollectionSize;
		
		int getAssetsId(string frameLabel);	
				
};

