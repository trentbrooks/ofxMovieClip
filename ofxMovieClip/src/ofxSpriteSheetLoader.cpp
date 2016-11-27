#include "ofxSpriteSheetLoader.h"

//--------------------------------------------------------------

ofxSpriteSheetLoader::ofxSpriteSheetLoader() {
    assetCollectionSize = 0;
}

ofxSpriteSheetLoader::~ofxSpriteSheetLoader() {
    
    for (int i = 0; i < assetCollections.size(); i++) {
        for (int j = 0; j < assetCollections[i]->imageFrames.size(); j++) {
            delete assetCollections[i]->imageFrames[j];
        }
        
        assetCollections[i]->imageFrames.clear();
    }
    
    assetCollections.clear();
}


//--------------------------------------------------------------
void ofxSpriteSheetLoader::loadSpriteSheet(string imagePath, int sliceWidth, int sliceHeight, int numImages, string frameLabel) {
    
    // setup texture
    ofTexture* texture = new ofTexture();// makeTextureFromImage(&loader);
    bool loaded = ofLoadImage(*texture, imagePath);
    if(!loaded) {
        ofLogError() << "* Failed to load image: " << imagePath;
        return;
    }
    
    SpriteSheetData* assetsAndLabel = new SpriteSheetData();
    assetsAndLabel->imageFrames.push_back(texture);
    
    // save meta data
    assetsAndLabel->frameLabel = (frameLabel == "") ? imagePath : frameLabel;
    assetsAndLabel->imageFramesSize = assetsAndLabel->imageFrames.size();
    assetsAndLabel->sliceWidth = sliceWidth;
    assetsAndLabel->sliceHeight = sliceHeight;
    assetsAndLabel->complete = true;
    
    // cache all the gridpositions (texture subsection draws)
    int rowCount = 0, colCount = 0;
    for(int i = 0; i < numImages; i++) {
        ofPoint texturePos(colCount*sliceWidth, rowCount*sliceHeight);
        if(texturePos.x + sliceWidth > texture->getWidth()) {
            rowCount++;
            colCount = 0;
            texturePos.set(colCount*sliceWidth, rowCount*sliceHeight);
        }
        colCount++;
        ofLog() << texturePos;
        assetsAndLabel->gridPositions.push_back(texturePos);
    }
    assetsAndLabel->gridPositionsSize = assetsAndLabel->gridPositions.size();
    
    // push assets to collections
    assetCollections.push_back(assetsAndLabel);
    assetCollectionSize = assetCollections.size();
    
}



//--------------------------------------------------------------
int ofxSpriteSheetLoader::getAssetsId(string frameLabel) {
    
	for(int i=0; i < assetCollectionSize; i++) {
		if(assetCollections[i]->frameLabel == frameLabel) {
			return i;
		}
	}

	return -1;
}



