
#pragma once

#include "ofMain.h"

/*
 ofxImageSequenceLoader
 - loads image sequences as ofTexture's or ofPixels
 - declare as ofxTextureImageSequenceLoader, or ofxPixelImageSequenceLoader (typedefs at bottom)
 */

template <typename T>
struct MovieClipAssetsAndLabel {
    
    //template <typename T>
    vector<T*> imageFrames; // ofTexture*, ofPixels*
    int imageFramesSize;
    string frameLabel;
};


template <typename ImageType> // ofTexture, or ofPixels
class ofxImageSequenceLoader {
    
    public:
        ofxImageSequenceLoader();
        ~ofxImageSequenceLoader();
    
        // loads image sequence  by folder
        void loadAndCreateSequence(string folderPath, string frameLabel = "", int resizeWidth = 0, int resizeHeight = 0);

        ofTexture* makeTextureFromImage(ofImage *img, bool useARB = true);
    
        vector<MovieClipAssetsAndLabel<ImageType>*> assetCollections;
        int assetCollectionSize;        
        int getAssetsId(string frameLabel);	        
};



// types = ofTexture, ofPixels
typedef ofxImageSequenceLoader<ofTexture> ofxTextureImageSequenceLoader;
typedef ofxImageSequenceLoader<ofPixels> ofxPixelsImageSequenceLoader;