#include "ofxPixelsSequenceLoaderThread.h"

//--------------------------------------------------------------
ofxPixelsSequenceLoaderThread::ofxPixelsSequenceLoaderThread() {
    allAssetsLoaded = false;
    collectionIndex = loadIndex = -1;
    sleepTime = 5;
}

ofxPixelsSequenceLoaderThread::~ofxPixelsSequenceLoaderThread() {
    waitForThread();
}
 
//--------------------------------------------------------------
void ofxPixelsSequenceLoaderThread::loadSequence(string folderPath, string frameLabel, int resizeWidth, int resizeHeight) {
    
    ofDirectory dir;
    int numFiles = dir.listDir(folderPath);
    dir.sort();
    
    MovieClipData<ofPixels>* assetsAndLabel = new MovieClipData<ofPixels>();
    
    for(int i=0; i < numFiles; i++) {
        
        // load and allocate memory for images
        ofPixels* pixs = new ofPixels();
        assetsAndLabel->imageFrames.push_back(pixs);
    }
    
    // save meta data
    assetsAndLabel->frameLabel = (frameLabel == "") ? folderPath : frameLabel;
    assetsAndLabel->folderPath = folderPath;
    assetsAndLabel->dir = dir;
    assetsAndLabel->resizeHeight = resizeHeight;
    assetsAndLabel->resizeWidth = resizeWidth;
    assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
    assetsAndLabel->complete = false;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();
}

void ofxPixelsSequenceLoaderThread::startThread(bool mutexBlocks) {
    
    allAssetsLoaded = false;
    collectionIndex = loadIndex = 0;
    ofThread::startThread(mutexBlocks);
}

//--------------------------------------------------------------
void ofxPixelsSequenceLoaderThread::threadedFunction(){
    
    while( isThreadRunning() ){
        
        mutex.lock();
        if(!allAssetsLoaded) {
            
            // load a single image from the movieclip meta info
            MovieClipData<ofPixels>* assetCollection = assetCollections[collectionIndex];
            if(!assetCollection->complete) {
                
                ofPixels* pixs = assetCollection->imageFrames[loadIndex];
                if(!pixs->isAllocated()) {
                    
                    string fileExtension = ofToUpper(assetCollection->dir.getFile(loadIndex).getExtension());
                    if(fileExtension == "JPG" || fileExtension == "PNG") {
                        
                        // load pixels data
                        bool loaded = ofLoadImage(*pixs, assetCollection->dir.getPath(loadIndex));
                        if(!loaded) {
                            ofLogError() << "* Failed to load image: " << assetCollection->dir.getPath(loadIndex);
                        } else {
                            // resize/log on success
                            if(assetCollection->resizeWidth > 0) pixs->resize(assetCollection->resizeWidth, assetCollection->resizeHeight);
                            ofLogVerbose() << "Pixels: " << assetCollection->dir.getPath(loadIndex);
                        }
                        
                        // increment load index even if image failed
                        loadIndex++;
                        if(loadIndex >= assetCollection->imageFramesSize) {
                            loadIndex = 0;
                            assetCollection->complete = true;
                        }
                    }
                }
            }
            
            // when finished loading current sequence - start loading next, or finish
            if(assetCollection->complete) {
                if(collectionIndex >= assetCollectionSize-1) {
                    allAssetsLoaded = true;
                    collectionIndex = 0;
                } else {
                    collectionIndex++;
                }
            }

        }
        mutex.unlock();
        
        sleep(sleepTime); //?
        
        if(allAssetsLoaded) stopThread();
    }
}

//--------------------------------------------------------------
void ofxPixelsSequenceLoaderThread::clearImageData(string frameLabel) {
    
    int assetIndex = getAssetsId(frameLabel);
    if(assetIndex >= 0 && assetIndex < assetCollections[assetIndex]->imageFramesSize) {
        for(int i = 0; i < assetCollections[assetIndex]->imageFramesSize; i++) {
            assetCollections[assetIndex]->imageFrames[i]->clear();
        }
        assetCollections[assetIndex]->complete = false;
    }    
    allAssetsLoaded = false;
}


