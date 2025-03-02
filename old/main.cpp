#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(960, 1170, OF_WINDOW);			// <-------- setup the GL context

	ofGLFWWindowSettings settings;
	settings.setSize(2304/2, 2808/2);  // Set the initial window size (this will be overridden by fullscreen)

	settings.windowMode = OF_WINDOW;//OF_FULLSCREEN;  // Set the window mode to fullscreen
	settings.setGLVersion(3, 2);
	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
