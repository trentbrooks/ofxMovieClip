#include "ofxFBOImageSequenceLoader.h"

//--------------------------------------------------------------
ofxFBOImageSequenceLoader::ofxFBOImageSequenceLoader(){

}


//--------------------------------------------------------------
void ofxFBOImageSequenceLoader::loadAndCreateSequence(string folderPath, string frameLabel)
{
    ofDirectory dir;
    int numFiles = dir.listDir(folderPath);
    dir.sort();

    ofxFBOAssetsAndLabel* assetsAndLabel = new ofxFBOAssetsAndLabel();
    
    //vector<ofTexture*> newAssets;
    
    ofImage	loader;

    // semi-transparent images don't seem to draw properly when ofEnableAlphaBlending is on?
    // so I will disable if it's on and then re-enable after fbos created.
    int isBlendingOn = ofGetStyle().blendingMode;
    if(isBlendingOn == 1) ofDisableAlphaBlending();

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
            //ofSetColor(255);
            loader.draw(0, 0);
            fbo->end();

            // clear image data from memory?
            loader.clear();

            // push fbos to vector array
            //newAssets.push_back(fbo);
            assetsAndLabel->imageFrames.push_back(fbo);
            assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
        }
	}

    // reset alpha blending
    if(isBlendingOn == 1) ofEnableAlphaBlending();

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


void ofxFBOImageSequenceLoader::loadAndCreateSequence(string frameLabel, int nImages, string filenamePrefix, string filetype, int numDigits, int startFrom)
{

	//vector<ofFbo*> newAssets;
    ofxFBOAssetsAndLabel* assetsAndLabel = new ofxFBOAssetsAndLabel();
    
    ofImage	loader;
    
    // semi-transparent images don't seem to draw properly when ofEnableAlphaBlending is on?
    // so I will disable if it's on and then re-enable after fbos created.
    int isBlendingOn = ofGetStyle().blendingMode;
    if(isBlendingOn == 1) ofDisableAlphaBlending();

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
		//newAssets.push_back(fbo);
        assetsAndLabel->imageFrames.push_back(fbo);
        assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();



	}

    // reset alpha blending
    if(isBlendingOn == 1) ofEnableAlphaBlending();
    
	// push assets to collections
	//assetCollections.push_back(newAssets);

	// push frame labes to vector array
	//assetFrameLabels.push_back(frameLabel);
    
    assetsAndLabel->frameLabel = frameLabel;
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();

}



//--------------------------------------------------------------
int ofxFBOImageSequenceLoader::getAssetsId(string frameLabel)
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


