
#pragma once

#include "ofMain.h"
#include "ofxImageSequenceLoader.h"

/*
 ofxSpriteSheetLoader
 - loads a single image as ofTexture or ofPixels
 - declare as ofxTextureSpriteSheetLoader, or ofxPixelSpriteSheetLoader (typedefs at bottom)
 */

struct SpriteSheetData : MovieClipData<ofTexture> {
    
    // all properties from MovieClipData are inherited
    
    int sliceWidth, sliceHeight;
    vector<ofPoint> gridPositions;
    int gridPositionsSize;
};


// ofTexture only
class ofxSpriteSheetLoader {
    
    public:
        ofxSpriteSheetLoader();
        ~ofxSpriteSheetLoader();
    
        void loadSpriteSheet(string imagePath, int sliceWidth, int slideHeight, int numImages, string frameLabel = "");
    
        vector<SpriteSheetData*> assetCollections;
        int assetCollectionSize;        
        int getAssetsId(string frameLabel);
    
};


