#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void loadVideo(const std::string& fileName);  // Function to load a video
		void resetVideo(); //resets current video
		void updateVideo(int m);
		void updateColors(int pIdx); //function to update the palette array
		void updatePalette(int n);
		
    ofVideoPlayer movie;  // Video player for the current video
    std::string currentVideoFile;  // Store the current video file name
		ofFbo fbo;
		ofShader shader; //shader filter
		//ofVec3f colors[20]; // Array to hold 20 colors
		//ofVec3f palette[10]; // Array to hold 10 colors

		std::vector<ofVec3f> colors;
    std::vector<ofVec3f> palette;

		int previousMonth;
		int currentMonth;
		int previousDay;
		int currentDay;
		int currentMinute;
		int previousMinute;
		float lastRestartTime;
		bool demo;
		float dotSize;
		
};
