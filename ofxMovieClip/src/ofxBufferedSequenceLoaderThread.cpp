#include "ofxBufferedSequenceLoaderThread.h"
#include "ofxMovieClip.h"

//--------------------------------------------------------------
ofxBufferedSequenceLoaderThread::ofxBufferedSequenceLoaderThread() {
    allAssetsLoaded = false;
    collectionIndex = loadIndex = -1;
    currentLoopMode = false;
    deleteIndex = -1;
    sleepTime = 2;
    
    currentPlayheadIndex = -1;
    maxBufferSize = 60; // store this many images im memory only
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
    collectionIndex = loadIndex = currentPlayheadIndex = 0;// assumes we have images to load
    deleteIndex = -1;
    ofThread::startThread(mutexBlocks);
}

//--------------------------------------------------------------
void ofxBufferedSequenceLoaderThread::threadedFunction(){
    
    while( isThreadRunning() ){
        
        //mutex.lock();
        
        // this begins loading all images from index 0 until complete
        // images are loaded 1 by 1 (loadIndex) - so adjusting the sleepTime might be necessary to keep up
        //loadAllImages();
        //loadPlayheadImages();
        loadBufferedImages(); // preloads 30, then moves window with playhead as needed
        
        //mutex.unlock();
        
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


// will need to fix thread locks here...
void ofxBufferedSequenceLoaderThread::loadBufferedImages() {
    
    // threaded run - always running, never keep too many images in memory
    // no need to do anything when playhead is stopped + buffer is full
    // maybe we should stop + start the thread only when the playhead needs it?
    if(!allAssetsLoaded && collectionIndex != -1) {
        
        MovieClipData<ofPixels>* assetCollection = assetCollections[collectionIndex];
        //ofLog() << "load count: " << loadIndex << ", playhead: " << currentPlayheadIndex << ", frames: " << assetCollection->imageFramesSize;
        
        bool needsFrame = false;
        if(loadIndex < maxBufferSize || !assetCollection->complete) {
            
            // havent filled pre-buffer yet - want at least 30 frames to begin
            //ofLog() << "Cmon go down?";
            needsFrame = true;
        }
        else {
            
            // we have the pre-buffer, now just check against the playhead
            // basically keep the window moving with at least 30 frames ahead all the time
            int maxLoadFrames = currentPlayheadIndex + maxBufferSize;
            if(maxLoadFrames > assetCollection->imageFramesSize) maxLoadFrames = assetCollection->imageFramesSize;
            //maxLoadFrames = maxLoadFrames % assetCollection->imageFramesSize; // make sure we dont test for > than max
            if(maxLoadFrames > loadIndex) {

                needsFrame = true;
            }
        }
        
        // load the next frame
        if(needsFrame) {
            
            // load a single image from the movieclip meta info- then increment the loadIndex
            ofPixels* pixs = assetCollection->imageFrames[loadIndex];
            //ofLog() << "Need a frame and going to load... " << loadIndex << " : " << pixs->isAllocated();
            if(!pixs->isAllocated()) {
                
                
                // load pixels data
                bool loaded = ofLoadImage(*pixs, assetCollection->imagePaths[loadIndex]);
                if(!loaded) {
                    ofLogError() << "* Failed to load image: " << assetCollection->imagePaths[loadIndex];
                } else {
                    // resize/log on success
                    if(assetCollection->resizeWidth > 0) pixs->resize(assetCollection->resizeWidth, assetCollection->resizeHeight);
                    ofLogVerbose() << "Loaded Pixels: " << assetCollection->imagePaths[loadIndex];
                }
                
                // check if we have enough in buffer
                // load 30 frames before 'complete'
                // we want this N frame + the next 30 always in memory
                if(!assetCollection->complete) {
                    
                    // only fires this once
                    int maxLoadFrames = maxBufferSize; //currentPlayheadIndex + maxBufferSize
                    if(maxLoadFrames > assetCollection->imageFramesSize) {
                        ofLog() << "Not a big enough buffer allocated- using frames size instead";
                        maxLoadFrames = assetCollection->imageFramesSize;
                    }
                    //maxLoadFrames = maxLoadFrames % assetCollection->imageFramesSize;
                    if(loadIndex >= maxLoadFrames) {
                        assetCollection->complete = true;
                        ofLogVerbose() << "Pre-buffer load complete: " << loadIndex << "/" << maxLoadFrames;
                        //condition.wait(mutex); // finished loading - waiting for playhead to move
                    }
                } else {
                    
                    // we have the pre-buffer filled - now just load what we need to keep the window moving
                    // since we loaded just 1 new image - lets delete just 1 image, the one before the current playhead (seems a bit jumpy?)
                    int deleteIndex = currentPlayheadIndex - 1;
                    if(deleteIndex >= 0) {
                        ofPixels* deletePx = assetCollection->imageFrames[deleteIndex];
                        if(deletePx->isAllocated()) {
                            ofLogVerbose() << "Deleting oldest (unused?) frame... " << deleteIndex << ", but also just loaded: " << loadIndex;
                            deletePx->clear();
                        }
                        
                    }
                    /*deleteIndex++;
                    if(deleteIndex >= 0) {
                        if(deleteIndex >= assetCollection->imageFramesSize) deleteIndex = 0;
                        //int deleteIndex = loadIndex - maxBufferSize - 1; // this should be the frame before the current
                        //if(deleteIndex < 0) deleteIndex += assetCollection->imageFramesSize;
                        //if(deleteIndex < 0) deleteIndex = 0;
                        ofLogVerbose() << "Deleting oldest (unused?) frame... " << deleteIndex << ", but also just loaded: " << loadIndex;
                        ofPixels* deletePx = assetCollection->imageFrames[deleteIndex];
                        if(deletePx->isAllocated()) {
                            
                            deletePx->clear();
                        }
                    }*/
                    
                    
                    // delete oldest/unused frames
                    /*int deleteBufferStart = currentPlayheadIndex - maxBufferSize;
                    if(deleteBufferStart < 0) deleteBufferStart += assetCollection->imageFramesSize;
                    //deleteBufferStart = deleteBufferStart % assetCollection->imageFramesSize;
                    //ofLog() << "Start delete index: " << deleteBufferStart;
                    int modFrames = deleteBufferStart + maxBufferSize;
                    modFrames = modFrames % assetCollection->imageFramesSize;
                    for(int i = deleteBufferStart; i < modFrames; i++) {
                        ofPixels* pixs = assetCollection->imageFrames[i];
                        if(pixs->isAllocated()) {
                            ofLogVerbose() << "Deleting oldest (unused?) frame... " << i;
                            assetCollection->imageFrames[i]->clear();
                            //break;
                        }
                     }*/
                }
                
                
                // increment load index even if image failed
                loadIndex++;
                if(loadIndex >= assetCollection->imageFramesSize) {
                    //ofLog() << "Fin...";
                    loadIndex = 0; // this will restart the loading process...
                    if(!currentLoopMode) stopThread(); // this will force stop at the end
                    
                }
            } else {
                
                // this condition happens when an allocation is valid- eg if pixels were skipped somehow
                //ofLog() << "Px already exist- skipping frame: " << loadIndex;
                loadIndex++;
                if(loadIndex >= assetCollection->imageFramesSize) {
                    //ofLog() << "Fin2..."; // this will restart the loading process...
                    loadIndex = 0;
                    if(!currentLoopMode) stopThread(); // this will force stop at the end
                }
            }
        }
    }
}

void ofxBufferedSequenceLoaderThread::loadPlayheadImages() {
    
    // wait until the playhead has moved before loading more images...
    //condition.wait(mutex);
    
    float t = ofGetElapsedTimef();
    if(!allAssetsLoaded && collectionIndex != -1) {
        
        MovieClipData<ofPixels>* assetCollection = assetCollections[collectionIndex];
        ofLog() << "load count: " << loadIndex << ", playhead: " << currentPlayheadIndex;
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
                
                
                // load 30 frames before 'complete'
                // we want this N frame + the next 30 always in memory
                int maxLoadFrames = currentPlayheadIndex + maxBufferSize;
                //ofLog() << "max frames: " << maxLoadFrames;
                //maxLoadFrames = maxLoadFrames % assetCollection->imageFramesSize;
                if(loadIndex >= maxLoadFrames) {
                    //if(loadIndex >= maxBufferSize) {
                    //loadIndex = 0;
                    assetCollection->complete = true;
                    ofLogVerbose() << "Load complete: " << loadIndex << "/" << maxLoadFrames;
                    //condition.wait(mutex); // finished loading - waiting for playhead to move
                }
                
                /*int bufferedIndex = currentPlayheadIndex + maxBufferSize;
                 bufferedIndex = bufferedIndex % assetCollection->imageFramesSize;
                 ofLog() << "Bufferd index; " << bufferedIndex;
                 if(loadIndex >= bufferedIndex) {
                 condition.wait(mutex);
                 }*/
            
                
                
            } else {
                
                //ofLogVerbose() << "Already allocated! " << loadIndex;
                //condition.wait(mutex);
            }
        } else {
            
            //condition.wait(mutex);
            
            // assetCollection is in 'complete' state, eg. we loaded 30 frames only
            // but the playhead is probably moving + passed 30 - so keep the loading buffer going
            //int maxLoadFrames = currentPlayheadIndex + maxBufferSize;
            //if(maxLoadFrames > assetCollection->imageFramesSize) maxLoadFrames = assetCollection->imageFramesSize;
            //maxLoadFrames = maxLoadFrames % assetCollection->imageFramesSize;
            //ofLog() << "(complete) max frames: " << maxLoadFrames << ", load index: " << loadIndex;
            //ofLog() << currentPlayheadIndex << " / " << loadIndex;
            int maxLoadFrames = currentPlayheadIndex + maxBufferSize;
            //maxLoadFrames = maxLoadFrames % assetCollection->imageFramesSize;
            if(maxLoadFrames > loadIndex) {
                
                
                loadIndex++;
                //ofLog() << "loading new frame: " << loadIndex;
                
                // load a single image from the movieclip meta info- then increment the loadIndex
                ofPixels* pixs = assetCollection->imageFrames[loadIndex];
                if(!pixs->isAllocated()) {
                    bool loaded = ofLoadImage(*pixs, assetCollection->imagePaths[loadIndex]);
                    //ofLog() << "Next loaded... " << loaded;
                    //ofLog() << (ofGetElapsedTimef() - t);
                    //condition.wait(mutex); // finished loading - waiting for playhead to move
                }
                
                //assetCollection->complete = false;
                //loadIndex++;
                //ofLogVerbose() << "Toggle load incomplete - need some more frames: " << loadIndex << ", max: " << currentPlayheadIndex;
                //condition.wait(mutex);
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


// if the playhead is moving- we need to trigger 'loading' and make sure were getting the next N frames
// this will only be called when the playhead has been changed
void ofxBufferedSequenceLoaderThread::onPlayheadChanged(int playheadIndex, bool loopMode) {
    
    mutex.lock();
    if(currentPlayheadIndex != playheadIndex) {
        
        // delete old/current image! (not threaded for now)
        /*if(currentPlayheadIndex >= 0) {
            ofPixels* pixs = assetCollections[0]->imageFrames[currentPlayheadIndex];
            if(pixs->isAllocated()) {
                ofLogVerbose() << "Deleting old/current image: " << currentPlayheadIndex;
                pixs->clear();
            }
        }*/
        
        /*for(int i = 0; i < assetCollections[assetIndex]->imageFramesSize; i++) {
            assetCollections[assetIndex]->imageFrames[i]->clear();
        }*/
        
        currentPlayheadIndex = playheadIndex;
        currentLoopMode = loopMode;
        /*if(currentPlayheadIndex == 0 && assetCollections[collectionIndex]->complete) {
            
            // restart loading again from 0
            assetCollections[collectionIndex]->complete = false;
            loadIndex = 0;
            
        }*/
        //condition.signal();
        
        // this force starts the thread if it's not running
        //ofLog() << "new frame: " << currentPlayheadIndex;
        //if(bool loopMode
        if(!isThreadRunning() && currentLoopMode) {
            ofLog() << "Restarting thread now: " << playheadIndex;
            startThread();
        }
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
    collectionIndex = loadIndex = -1;
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


