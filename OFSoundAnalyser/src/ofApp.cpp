#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // sound
    bufferSize = 1024;
    sampleRate = 44100;
    
    soundClip.load(ofToDataPath("soundClips/single_ding_silver.wav"));
    
    
    ofSoundStreamSetup(2, 2, this, sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
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

void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    for (int i = 0; i < bufferSize; i++) {
        double sound = soundClip.play();
        
        output[i * nChannels] = sound;
        output[i * nChannels + 1] = sound;
        
    }
}

/*
float ofApp::getFreqByIndex (const int & index) {
    float sampleRate = 22050;
    return index * (sampleRate / (float) nBandsToGet);
}
*/
