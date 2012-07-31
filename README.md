ofxFBOMovieClip & ofxFBOImageSequenceLoader are a set of helper classes for loading and controlling image sequences as FBO textures. These were created to add simple timeline control similar to Flash ( play(), stop(), gotoAndPlay(), etc ). Works with Openframeworks 007.

## Credit ##
ofxFBOImageSequenceLoader is similar to and inspired by FlightPhase's http://www.flightphase.com ofxImageSequence.

## Sample usage ##
	// setup image sequencer
	ofxFBOImageSequenceLoader fis;

	// load a folder of images as image sequence -optional 2nd paramter is frame label, if empty it defaults to folder name. eg "frogs"
	fis.loadAndCreateSequence("frogs");
	fis.loadAndCreateSequence("horses");

	// initialise a movie clip, passing a reference to the ofxFBOImageSequenceLoader, and desired frame rate for the image sequence
	ofxFBOMovieClip mc;
	mc.init(&fis, 30.0f);

	// control the movie clip timeline from keyboard events/mouse events etc.
	mc.gotoAndPlay("horses");

	// in your main draw loop
	mc.drawFrame(0, 0);

Trent