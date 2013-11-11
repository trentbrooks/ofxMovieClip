ofxMovieClip & ofxImageSequenceLoader are a set of helper classes for loading and controlling image sequences. These were created to add simple timeline control similar to Flash ( play(), stop(), gotoAndPlay(), etc ).

Now templated with ofxTextureMovieClip/ofxTextureImageSequence for ofTexture's, ofxFboMovieClip/ofxFboImageSequence for ofFbo's, and ofxPixelsMovieClip/ofxPixelsImageSequence for ofPixels. ofPixels are good for very large image sequences, stored on disk instead of graphics card.

## Credit ##
ofxImageSequenceLoader is similar to and inspired by FlightPhase's http://www.flightphase.com ofxImageSequence.

## Sample usage ##
	// setup image sequencer
	ofxTextureImageSequenceLoader imageSequence;

	// load a folder of images as image sequence -optional 2nd paramter is frame label, if empty it defaults to folder name. eg "frogs"
	imageSequence.loadAndCreateSequence("frogs");
	imageSequence.loadAndCreateSequence("horses");

	// initialise a movie clip, passing a reference to the ofxImageSequenceLoader, and desired frame speed/rate for the image sequence
	// 2nd parameter (frameSpeed): 1.0f or 60.0f / ofGetFrameRate() plays back at apps frame rate, 0.5f or or 30.0f / ofGetFrameRate() plays back at half speed, 2.0f or 120.0 / ofGetFrameRate() plays back twice as fast as app frame rate
	ofxTextureMovieClip mc;
	mc.init(&imageSequence, 30.0f / ofGetFrameRate()); 

	// control the movie clip timeline from keyboard events/mouse events etc.
	mc.gotoAndPlay("horses");

	// in your main draw loop
	mc.draw(0, 0);

Trent