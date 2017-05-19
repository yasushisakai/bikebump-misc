#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(backgroundColor);
    ofSetLineWidth(1.0f);
    
    nBandsToGet = 512;
    filename = "single_ding_silver.wav";
    filenameRect = getBitMapStringBoundingBox(filename);
    ding.load("sounds/" + filename);
    ding.setLoop(true);
    ding.play();
    
    //
    // FBO
    //
    
    int innerWidth = ofGetWidth() - margin * 2;
    metaInfo.allocate(innerWidth, 50, GL_RGB);
    
    currentPosition.allocate(innerWidth, 50, GL_RGB);
    
    frequencyGraph.allocate(ofGetWidth() - margin * 2, 200, GL_RGB);
    bandWidth = float(frequencyGraph.getWidth()) / float(nBandsToGet);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // computation part
    ofSoundUpdate();
    fft = ofSoundGetSpectrum(nBandsToGet);
    
    position = ding.getPosition();
    positionMS = zeroPad(ding.getPositionMS(), 5);
    
    // below, writing on buffers
    metaInfo.begin();
        ofClear(backgroundColor);
        ofSetColor(20, 20, 20);
        ofDrawBitmapString(filename, 0, 10);
        ofDrawBitmapString(positionMS, filenameRect.width + 5, 10);
    metaInfo.end();
    
    currentPosition.begin();
        ofClear(backgroundColor);
        ofSetColor(10, 10, 10);
        ofFill();
        ofDrawLine(0, currentPosition.getHeight() / 2, currentPosition.getWidth(), currentPosition.getHeight() / 2);
        ofDrawCircle(currentPosition.getWidth() * position, currentPosition.getHeight() / 2, 3);
    currentPosition.end();
    
    ofPolyline freqPolyline;
    vector<ofPoint> points(nBandsToGet);
    
    float maxValue = -100000;
    
    for (int i = 0; i < nBandsToGet; i++) {
        
        if (maxValue < fft[i]) {
            maxValue = fft[i];
            maxIndex = i;
        }
        
        float x = i * bandWidth;
        float y = frequencyGraph.getHeight() * (1.0f - (fft[i] * 0.5f));
        
        points[i] = ofPoint(x,y);
    }
    
    freqPolyline.addVertices(points);
    
    frequencyGraph.begin();
        ofClear(backgroundColor);
        ofSetColor(10, 10, 10);
        ofNoFill();
        freqPolyline.draw();
    frequencyGraph.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // info
    metaInfo.draw(margin, 20);
    
    // position
    currentPosition.draw(margin, 50);
    
    // distribution of frequency
    frequencyGraph.draw(margin, 100);
    
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
    ofSetVerticalSync(isRecordingFrames);
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
