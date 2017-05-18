#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(240, 240, 240);
    ofDisableSmoothing();
    ofSetLineWidth(1.0f);
    
    nBandsToGet = 512;
    filename = "single_ding_silver.wav";
    filenameRect = getBitMapStringBoundingBox(filename);
    ding.load("sounds/" + filename);
    ding.setLoop(true);
    ding.play();
    
    bandWidth = float(ofGetWidth() - margin * 2) / float(nBandsToGet);
    graphHeight = 200.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // update sound
    ofSoundUpdate();
    fft = ofSoundGetSpectrum(nBandsToGet);
    
    position = ding.getPosition();
    positionMS = zeroPad(ding.getPositionMS(), 5);
    
    
    vector<ofPoint> points(nBandsToGet);
    
    float maxValue = -100000;
    
    for (int i = 0; i < nBandsToGet; i++) {
        
        if (maxValue < fft[i]) {
            maxValue = fft[i];
            maxIndex = i;
        }
        
        float x = margin + i * bandWidth;
        float y = margin + 180 + graphHeight * (1 - fft[i]);
        
        points[i] = ofPoint(x,y);
    }
    
    frequencyGraph.clear(); // empty verticies
    frequencyGraph.addVertices(points);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // info
    ofDrawBitmapString(filename, margin, margin);
    ofDrawBitmapString(positionMS, margin + filenameRect.width + 5, margin);
    
    // position
    ofSetColor(10, 10, 10);
    ofFill();
    ofDrawLine(margin, margin + 80, ofGetWidth() - margin, margin + 80);
    ofDrawCircle(margin + float(ofGetWidth() - margin * 2) * position, margin + 80, 3);
    
    // distribution of frequency
    ofSetColor(10, 10, 10);
    ofNoFill();
    frequencyGraph.draw();
    
    // save image
    if (isRecordingFrames) {
        ofSaveScreen(outDir + filename + "_" + positionMS + ".png");
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
    // toggle Record
    isRecordingFrames = !isRecordingFrames;
    
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
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

string ofApp::zeroPad(const int& num, const int& digits) {
    string numString = ofToString(num);
    int digitLeft = digits - numString.size();
    
    string zeros;
    
    for(int i = 0; i < digitLeft; i++) {
        zeros = "0" + zeros;
    }
    
    return zeros + numString;
    
}
