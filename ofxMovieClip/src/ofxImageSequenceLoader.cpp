#include "ofxImageSequenceLoader.h"

//--------------------------------------------------------------
template <typename ImageType>
ofxImageSequenceLoader<ImageType>::ofxImageSequenceLoader() {
    assetCollectionSize = 0;
}

template <typename ImageType>
ofxImageSequenceLoader<ImageType>::~ofxImageSequenceLoader() {
    
    for (int i = 0; i < assetCollections.size(); i++) {
        for (int j = 0; j < assetCollections[i]->imageFrames.size(); j++) {
            delete assetCollections[i]->imageFrames[j];
        }
        
        assetCollections[i]->imageFrames.clear();
    }
    
    assetCollections.clear();
}
 
//--------------------------------------------------------------
// template specialisation: ofTexture
template<>
void ofxImageSequenceLoader<ofTexture>::loadSequence(string folderPath, string frameLabel, int resizeWidth, int resizeHeight) {
    
    ofDirectory dir;
    int numFiles = dir.listDir(folderPath);
    dir.sort();
    
    MovieClipData<ofTexture>* assetsAndLabel = new MovieClipData<ofTexture>();
    
	for(int i=0; i < numFiles; i++){
        
        string fileExtension = ofToUpper(dir.getFile(i).getExtension());
        if(fileExtension == "JPG" || fileExtension == "PNG") {
            
            // load and allocate memory for images
            ofPixels pixsLoader;
            bool loaded = ofLoadImage(pixsLoader, dir.getPath(i));
            if(!loaded) {
                ofLogError() << "* Failed to load image: " << dir.getPath(i);
                continue;
            }
            
            if(resizeWidth > 0) pixsLoader.resize(resizeWidth, resizeHeight);
            ofLogVerbose() << "Texture: " << dir.getPath(i);
            
            // setup texture
            ofTexture* texture = new ofTexture();// makeTextureFromImage(&loader);
            texture->loadData(pixsLoader);
            
            // push texture to vector array
            assetsAndLabel->imageFrames.push_back(texture);
        }
    }
    
	// save meta data
    assetsAndLabel->frameLabel = (frameLabel == "") ? folderPath : frameLabel;
    assetsAndLabel->folderPath = folderPath;
    //assetsAndLabel->dir = dir;
    assetsAndLabel->resizeHeight = resizeHeight;
    assetsAndLabel->resizeWidth = resizeWidth;
    assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
    assetsAndLabel->complete = true;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();
}


// template specialisation: ofPixels
template<>
void ofxImageSequenceLoader<ofPixels>::loadSequence(string folderPath, string frameLabel, int resizeWidth, int resizeHeight) {
    
    ofDirectory dir;
    int numFiles = dir.listDir(folderPath);
    dir.sort();
    
    MovieClipData<ofPixels>* assetsAndLabel = new MovieClipData<ofPixels>();
        
	for(int i=0; i < numFiles; i++){
        
        string fileExtension = ofToUpper(dir.getFile(i).getExtension());
        if(fileExtension == "JPG" || fileExtension == "PNG") {
            
            // load and allocate memory for images            
            ofPixels* pixs = new ofPixels();
            bool loaded = ofLoadImage(*pixs, dir.getPath(i));
            
            if(!loaded) {
                ofLogError() << "* Failed to load image: " << dir.getPath(i);
                continue;
            }
            
            if(resizeWidth > 0) pixs->resize(resizeWidth, resizeHeight);
            ofLogVerbose() << "Pixels: " << dir.getPath(i);
            
            // push fbos to vector array
            assetsAndLabel->imageFrames.push_back(pixs);
        }
	}
    
	// save meta data
    assetsAndLabel->frameLabel = (frameLabel == "") ? folderPath : frameLabel;
    assetsAndLabel->folderPath = folderPath;
    //assetsAndLabel->dir = dir;
    assetsAndLabel->resizeHeight = resizeHeight;
    assetsAndLabel->resizeWidth = resizeWidth;
    assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
    assetsAndLabel->complete = true;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();
}

//--------------------------------------------------------------
template<typename ImageType>
void ofxImageSequenceLoader<ImageType>::resizeSequence(string folderPath, string folderOutputPath, int resizeWidth, int resizeHeight) {

	ofDirectory dir;
	int numFiles = dir.listDir(folderPath);
	dir.sort();

	int saveCount = 0;
	for (int i = 0; i < numFiles; i++) {

		string fileExtension = ofToUpper(dir.getFile(i).getExtension());
		
		if (fileExtension == "JPG" || fileExtension == "PNG") {

			// load and allocate memory for images            
			ofPixels* pixs = new ofPixels();
			bool loaded = ofLoadImage(*pixs, dir.getPath(i));

			if (!loaded) {
				ofLogError() << "* Failed to load image: " << dir.getPath(i);
				continue;
			}

			if (resizeWidth > 0) pixs->resize(resizeWidth, resizeHeight);

			string outputPath = folderOutputPath + "/" + dir.getName(i);
			ofLogVerbose() << ((i+1/numFiles)) << "Saving resized image to : " << outputPath;

			//ofImageQualityType format = (fileExtension == "JPG") ? OF_IMAGE_FORMAT_JPEG : OF_IMAGE_FORMAT_PNG;
			//void ofSaveImage(ofPixels & pix, string path, ofImageQualityType qualityLevel = OF_IMAGE_QUALITY_BEST);

			ofSaveImage(*pixs, outputPath, OF_IMAGE_QUALITY_BEST);
			saveCount++;
		}
	}

	ofLog() << (saveCount) << " images resized to: " << folderOutputPath;
}

//--------------------------------------------------------------
template<typename ImageType>
int ofxImageSequenceLoader<ImageType>::getAssetsId(string frameLabel) {
    
	for(int i=0; i < assetCollectionSize; i++) {
		if(assetCollections[i]->frameLabel == frameLabel) {
			return i;
		}
	}

	return -1;
}



template class ofxImageSequenceLoader<ofTexture>;
template class ofxImageSequenceLoader<ofPixels>;
