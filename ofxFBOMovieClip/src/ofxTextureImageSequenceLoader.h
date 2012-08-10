
#pragma once

#include "ofMain.h"

class ofxTextureImageSequenceLoader
{
public:
    ofxTextureImageSequenceLoader();
    
    // loads image sequence  by folder
    void loadAndCreateSequence(string folderPath, string frameLabel = ""); 
    
    // loads image sequence by set/range
    void loadAndCreateSequence(string frameLabel, int nImages, string filenamePrefix, string filetype, int numDigits, int startFrom=0);
    
    ofTexture* makeTextureFromImage(ofImage *img, bool useARB = true);
    
    void dispose();			
    
    vector< vector<ofTexture*> > assetCollections;
    vector<string>assetFrameLabels;
    
    int getAssetsId(string frameLabel);	
    
    ofImage	loader;
};
    