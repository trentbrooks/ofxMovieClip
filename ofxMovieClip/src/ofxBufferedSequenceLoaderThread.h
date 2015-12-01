
#pragma once

#include "ofMain.h"
#include "ofxImageSequenceLoader.h"
//#include "ofxMovieClip.h"
#include "Poco/Condition.h"

template <class T>
class ofxMovieClip;

/*
 ofxPixelsSequenceLoaderThread
 - a pixels only version of ofxImageSequenceLoader
 - loads all assets in a thread
 
 // TODO: allow buffering, eg. load X images and delete old images as playhead passes
 */


class ofxBufferedSequenceLoaderThread : public ofxPixelsImageSequenceLoader, public ofThread  {
    
    public:
        ofxBufferedSequenceLoaderThread();
        ~ofxBufferedSequenceLoaderThread();
    
        // loads image sequence  by folder
        void loadSequence(string folderPath, string frameLabel = "", int resizeWidth = 0, int resizeHeight = 0);
    
        // deprecated- name to long
        void loadAndCreateSequence(string folderPath, string frameLabel = "", int resizeWidth = 0, int resizeHeight = 0) {
            loadSequence(folderPath, frameLabel, resizeWidth, resizeHeight);
        };
    
    
        // call begin() after all loadSequence's called to start the threaded load
        void begin();
        void startThread(bool mutexBlocks = true);
        int sleepTime;
        void threadedFunction();

    
        // deletes all image data
        void clearImageData(bool stopThread = true);
        void clearImageData(string frameLabel, bool stopThread = true);
    
    
        // TODO: buffered movieclips
        // let's try deleting images as we go to keep memory footprint low
        //void clearBufferFromPlayhead(MovieClipData<ofPixels>* movieClipData, int clearCount, int playheadIndex);
        //void clearBufferFromMovieClip(ofxMovieClip<ofPixels>* mc);
    
        // manually delete a single image
        //void clearImageFromPlayhead(MovieClipData<ofPixels>* movieClipData, int playheadIndex);
    
        int maxBufferSize;
        int currentPlayheadIndex;
        bool currentLoopMode;
        void onPlayheadChanged(int playheadIndex, bool loopMode);
    
    protected:
    
        // keep track of which assets are loaded
        bool allAssetsLoaded;
        int collectionIndex, loadIndex, deleteIndex;
    
        // 1. loads each image one by one until batch is complete
        void loadAllImages();
    
        // 2. load a preset buffer size of images based on the related movieclips playhead
        void loadPlayheadImages();
    
        // 3.
        void loadBufferedImages();
    
        Poco::Condition condition;
    
};
