#include "ofxPixelsSequenceLoaderThread.h"
#include "ofxMovieClip.h"

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
    
    mutex.lock();
    ofDirectory dir;
    int numFiles = dir.listDir(folderPath);
    dir.sort();
    
    MovieClipData<ofPixels>* assetsAndLabel = new MovieClipData<ofPixels>();
    
    for(int i=0; i < numFiles; i++) {
        
        string fileExtension = ofToUpper(dir.getFile(i).getExtension());
        if(fileExtension == "JPG" || fileExtension == "PNG") {
            // valid image - push to string vector
            assetsAndLabel->imagePaths.push_back(dir.getPath(i));
            
            // load and allocate memory for images
            ofPixels* pixs = new ofPixels();
            assetsAndLabel->imageFrames.push_back(pixs);
        } else {
            ofLogVerbose() << "File is not valid image: " << dir.getPath(i);
        }
    }
    
    // save meta data
    assetsAndLabel->frameLabel = (frameLabel == "") ? folderPath : frameLabel;
    assetsAndLabel->folderPath = folderPath;
    //assetsAndLabel->dir = dir;
    assetsAndLabel->resizeHeight = resizeHeight;
    assetsAndLabel->resizeWidth = resizeWidth;
    assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
    assetsAndLabel->complete = false;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();
    mutex.unlock();
}

void ofxPixelsSequenceLoaderThread::startThread(bool mutexBlocks) {
    
    if(!isThreadRunning()) {
        allAssetsLoaded = false;
        collectionIndex = loadIndex = 0;// assumes we have images to load
        ofThread::startThread(mutexBlocks);
        
        ofAddListener(ofEvents().update,this,&ofxPixelsSequenceLoaderThread::update);
    } else {
        ofLog() << "thread already running...";
    }
    
}

//--------------------------------------------------------------
void ofxPixelsSequenceLoaderThread::update(ofEventArgs & args) {
    
    // auto updates should also automaticall remove listener when complete
    //ofLog() << "pixels are updateing...";
    if(getAllAssetsLoaded()) {
        
        ofLogVerbose() << "All assets loaded: stopping thread";
        
        stopThread();
        
        // notofy when all assets are ready - not thread safe?
        ofNotifyEvent(onAllAssetsLoadedEvent);
        
        // stop the updates
        ofRemoveListener(ofEvents().update,this,&ofxPixelsSequenceLoaderThread::update);
    }
}

void ofxPixelsSequenceLoaderThread::threadedFunction(){
    
    while( isThreadRunning() ){
        
        mutex.lock();
        
        // this begins loading all images from index 0 until complete
        // images are loaded 1 by 1 (loadIndex) - so adjusting the sleepTime might be necessary to keep up
        loadAllImages();
        
        mutex.unlock();
        
        sleep(sleepTime); //?
        
        /*
        if(allAssetsLoaded) {
            ofLogVerbose() << "All assets loaded: stopping thread";
            stopThread();
            
            // notofy when all assets are ready - not thread safe?
            ofNotifyEvent(onAllAssetsLoadedEvent);
        }
         */
    }
}

void ofxPixelsSequenceLoaderThread::loadAllImages() {
    
    if(!allAssetsLoaded && collectionIndex != -1) {
        
        MovieClipData<ofPixels>* assetCollection = assetCollections[collectionIndex];
        if(!assetCollection->complete) {
            
            // load a single image from the movieclip meta info- then increment the loadIndex
            ofPixels* pixs = assetCollection->imageFrames[loadIndex];
            if(!pixs->isAllocated()) {
                
                // load pixels data
                bool loaded = ofLoadImage(*pixs, assetCollection->imagePaths[loadIndex]);
                if(!loaded) {
                    ofLogError() << "* Failed to load image: " << assetCollection->imagePaths[loadIndex];
                } else {
                    // resize/log on success
                    if(assetCollection->resizeWidth > 0) pixs->resize(assetCollection->resizeWidth, assetCollection->resizeHeight);
                    ofLogVerbose() << "Pixels: " << assetCollection->imagePaths[loadIndex];
                }
                
                // increment load index even if image failed
                loadIndex++;
                if(loadIndex >= assetCollection->imageFramesSize) {
                    loadIndex = 0;
                    assetCollection->complete = true;
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
}




//--------------------------------------------------------------
void ofxPixelsSequenceLoaderThread::clearImageData(bool stopThread) {
    
    // check if thread is still running
    if(stopThread) waitForThread(true, 10000);
    
    // clear all image data for all frame labels
    mutex.lock();
    for(int assetIndex = 0; assetIndex < assetCollections.size(); assetIndex++) {
        if(assetIndex >= 0 && assetIndex < assetCollections[assetIndex]->imageFramesSize) {
            for(int i = 0; i < assetCollections[assetIndex]->imageFramesSize; i++) {
                assetCollections[assetIndex]->imageFrames[i]->clear();
				ofLogVerbose() << "Deleted asset from collection: " << assetIndex << " #: " << i;
            }
            assetCollections[assetIndex]->complete = false;
        }
    }
    allAssetsLoaded = false;
    collectionIndex =loadIndex = -1;
    mutex.unlock();
}

void ofxPixelsSequenceLoaderThread::clearImageData(string frameLabel, bool stopThread) {
    
    // check if thread is still running
    if(stopThread) waitForThread(true, 10000);
    
    // clear all image data for selected frameLabel
    mutex.lock();
    int assetIndex = getAssetsId(frameLabel);
    if(assetIndex >= 0 && assetIndex < assetCollections[assetIndex]->imageFramesSize) {
        for(int i = 0; i < assetCollections[assetIndex]->imageFramesSize; i++) {
            assetCollections[assetIndex]->imageFrames[i]->clear();
        }
        assetCollections[assetIndex]->complete = false;
    }    
    allAssetsLoaded = false;
    collectionIndex =loadIndex = -1;
    mutex.unlock();
    
}

bool ofxPixelsSequenceLoaderThread::getAllAssetsLoaded() {
    Poco::ScopedLock<ofMutex> lock(mutex);
    return allAssetsLoaded;
}

/*void ofxPixelsSequenceLoaderThread::clearBufferFromPlayhead(MovieClipData<ofPixels>* movieClipData, int clearCount, int playheadIndex) {
    
    // something weird going on with playhead herer!
    mutex.lock();
    if(playheadIndex <= 0 ) {
        mutex.unlock();
        return; // || playheadIndex == movieClipData->imageFramesSize
    }
    for(int i = 0; i < playheadIndex-1; i++) {
        ofPixels* px = movieClipData->imageFrames[i];
        if(px->isAllocated()) {
            ofLog() << "Cleared this image!: " << i << " of " <<  movieClipData->imageFramesSize << ". Playhead: " << playheadIndex << ". LoadIndex: " << loadIndex;
            px->clear();
        }
    }
    movieClipData->complete = false;
    mutex.unlock();
}

void ofxPixelsSequenceLoaderThread::clearBufferFromMovieClip(ofxMovieClip<ofPixels>* mc) {
    
    clearBufferFromPlayhead(mc->activeAsset, 30, mc->getPlayhead());
}

void ofxPixelsSequenceLoaderThread::clearImageFromPlayhead(MovieClipData<ofPixels>* movieClipData, int playheadIndex) {
    
    mutex.lock();
    if(playheadIndex < 0 ) {
        mutex.unlock();
        return; // || playheadIndex == movieClipData->imageFramesSize
    }
    ofPixels* px = movieClipData->imageFrames[playheadIndex];
    if(px->isAllocated()) {
        ofLog() << "Cleared this image!: " << playheadIndex << " of " <<  movieClipData->imageFramesSize;
        px->clear();
        movieClipData->complete = false;
    }
    mutex.unlock();
}*/


