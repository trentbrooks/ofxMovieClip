
#pragma once

#include "ofMain.h"
#include "ofxImageSequenceLoader.h"

/*
 ofxPixelsSequenceLoaderThread
 - a pixels only version of ofxImageSequenceLoader
 - loads assets in a thread
 */


class ofxPixelsSequenceLoaderThread : public ofxPixelsImageSequenceLoader, public ofThread  {
    
    public:
        ofxPixelsSequenceLoaderThread();
        ~ofxPixelsSequenceLoaderThread();
    
        // loads image sequence  by folder
        void loadSequence(string folderPath, string frameLabel = "", int resizeWidth = 0, int resizeHeight = 0);
    
        // call begin() after all loadSequence's called to start the threaded load
        void begin();
        void startThread(bool mutexBlocks = true);
        int sleepTime;
        void threadedFunction();
    
        // keep track of which assets are loaded
        bool allAssetsLoaded;
        int collectionIndex, loadIndex;
    
        void clearImageData(string frameLabel);
};
