ofxMovieClip & ofxImageSequenceLoader are a set of helper classes for loading and controlling image sequences. These were created to add simple timeline control similar to Flash ( play(), stop(), gotoAndPlay(), etc ).

Now templated with ofxTextureMovieClip/ofxTextureImageSequence for ofTexture's, ofxFboMovieClip/ofxFboImageSequence for ofFbo's, and ofxPixelsMovieClip/ofxPixelsImageSequence for ofPixels. ofPixels are good for very large image sequences, stored on disk instead of graphics card.

## Credit ##
ofxImageSequenceLoader is just a modified version of FlightPhase's http://www.flightphase.com ofxImageSequence.

## Sample usage ##
	// setup image sequencer
	ofxTextureImageSequenceLoader imageSequence;

	// load a folder of images as image sequence -optional 2nd paramter is frame label, if empty it defaults to folder name. eg "frogs"
	imageSequence.loadAndCreateSequence("frogs");
	imageSequence.loadAndCreateSequence("horses");

	// create a movieclip and pass in a reference to the ofxImageSequenceLoader & desired frame delay in seconds
	ofxTextureMovieClip mc;
	mc.init(&imageSequence, 1.0/30.0f); 

	// control the movie clip timeline from keyboard events/mouse events etc.
	mc.gotoAndPlay("horses");

	// in your main draw loop
	mc.draw();

Trent