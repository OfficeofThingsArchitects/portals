#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //get time
    previousMonth = -1;
    currentMonth = ofGetMonth();
    previousDay = -1;
    currentDay = ofGetDay(); //1-31
    currentMinute = ofGetMinutes();
    previousMinute = -1;

    ofSetFullscreen(true);  // Set the application to fullscreen mode
    ofHideCursor(); //Hide the cursor

    // Allocate the FBO with the same resolution as the window
    fbo.allocate(ofGetWidth(), ofGetHeight());

    // Initially load the first video
    updateVideo(currentMonth);

    dotSize = 1.0;

    //demo mode
    demo = false;

    // Load shader
    shader.load("filter.vert", "filter.frag");


    //get the seasonal colors
    colors.resize(20);
    palette.resize(10);
    updateColors(currentMonth);

    //init palette
    for (int i = 0; i < 10; ++i) {
        int randomIndex = static_cast<int>(floor( ofRandom(0, colors.size()) ) );
        palette[i] = colors[randomIndex];
    }

    // palette[0] = ofVec3f(0.95,0.49,0.31);
    // palette[1] = ofVec3f(0.97,0.69,0.59);
    // palette[2] = ofVec3f(0.64,0.64,0.68);
    // palette[3] = ofVec3f(0.84,0.84,0.88);
    // palette[4] = ofVec3f(0.35,0.55,0.78);
    // palette[5] = ofVec3f(0.63,0.70,0.86);
    // palette[6] = ofVec3f(0.16,0.61,0.84);
    // palette[7] = ofVec3f(0.49,0.80,0.94);
    // palette[8] = ofVec3f(0.49,0.49,0.59);
    // palette[9] = ofVec3f(0.72,0.72,0.82);

}

//--------------------------------------------------------------
void ofApp::update(){
    //update time
    // demoMonth = (ofGetMinutes() % 12) + 1;//ofGetMonth();
    // demoDay = (ofGetSeconds() % 20); //ofGetDay();

    //currentMonth = ofGetMonth();
    //currentDay = ofGetDay();

    static bool lastDemoMode = demo; // Track the previous demo mode

    // Update time based on mode
    if (!demo) {
        currentMonth = ofGetMonth();
        currentDay = ofGetDay();
    } else {
        currentMonth = (ofGetMinutes() % 12) + 1;
        currentDay = (ofGetSeconds() % 30);
    }

    // Detect mode change and force updates
    if (demo != lastDemoMode) {
        lastDemoMode = demo;
        updatePalette(currentDay);
        //updateVideo(currentMonth);
        updateColors(currentMonth); // Ensure colors update on mode switch
    }

    //update palette
    if (currentDay != previousDay) {
        previousDay = currentDay;
        //updateVideo(currentMonth);
        updatePalette(currentDay);
    }

    //update video
    if (currentMonth != previousMonth) {
        previousMonth = currentMonth;
        updateColors(currentMonth);
    }

    //reset video
    // if (ofGetSeconds() % 60 == 0) {
    //     resetVideo();
    // }

    // Get the current time in minutes
    currentMinute = ofGetMinutes();

    // Check if the minute has changed
    if (currentMinute != previousMinute) {
        previousMinute = currentMinute;  // Update the previous minute

        // Alternate between videos based on whether the minute is even or odd
        if (currentMinute % 2 == 0) {
            loadVideo("AA.mov");
        } else {
            loadVideo("A.mov");
        }
    }

    // Update the movie frame
    movie.update();

    // Clear the FBO
    fbo.begin();
    ofClear(0); // Clear the FBO before drawing

    // Draw video into the FBO
    //movie.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    //movie.draw(0, 0, fbo.getWidth(), fbo.getHeight());
    movie.draw(0, 0, 1152, 1404);

    fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Get the window dimensions
    float windowWidth = ofGetWidth();
    float windowHeight = ofGetHeight();

    // Set nearest neighbor interpolation
    fbo.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

    shader.begin();
    shader.setUniformTexture("tex0", fbo.getTexture(), 1);
    shader.setUniform2f("resolution", windowWidth, windowHeight);
    shader.setUniform1f("dotSize", dotSize);
    shader.setUniform3fv("palette", &palette[0].x, 10); // Pass the array of colors

    fbo.draw(0, 0, windowWidth, windowHeight);
    shader.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f' || key == 'F') {
        ofToggleFullscreen();  // Toggle fullscreen mode
    } else if (key == '1') {
        dotSize = 1.0;
    } else if (key == '2') {
        dotSize = 2.0;
    } else if (key == '3') {
        dotSize = 4.0;
    } else if (key == '4') {
        dotSize = 8.0;
    } else if (key == '9') {
        demo = !demo;
    }
}

//--------------------------------------------------------------
// void ofApp::loadVideo(const std::string& fileName) {
//     if (currentVideoFile != fileName) {  // Load the new video only if it's different from the current one
//         movie.stop();  // Stop the current video
//         movie.close();  // Close the current video to free resources
//         currentVideoFile = fileName;  // Update the current video file name
//         movie.load(fileName);  // Load the new video
//         movie.setLoopState(OF_LOOP_NORMAL);  // Set the video to loop
//         movie.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);  // Set nearest neighbor interpolation
//         movie.play();  // Start playing the new video
//     }
// }

void ofApp::loadVideo(const std::string& fileName) {
    if (currentVideoFile != fileName) {  // Only reload if the video is different
        movie.stop();                  // Stop current playback
        movie.close();                 // Close the current video to release resources

        currentVideoFile = fileName;   // Update the current video file
        if (movie.load(fileName)) {    // Load the new video
            movie.setLoopState(OF_LOOP_NORMAL);  // Set to loop
            movie.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);  // Texture optimization
            movie.play();  // Start playing the video
        } else {
            ofLogError("VideoPlayer") << "Failed to load video: " << fileName;
        }
    }
}

void ofApp::resetVideo() {
        movie.stop();  // Stop the current video
        movie.close();  // Close the current video to free resources
        movie.load(currentVideoFile);  // Load the new video
        movie.setLoopState(OF_LOOP_NORMAL);  // Set the video to loop
        movie.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);  // Set nearest neighbor interpolation
        movie.play();  // Start playing the new video
}

void ofApp::updateVideo(int m) {
    if (m == 12) {
        // Create an array of video filenames
        std::vector<std::string> options = {"A.mov", "D.mov", "F.mov"};
        // Generate a random index to select one of the options
        int randomIndex = (int)floor( ofRandom(0, options.size()) );
        // Load the randomly selected video
        loadVideo(options[randomIndex]);
        //loadVideo("J.mov");//special
    } else if (m == 1) {
        loadVideo("A2.mov");
    } else if (m == 2) {
        loadVideo("A.mov");
    } else if (m == 3) {
        loadVideo("C.mov");
    } else if (m == 4) {
        loadVideo("G.mov");//special
    } else if (m == 5) {
        loadVideo("C.mov");
    } else if (m == 6) {
        loadVideo("B.mov");
    } else if (m == 7) {
        loadVideo("H.mov");//special
    } else if (m == 8) {
        loadVideo("B.mov");
    } else if (m == 9) {
        loadVideo("D.mov");
    } else if (m == 10) {
        loadVideo("E.mov");//special
    } else if (m == 11) {
        loadVideo("D.mov");
    }
}

void ofApp::updateColors(int pIdx) {
    if (pIdx == 12 || pIdx == 1 || pIdx == 2) { //WINTER
        colors[0] = ofVec3f(0.88, 0.57, 0.53);
        colors[1] = ofVec3f(0.97, 0.68, 0.59);
        colors[2] = ofVec3f(0.98, 0.80, 0.72);
        colors[3] = ofVec3f(0.98, 0.92, 0.84);
        colors[4] = ofVec3f(0.47, 0.39, 0.41);
        colors[5] = ofVec3f(0.63, 0.59, 0.60);
        colors[6] = ofVec3f(0.82, 0.78, 0.79);
        colors[7] = ofVec3f(0.00, 0.20, 0.29);
        colors[8] = ofVec3f(0.09, 0.29, 0.39);
        colors[9] = ofVec3f(0.19, 0.39, 0.49);
        colors[10] = ofVec3f(0.29, 0.49, 0.59);
        colors[11] = ofVec3f(0.39, 0.59, 0.68);
        colors[12] = ofVec3f(0.20, 0.55, 0.71);
        colors[13] = ofVec3f(0.27, 0.63, 0.79);
        colors[14] = ofVec3f(0.49, 0.80, 0.94);
        colors[15] = ofVec3f(0.61, 0.86, 0.96);
        colors[16] = ofVec3f(0.52, 0.64, 0.63);
        colors[17] = ofVec3f(0.60, 0.70, 0.73);
        colors[18] = ofVec3f(0.69, 0.74, 0.78);
        colors[19] = ofVec3f(0.77, 0.80, 0.84);
    } else if (pIdx == 3 || pIdx == 4 || pIdx == 5) { //SPRING
        colors[0] = ofVec3f(0.80, 0.88, 0.55);
        colors[1] = ofVec3f(0.84, 0.92, 0.66);
        colors[2] = ofVec3f(0.88, 0.96, 0.77);
        colors[3] = ofVec3f(0.92, 1.00, 0.88);
        colors[4] = ofVec3f(0.96, 0.57, 0.63);
        colors[5] = ofVec3f(0.98, 0.68, 0.70);
        colors[6] = ofVec3f(0.99, 0.80, 0.78);
        colors[7] = ofVec3f(0.98, 0.86, 0.45);
        colors[8] = ofVec3f(0.98, 0.90, 0.59);
        colors[9] = ofVec3f(0.99, 0.94, 0.72);
        colors[10] = ofVec3f(0.99, 0.98, 0.84);
        colors[11] = ofVec3f(0.94, 0.72, 0.95);
        colors[12] = ofVec3f(0.96, 0.80, 0.97);
        colors[13] = ofVec3f(0.98, 0.92, 0.98);
        colors[14] = ofVec3f(0.96, 0.49, 0.63);
        colors[15] = ofVec3f(0.98, 0.61, 0.72);
        colors[16] = ofVec3f(1.00, 0.70, 0.82);
        colors[17] = ofVec3f(0.37, 0.68, 0.84);
        colors[18] = ofVec3f(0.47, 0.78, 0.92);
        colors[19] = ofVec3f(0.57, 0.88, 1.00);
    } else if (pIdx == 6 || pIdx == 7 || pIdx == 8) { //SUMMER
        colors[0] = ofVec3f(0.31, 0.66, 0.88);
        colors[1] = ofVec3f(0.39, 0.72, 0.90);
        colors[2] = ofVec3f(0.47, 0.78, 0.92);
        colors[3] = ofVec3f(0.00, 0.55, 0.63);
        colors[4] = ofVec3f(0.20, 0.70, 0.74);
        colors[5] = ofVec3f(0.39, 0.86, 0.86);
        colors[6] = ofVec3f(0.16, 0.66, 0.33);
        colors[7] = ofVec3f(0.23, 0.74, 0.41);
        colors[8] = ofVec3f(0.31, 0.82, 0.53);
        colors[9] = ofVec3f(0.39, 0.90, 0.64);
        colors[10] = ofVec3f(0.08, 0.59, 0.55);
        colors[11] = ofVec3f(0.16, 0.66, 0.63);
        colors[12] = ofVec3f(0.23, 0.74, 0.70);
        colors[13] = ofVec3f(0.78, 0.43, 0.51);
        colors[14] = ofVec3f(0.88, 0.59, 0.66);
        colors[15] = ofVec3f(0.98, 0.74, 0.82);
        colors[16] = ofVec3f(0.98, 0.74, 0.00);
        colors[17] = ofVec3f(0.98, 0.78, 0.20);
        colors[18] = ofVec3f(0.98, 0.82, 0.39);
        colors[19] = ofVec3f(0.98, 0.86, 0.59);
    } else if (pIdx == 9 || pIdx == 10 || pIdx == 11) { //FALL
        colors[0] = ofVec3f(0.88, 0.53, 0.00);
        colors[1] = ofVec3f(0.92, 0.61, 0.14);
        colors[2] = ofVec3f(0.96, 0.69, 0.27);
        colors[3] = ofVec3f(1.00, 0.77, 0.41);
        colors[4] = ofVec3f(0.82, 0.48, 0.32);
        colors[5] = ofVec3f(0.87, 0.59, 0.45);
        colors[6] = ofVec3f(0.92, 0.69, 0.59);
        colors[7] = ofVec3f(0.97, 0.80, 0.72);
        colors[8] = ofVec3f(0.20, 0.47, 0.18);
        colors[9] = ofVec3f(0.33, 0.57, 0.18);
        colors[10] = ofVec3f(0.47, 0.63, 0.18);
        colors[11] = ofVec3f(0.61, 0.72, 0.18);
        colors[12] = ofVec3f(0.23, 0.29, 0.10);
        colors[13] = ofVec3f(0.31, 0.39, 0.21);
        colors[14] = ofVec3f(0.39, 0.49, 0.33);
        colors[15] = ofVec3f(0.47, 0.59, 0.41);
        colors[16] = ofVec3f(0.41, 0.27, 0.14);
        colors[17] = ofVec3f(0.55, 0.43, 0.25);
        colors[18] = ofVec3f(0.68, 0.59, 0.37);
        colors[19] = ofVec3f(0.82, 0.74, 0.45);
    }
}

void ofApp::updatePalette(int n) {
    // Generate a random seed with ofRandom
    //unsigned seed = static_cast<unsigned>(ofRandom(0, std::numeric_limits<unsigned>::max()));
    
    //std::shuffle(colors.begin(), colors.end(), std::default_random_engine(seed));

    for (int i = 0; i < 10; ++i) {
        int randomIndex = static_cast<int>(floor( ofRandom(0, colors.size()) ));
        palette[i] = colors[randomIndex];
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
