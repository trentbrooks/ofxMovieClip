#include "ofxFBOImageSequenceLoader.h"

//--------------------------------------------------------------
ofxFBOImageSequenceLoader::ofxFBOImageSequenceLoader(){

}


//--------------------------------------------------------------
void ofxFBOImageSequenceLoader::loadAndCreateSequence(string folderPath, string frameLabel) 
{
    ofDirectory dir;
    dir.sort();
    int numFiles = dir.listDir(folderPath);
    
    vector<ofFbo*> newAssets;
	
	for(int i=0; i < numFiles; i++){
		
        string fileExtension = ofToUpper(dir.getFile(i).getExtension());
        if(fileExtension == "JPG" || fileExtension == "PNG") {
            
            // load and allocate memory for images
            loader.loadImage(dir.getPath(i));
            cout << dir.getPath(i) << endl;
            
            // setup new fbo
            ofFbo* fbo = new ofFbo();
            fbo->allocate(loader.getWidth(), loader.getHeight());
            
            // draw to fbo once
            fbo->begin();
            ofClear(0,0,0,0);
            loader.draw(0, 0, loader.getWidth(), loader.getHeight());		
            fbo->end();
            
            // clear image data from memory?
            loader.clear();
            
            // push fbos to vector array
            newAssets.push_back(fbo);
        }
	}
    
	// push assets to collections
	assetCollections.push_back(newAssets);
	
	// push frame labes to vector array
    string newFrameLabel = (frameLabel == "") ? folderPath : frameLabel;
	assetFrameLabels.push_back(newFrameLabel);	
}


void ofxFBOImageSequenceLoader::loadAndCreateSequence(string frameLabel, int nImages, string filenamePrefix, string filetype, int numDigits, int startFrom)
{

	
	vector<ofFbo*> newAssets;
	
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
		
		// setup new fbo
        ofFbo* fbo = new ofFbo();
		fbo->allocate(loader.getWidth(), loader.getHeight());
		
		// draw to fbo once
		fbo->begin();
		ofClear(0,0,0,0);
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
			//assetCollections[i][j]->clear();		
			delete assetCollections[i][j];
		}
	}
	
}
