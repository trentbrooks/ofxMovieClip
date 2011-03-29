#include "ofxFBOImageSequenceLoader.h"

//--------------------------------------------------------------
ofxFBOImageSequenceLoader::ofxFBOImageSequenceLoader(){

}


//--------------------------------------------------------------
void ofxFBOImageSequenceLoader::loadAndCreateSequence(string frameLabel, int nImages, string filenamePrefix, string filetype, int numDigits){

	
	vector<ofxFBOTexture*> newAssets;
	
	for(int i=0; i < nImages; i++){
		
		// format string to load image sequence - note images must start at 0 not 1.
		stringstream format;
		char imagename[1024];				
		if(numDigits != 0){
			format <<filenamePrefix<<"%0"<<numDigits<<"d."<<filetype;
		} else{
			format <<filenamePrefix<<"%d."<<filetype; 
		}		
		sprintf(imagename, format.str().c_str(), i);
		printf("%s \n", imagename);
		
		// load and allocate memory for images
		loader.loadImage(string(imagename));

		
		
		// setup new fbo
		ofxFBOTexture* fbo = new ofxFBOTexture();
		fbo->allocate(loader.getWidth(), loader.getHeight());
		
		// draw to fbo once
		fbo->begin();
		loader.draw(0, 0, loader.getWidth(), loader.getHeight());		
		fbo->end();
		
		// clear image data from memory?
		loader.clear();
		
		// push fbos to vector array
		newAssets.push_back(fbo);
		
		
		
	}

	// push assets to collections
	assetCollections.push_back(newAssets);
	
	// push frame labes to vector array
	assetFrameLabels.push_back(frameLabel);	
	
}


//--------------------------------------------------------------
int ofxFBOImageSequenceLoader::getAssetsId(string frameLabel)
{
	for(int i=0; i < assetFrameLabels.size(); i++)
	{
		if(assetFrameLabels[i] == frameLabel)
		{
			return i;
		}
	}
	 
	
	return 0;
}


//--------------------------------------------------------------
void ofxFBOImageSequenceLoader::dispose()
{
	loader.clear();

	int assetsSize = assetCollections.size();
	for(int i=0; i < assetsSize; i++){
		int imagesSize = assetCollections[i].size();
		for(int j=0; j < imagesSize; j++){
			assetCollections[i][j]->clear();		
			delete assetCollections[i][j];
		}
	}
	
}
