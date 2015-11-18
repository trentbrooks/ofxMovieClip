#include "ofxBufferedSequenceLoaderThread.h"
#include "ofxMovieClip.h"

//--------------------------------------------------------------
ofxBufferedSequenceLoaderThread::ofxBufferedSequenceLoaderThread() {
    allAssetsLoaded = false;
    collectionIndex = loadIndex = -1;
    sleepTime = 5;
    
    currentPlayheadIndex = -1;
    maxBufferSize = 30;
}

ofxBufferedSequenceLoaderThread::~ofxBufferedSequenceLoaderThread() {
    waitForThread();
}
 
//--------------------------------------------------------------
void ofxBufferedSequenceLoaderThread::loadSequence(string folderPath, string frameLabel, int resizeWidth, int resizeHeight) {
    
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

void ofxBufferedSequenceLoaderThread::startThread(bool mutexBlocks) {
    
    allAssetsLoaded = false;
    collectionIndex = loadIndex = 0;// assumes we have images to load
    ofThread::startThread(mutexBlocks);
}

//--------------------------------------------------------------
void ofxBufferedSequenceLoaderThread::threadedFunction(){
    
    while( isThreadRunning() ){
        
        mutex.lock();
        
        // this begins loading all images from index 0 until complete
        // images are loaded 1 by 1 (loadIndex) - so adjusting the sleepTime might be necessary to keep up
        //loadAllImages();
        loadPlayheadImages();
        
        mutex.unlock();
        
        sleep(sleepTime); //?
        
        if(allAssetsLoaded) {
            ofLogVerbose() << "All assets loaded: stopping thread";
            stopThread();
        }
    }
}

void ofxBufferedSequenceLoaderThread::loadAllImages() {
    
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

void ofxBufferedSequenceLoaderThread::loadPlayheadImages() {
    
    // wait until the playhead has moved before loading more images...
    //condition.wait(mutex);
    
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
                //loadIndex = loadIndex % assetCollection->imageFramesSize;
                if(loadIndex >= assetCollection->imageFramesSize) {
                    //loadIndex = 0;
                    assetCollection->complete = true;
                    ofLog() << "Load complete";
                    condition.wait(mutex);
                }
                
                /*int bufferedIndex = currentPlayheadIndex + maxBufferSize;
                bufferedIndex = bufferedIndex % assetCollection->imageFramesSize;
                ofLog() << "Bufferd index; " << bufferedIndex;
                if(loadIndex >= bufferedIndex) {
                    condition.wait(mutex);
                }*/
                
                
            } else {
                
                //ofLogVerbose() << "Already allocated! " << loadIndex;
                condition.wait(mutex);
            }
        }
        
        // when finished loading current sequence - start loading next, or finish
        /*if(assetCollection->complete) {
            if(collectionIndex >= assetCollectionSize-1) {
                allAssetsLoaded = true;
                collectionIndex = 0;
            } else {
                collectionIndex++;
            }
        }*/
        
    }
}


// if the playhead is moving- we need to trigger 'loading'
void ofxBufferedSequenceLoaderThread::updateLoadFromPlayhead(int playheadIndex) {
    
    mutex.lock();
    if(currentPlayheadIndex != playheadIndex) {
        
        // delete old image! (not threaded for now)
        if(currentPlayheadIndex >= 0) {
            ofPixels* pixs = assetCollections[0]->imageFrames[currentPlayheadIndex];
            if(pixs->isAllocated()) {
                ofLog() << "Deleting old image: " << currentPlayheadIndex;
                pixs->clear();
            }
        }
        
        /*for(int i = 0; i < assetCollections[assetIndex]->imageFramesSize; i++) {
            assetCollections[assetIndex]->imageFrames[i]->clear();
        }*/
        
        currentPlayheadIndex = playheadIndex;
        if(currentPlayheadIndex == 0 && assetCollections[collectionIndex]->complete) {
            
            // restart loading again from 0
            assetCollections[collectionIndex]->complete = false;
            loadIndex = 0;
            
        }
        condition.signal();
    }
    mutex.unlock();
}


//--------------------------------------------------------------
void ofxBufferedSequenceLoaderThread::clearImageData(bool stopThread) {
    
    // check if thread is still running
    if(stopThread) waitForThread();
    
    // clear all image data for all frame labels
    mutex.lock();
    for(int assetIndex = 0; assetIndex < assetCollections.size(); assetIndex++) {
        if(assetIndex >= 0 && assetIndex < assetCollections[assetIndex]->imageFramesSize) {
            for(int i = 0; i < assetCollections[assetIndex]->imageFramesSize; i++) {
                assetCollections[assetIndex]->imageFrames[i]->clear();
            }
            assetCollections[assetIndex]->complete = false;
        }
    }
    allAssetsLoaded = false;
    collectionIndex =loadIndex = -1;
    mutex.unlock();
}

void ofxBufferedSequenceLoaderThread::clearImageData(string frameLabel, bool stopThread) {
    
    // check if thread is still running
    if(stopThread) waitForThread();
    
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

/*void ofxBufferedSequenceLoaderThread::clearBufferFromPlayhead(MovieClipData<ofPixels>* movieClipData, int clearCount, int playheadIndex) {
    
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

void ofxBufferedSequenceLoaderThread::clearBufferFromMovieClip(ofxMovieClip<ofPixels>* mc) {
    
    clearBufferFromPlayhead(mc->activeAsset, 30, mc->getPlayhead());
}

void ofxBufferedSequenceLoaderThread::clearImageFromPlayhead(MovieClipData<ofPixels>* movieClipData, int playheadIndex) {
    
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

