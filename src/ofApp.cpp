#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //get time
    previousMonth = -1;
    currentMonth = (ofGetMinutes() % 12) + 1;//ofGetMonth();
    previousDay = -1;
    currentDay = ofGetDay(); //1-31

    ofSetFullscreen(true);  // Set the application to fullscreen mode
    ofHideCursor(); //Hide the cursor

    // Allocate the FBO with the same resolution as the window
    fbo.allocate(ofGetWidth(), ofGetHeight());

    // Initially load the first video
    updateVideo(currentMonth);

    dotSize = 1.0;

    // Load shader
    shader.load("filter.vert", "filter.frag");


    //get the seasonal colors
    colors.resize(20);
    palette.resize(10);
    updateColors(currentMonth);

    //init palette
    for (int i = 0; i < 10; ++i) {
        int randomIndex = static_cast<int>(ofRandom(0, colors.size()));
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
    currentMonth = (ofGetMinutes() % 12) + 1;//ofGetMonth();
    currentDay = (ofGetSeconds() % 20); //ofGetDay();

    //update palette
    if (ofGetSeconds() % 30 == 0) {
        previousDay = currentDay;
        updatePalette(currentMonth);
    }

    //update video
    if (currentMonth != previousMonth) {
        previousMonth = currentMonth;
        updateVideo(currentMonth);
        updateColors(currentMonth);
    }

    // Update the movie frame
    movie.update();

    // Clear the FBO
    fbo.begin();
    ofClear(0); // Clear the FBO before drawing

    // Draw video into the FBO
    //movie.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    //movie.draw(0, 0, fbo.getWidth(), fbo.getHeight());
    movie.draw(0, 0, 1154, 1404);

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
    if (key == 't' || key == 'T') {
        ofToggleFullscreen();  // Toggle fullscreen mode
    } else if (key == '1') {
        dotSize = 1.0;
    } else if (key == '2') {
        dotSize = 2.0;
    } else if (key == '3') {
        dotSize = 4.0;
    } else if (key == '4') {
        dotSize = 8.0;
    } else if (key == 'w') {
        updateColors(1);
    } else if (key == 's') {
        updateColors(4);
    } else if (key == 'm') {
        updateColors(7);
    } else if (key == 'f') {
        updateColors(10);
    }
}

//--------------------------------------------------------------
void ofApp::loadVideo(const std::string& fileName){
    if (currentVideoFile != fileName) {  // Load the new video only if it's different from the current one
        movie.stop();  // Stop the current video
        movie.close();  // Close the current video to free resources
        currentVideoFile = fileName;  // Update the current video file name
        movie.load(fileName);  // Load the new video
        movie.setLoopState(OF_LOOP_NORMAL);  // Set the video to loop
        movie.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);  // Set nearest neighbor interpolation
        movie.play();  // Start playing the new video
    }
}

void ofApp::updateVideo(int m) {
    if (m == 12) {
        loadVideo("F.mov");
    } else if (m == 1) {
        loadVideo("A2.mov");
    } else if (m == 2) {
        loadVideo("A.mov");
    } else if (m == 3) {
        loadVideo("C.mov");
    } else if (m == 4) {
        loadVideo("G.mov");
    } else if (m == 5) {
        loadVideo("C.mov");
    } else if (m == 6) {
        loadVideo("B.mov");
    } else if (m == 7) {
        loadVideo("H.mov");
    } else if (m == 8) {
        loadVideo("B.mov");
    } else if (m == 9) {
        loadVideo("D.mov");
    } else if (m == 10) {
        loadVideo("E.mov");
    } else if (m == 11) {
        loadVideo("D.mov");
    }
}

void ofApp::updateColors(int pIdx) {
    if (pIdx == 12 || pIdx == 1 || pIdx == 2) { //WINTER
        colors[0] = ofVec3f(0.96, 0.57, 0.45);
        colors[1] = ofVec3f(0.97, 0.68, 0.59);
        colors[2] = ofVec3f(0.98, 0.80, 0.72);
        colors[3] = ofVec3f(0.98, 0.92, 0.84);
        colors[4] = ofVec3f(0.64, 0.64, 0.68);
        colors[5] = ofVec3f(0.74, 0.74, 0.78);
        colors[6] = ofVec3f(0.84, 0.84, 0.88);
        colors[7] = ofVec3f(0.21, 0.45, 0.74);
        colors[8] = ofVec3f(0.35, 0.55, 0.78);
        colors[9] = ofVec3f(0.49, 0.64, 0.82);
        colors[10] = ofVec3f(0.63, 0.70, 0.86);
        colors[11] = ofVec3f(0.76, 0.80, 0.90);
        colors[12] = ofVec3f(0.00, 0.53, 0.80);
        colors[13] = ofVec3f(0.16, 0.61, 0.84);
        colors[14] = ofVec3f(0.31, 0.72, 0.92);
        colors[15] = ofVec3f(0.49, 0.80, 0.94);
        colors[16] = ofVec3f(0.61, 0.86, 0.94);
        colors[17] = ofVec3f(0.61, 0.61, 0.70);
        colors[18] = ofVec3f(0.72, 0.72, 0.82);
        colors[19] = ofVec3f(0.84, 0.84, 0.94);
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
        colors[0] = ofVec3f(0.95, 0.53, 0.00);
        colors[1] = ofVec3f(0.96, 0.63, 0.20);
        colors[2] = ofVec3f(0.97, 0.72, 0.40);
        colors[3] = ofVec3f(0.98, 0.81, 0.60);
        colors[4] = ofVec3f(0.95, 0.48, 0.32);
        colors[5] = ofVec3f(0.96, 0.59, 0.45);
        colors[6] = ofVec3f(0.97, 0.69, 0.59);
        colors[7] = ofVec3f(0.98, 0.79, 0.72);
        colors[8] = ofVec3f(0.04, 0.54, 0.14);
        colors[9] = ofVec3f(0.23, 0.63, 0.31);
        colors[10] = ofVec3f(0.42, 0.72, 0.48);
        colors[11] = ofVec3f(0.65, 0.83, 0.69);
        colors[12] = ofVec3f(0.00, 0.30, 0.07);
        colors[13] = ofVec3f(0.20, 0.45, 0.25);
        colors[14] = ofVec3f(0.40, 0.58, 0.44);
        colors[15] = ofVec3f(0.64, 0.75, 0.66);
        colors[16] = ofVec3f(0.66, 0.27, 0.20);
        colors[17] = ofVec3f(0.76, 0.43, 0.35);
        colors[18] = ofVec3f(0.86, 0.59, 0.51);
        colors[19] = ofVec3f(0.96, 0.74, 0.66);
    }
}

void ofApp::updatePalette(int n) {
    // Generate a random seed with ofRandom
    //unsigned seed = static_cast<unsigned>(ofRandom(0, std::numeric_limits<unsigned>::max()));
    
    //std::shuffle(colors.begin(), colors.end(), std::default_random_engine(seed));

    for (int i = 0; i < 10; ++i) {
        int randomIndex = static_cast<int>(ofRandom(0, colors.size()));
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
