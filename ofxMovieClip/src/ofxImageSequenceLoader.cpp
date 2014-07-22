#include "ofxImageSequenceLoader.h"

//--------------------------------------------------------------
template <typename ImageType>
ofxImageSequenceLoader<ImageType>::ofxImageSequenceLoader(){
    assetCollectionSize = 0;
}

template <typename ImageType>
ofxImageSequenceLoader<ImageType>::~ofxImageSequenceLoader()
{
    
    for (int i = 0; i < assetCollections.size(); i++)
    {
        for (int j = 0; j < assetCollections[i]->imageFrames.size(); j++)
		{
            delete assetCollections[i]->imageFrames[j];
        }
        
        assetCollections[i]->imageFrames.clear();
        
    }
    
    assetCollections.clear();
}
 
//--------------------------------------------------------------
// template specialisation: ofTexture
template<>
void ofxImageSequenceLoader<ofTexture>::loadAndCreateSequence(string folderPath, string frameLabel, int resizeWidth, int resizeHeight)
{
    ofDirectory dir;
    int numFiles = dir.listDir(folderPath);
    dir.sort();
    
    MovieClipAssetsAndLabel<ofTexture>* assetsAndLabel = new MovieClipAssetsAndLabel<ofTexture>();
        
    ofImage	loader;
    
	for(int i=0; i < numFiles; i++){
        
        string fileExtension = ofToUpper(dir.getFile(i).getExtension());
        if(fileExtension == "JPG" || fileExtension == "PNG") {
            
            // load and allocate memory for images
            loader.loadImage(dir.getPath(i));
            if(resizeWidth > 0) loader.resize(resizeWidth, resizeHeight);
            cout << "Texture: " << dir.getPath(i) << endl;
            
            // setup texture
            ofTexture* texture = makeTextureFromImage(&loader);
            
            // push texture to vector array
            //newAssets.push_back(texture);
            assetsAndLabel->imageFrames.push_back(texture);
            assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
            
            // clear image data from memory?
            loader.clear();
        }
        
    }
    
	// push frame labes to vector array
    string newFrameLabel = (frameLabel == "") ? folderPath : frameLabel;
	//assetFrameLabels.push_back(newFrameLabel);
    assetsAndLabel->frameLabel = newFrameLabel;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();
}


// template specialisation: ofPixels
template<>
void ofxImageSequenceLoader<ofPixels>::loadAndCreateSequence(string folderPath, string frameLabel, int resizeWidth, int resizeHeight)
{
    ofDirectory dir;
    int numFiles = dir.listDir(folderPath);
    dir.sort();
    
    MovieClipAssetsAndLabel<ofPixels>* assetsAndLabel = new MovieClipAssetsAndLabel<ofPixels>();
        
	for(int i=0; i < numFiles; i++){
        
        string fileExtension = ofToUpper(dir.getFile(i).getExtension());
        if(fileExtension == "JPG" || fileExtension == "PNG") {
            
            // load and allocate memory for images            
            ofPixels* pixs = new ofPixels();
            ofLoadImage(*pixs, dir.getPath(i));
            if(resizeWidth > 0) pixs->resize(resizeWidth, resizeHeight);
            cout << "PIXELS: " << dir.getPath(i) << endl;
            
            // push fbos to vector array
            //newAssets.push_back(fbo);
            assetsAndLabel->imageFrames.push_back(pixs);
            assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
        }
	}
    
	// push frame labes to vector array
    string newFrameLabel = (frameLabel == "") ? folderPath : frameLabel;
	//assetFrameLabels.push_back(newFrameLabel);
    assetsAndLabel->frameLabel = newFrameLabel;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();
}

template<typename ImageType>
ofTexture* ofxImageSequenceLoader<ImageType>::makeTextureFromImage(ofImage *img, bool useARB){
    
	ofTexture* t = new ofTexture();
    if(img->type == OF_IMAGE_COLOR_ALPHA){
        // alpha image
        t->allocate(img->width, img->height, GL_RGBA, useARB);
        t->loadData(img->getPixels(),img->width, img->height, GL_RGBA);
	}else if(img->type == OF_IMAGE_COLOR){
        // rgb image
		t->allocate(img->width, img->height, GL_RGB, useARB);
        t->loadData(img->getPixels(),img->width, img->height, GL_RGB);
	} else {
        // greyscale image
        t->allocate(img->width, img->height, GL_LUMINANCE, false);
        t->loadData(img->getPixels(),img->width, img->height, GL_LUMINANCE);
    }
    
	return t;
    
}


//--------------------------------------------------------------
template<typename ImageType>
int ofxImageSequenceLoader<ImageType>::getAssetsId(string frameLabel)
{
	for(int i=0; i < assetCollectionSize; i++)
	{
		if(assetCollections[i]->frameLabel == frameLabel)
		{
			return i;
		}
	}


	return 0;
}


template class ofxImageSequenceLoader<ofTexture>;
template class ofxImageSequenceLoader<ofPixels>;
