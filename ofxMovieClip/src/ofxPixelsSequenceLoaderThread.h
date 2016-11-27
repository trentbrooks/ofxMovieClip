
#pragma once

#include "ofMain.h"
#include "ofxImageSequenceLoader.h"
//#include "ofxMovieClip.h"

template <class T>
class ofxMovieClip;

/*
 ofxPixelsSequenceLoaderThread
 - a pixels only version of ofxImageSequenceLoader
 - loads all assets in a thread
 
 // TODO: allow buffering, eg. load X images and delete old images as playhead passes
 */


class ofxPixelsSequenceLoaderThread : public ofxPixelsImageSequenceLoader, public ofThread  {
    
    public:
        ofxPixelsSequenceLoaderThread();
        ~ofxPixelsSequenceLoaderThread();
    
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

        // completion event - not thread safe, just fires from within the thread loop
        // need an auto update + remove
        ofEvent<void> onAllAssetsLoadedEvent;
    
        // threadsafe auto update
        void update(ofEventArgs & args);
    
        // deletes all image data
        void clearImageData(bool stopThread = true);
        void clearImageData(string frameLabel, bool stopThread = true);
    
    
        // TODO: buffered movieclips
        // let's try deleting images as we go to keep memory footprint low
        //void clearBufferFromPlayhead(MovieClipData<ofPixels>* movieClipData, int clearCount, int playheadIndex);
        //void clearBufferFromMovieClip(ofxMovieClip<ofPixels>* mc);
    
        // manually delete a single image
        //void clearImageFromPlayhead(MovieClipData<ofPixels>* movieClipData, int playheadIndex);
        
    
    protected:
    
        // keep track of which assets are loaded
        bool allAssetsLoaded;
        bool getAllAssetsLoaded();
        int collectionIndex, loadIndex;
    
        // 1. loads each image one by one until batch is complete
        void loadAllImages();
    
        // 2. load a preset buffer size of images based on the related movieclips playhead
        //void loadPlayheadImages();
    
        //Poco::Condition condition;
    
};
