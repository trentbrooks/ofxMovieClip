#include "ofxTextureImageSequenceLoader.h"

//--------------------------------------------------------------
ofxTextureImageSequenceLoader::ofxTextureImageSequenceLoader(){
    assetCollectionSize = 0;
}


//--------------------------------------------------------------
void ofxTextureImageSequenceLoader::loadAndCreateSequence(string folderPath, string frameLabel, int resizeWidth, int resizeHeight)
{
    ofDirectory dir;
    int numFiles = dir.listDir(folderPath);
    dir.sort();

    ofxTextureAssetsAndLabel* assetsAndLabel = new ofxTextureAssetsAndLabel();
    
    //vector<ofTexture*> newAssets;
    
    ofImage	loader;

	for(int i=0; i < numFiles; i++){

        string fileExtension = ofToUpper(dir.getFile(i).getExtension());
        if(fileExtension == "JPG" || fileExtension == "PNG") {
            
            // load and allocate memory for images
            loader.loadImage(dir.getPath(i));
            if(resizeWidth > 0) loader.resize(resizeWidth, resizeHeight);
            cout << dir.getPath(i) << endl;
            
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

	// push assets to collections
	//assetCollections.push_back(newAssets);
    //cout << newAssets.size() << endl;

	// push frame labes to vector array
    string newFrameLabel = (frameLabel == "") ? folderPath : frameLabel;
	//assetFrameLabels.push_back(newFrameLabel);
    assetsAndLabel->frameLabel = newFrameLabel;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();
}


void ofxTextureImageSequenceLoader::loadAndCreateSequence(string frameLabel, int nImages, string filenamePrefix, string filetype, int numDigits, int startFrom, int resizeWidth, int resizeHeight)
{

	//vector<ofTexture*> newAssets;
    ofxTextureAssetsAndLabel* assetsAndLabel = new ofxTextureAssetsAndLabel();
    
    ofImage	loader;

	for(int i=startFrom; i < nImages; i++){

		// format string to load image sequence - note images must start at 0 not 1.
		stringstream format;
		char imagename[1024];
		if(numDigits != 0){
			format <<filenamePrefix<<"%0"<<numDigits<<"d."<<filetype;
		} else{
			format <<filenamePrefix<<"%d."<<filetype;
		}
		sprintf(imagename, format.str().c_str(), i);
        cout << imagename << endl;

		// load and allocate memory for images
		loader.loadImage(string(imagename));

        // setup texture
        ofTexture* texture = makeTextureFromImage(&loader);

		// push texture to vector array
		//newAssets.push_back(texture);
        assetsAndLabel->imageFrames.push_back(texture);
        assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
        
        // clear image data from memory?
		loader.clear();



	}

	// push assets to collections
	//assetCollections.push_back(newAssets);

	// push frame labes to vector array
	//assetFrameLabels.push_back(frameLabel);
    assetsAndLabel->frameLabel = frameLabel;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();

}

ofTexture* ofxTextureImageSequenceLoader::makeTextureFromImage(ofImage *img, bool useARB){
    
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
int ofxTextureImageSequenceLoader::getAssetsId(string frameLabel)
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

