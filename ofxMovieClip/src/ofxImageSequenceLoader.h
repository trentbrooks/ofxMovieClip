
#pragma once

#include "ofMain.h"

/*
 ofxImageSequenceLoader
 - loads image sequences as ofTexture's or ofPixels
 - declare as ofxTextureImageSequenceLoader, or ofxPixelImageSequenceLoader (typedefs at bottom)
 */

template <typename T>
struct MovieClipData {
    
    vector<T*> imageFrames; // ofTexture*, ofPixels*
    int imageFramesSize;
    string frameLabel;
    
    // extra info (only used by threaded loader)
    bool complete;
    int resizeWidth, resizeHeight;
    string folderPath;
    ofDirectory dir;
};


template <typename ImageType> // ofTexture, or ofPixels
class ofxImageSequenceLoader {
    
    public:
        ofxImageSequenceLoader();
        ~ofxImageSequenceLoader();
    
        // loads image sequence by folder
        void loadSequence(string folderPath, string frameLabel = "", int resizeWidth = 0, int resizeHeight = 0);
    
        // deprecated- name to long
        void loadAndCreateSequence(string folderPath, string frameLabel = "", int resizeWidth = 0, int resizeHeight = 0) {
            loadSequence(folderPath, frameLabel, resizeWidth, resizeHeight);
        }
    
        vector<MovieClipData<ImageType>*> assetCollections;
        int assetCollectionSize;        
        int getAssetsId(string frameLabel);
    
};



// types = ofTexture, ofPixels
typedef ofxImageSequenceLoader<ofTexture> ofxTextureSequenceLoader;
typedef ofxImageSequenceLoader<ofPixels> ofxPixelsSequenceLoader;

// deprecated - names too long
typedef ofxImageSequenceLoader<ofTexture> ofxTextureImageSequenceLoader;
typedef ofxImageSequenceLoader<ofPixels> ofxPixelsImageSequenceLoader;
