#include "ofApp.h"

using namespace Goodies;

//--------------------------------------------------------------
void ofApp::setup(){
    
    filenameBoundingBox = getBitMapStringBoundingBox(filename);
    
    soundClip.load(ofToDataPath("soundClips/" + filename));
    
    cout << soundClip.getSummary() << endl;
    
    length = soundClip.getLength();
    position = 0;
    bufferSize = 1024;
    sampleRate = 44100;
    
    fft.setup(bufferSize, 512, 256);
    magnitudes = fft.magnitudes; // fft.magsToDB();
    maxIndex = -1;
    needsRecord = false;
    
    lowClampIndex = getIndexFromFreq(1500);
    highClampIndex = getIndexFromFreq(5000);
    targetScopeIndex = getIndexFromFreq(2462);
    neighborCells = 2;
    
    ofSoundStreamSetup(2, 2, this, sampleRate, bufferSize, 4);
    
    auto innerWidth = ofGetWidth() - margin * 2;
    ofBackground(white);
    metaInfo.allocate(innerWidth, 50, GL_RGB);
    wavePlotter.allocate(innerWidth, 202, GL_RGB);
    positionIndicator.allocate(innerWidth, 50, GL_RGB);
    magnitudeTracker.allocate(innerWidth, 200, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float positionParameter = ((float)position) / ((float) length);
    msFromStart = convertPositionToMS(position);
    
    metaInfo.begin();
    ofClear(white);
    ofSetColor(black);
    ofDrawBitmapString(filename, 0, bitmapStringHeight);
    ofDrawBitmapString(zeroPad(msFromStart, 4), filenameBoundingBox.getWidth() + 3, bitmapStringHeight);
    metaInfo.end();
    
    positionIndicator.begin();
    ofClear(white);
    ofSetColor(black);
    ofDrawLine(0,
               positionIndicator.getHeight() * 0.5,
               positionIndicator.getWidth(),
               positionIndicator.getHeight() * 0.5);
    
    ofDrawCircle(positionIndicator.getWidth() * positionParameter,
                 positionIndicator.getHeight() * 0.5,
                 3);
    positionIndicator.end();
    
    
    wavePlotter.begin();
    ofClear(white);
    auto rangeBetween = highClampIndex - lowClampIndex + 1;
    auto points = vector<ofPoint>(rangeBetween);
    for(int i = lowClampIndex; i <= highClampIndex; i++) {
        float x = ofMap(i, lowClampIndex, highClampIndex, 0, wavePlotter.getWidth());
        float y = ofMap(magnitudes[i], 100.0f, 0.0f, 0, wavePlotter.getHeight() - 2);
        if (i % 2 == 0) {
            ofSetColor(ofColor(242));
            ofDrawRectangle(x, 0, wavePlotter.getWidth() / (rangeBetween - 1), wavePlotter.getHeight());
        }
        ofSetColor(grey);
        // ofDrawCircle(x, y, 2);
        
        points[i - lowClampIndex] = ofPoint(x, y);
    }
    ofPolyline(points).draw();
    
    // three points to represent focused slopes
    ofPoint a,b,c;
    
    a = ofPoint(
                ofMap(targetScopeIndex - neighborCells, lowClampIndex, highClampIndex, 0, wavePlotter.getWidth()),
                ofMap(magnitudes[targetScopeIndex - neighborCells], 100.0f, 0.0f, 0, wavePlotter.getHeight() - 2)
                );
    
    b = ofPoint(
                ofMap(targetScopeIndex, lowClampIndex, highClampIndex, 0, wavePlotter.getWidth()),
                ofMap(magnitudes[targetScopeIndex], 100.0f, 0.0f, 0, wavePlotter.getHeight() - 2)
                );
    
    c = ofPoint(
                ofMap(targetScopeIndex + neighborCells, lowClampIndex, highClampIndex, 0, wavePlotter.getWidth()),
                ofMap(magnitudes[targetScopeIndex + neighborCells], 100.0f, 0.0f, 0, wavePlotter.getHeight() - 2)
                );
    
    ofPushStyle();
    ofSetColor(black);
    /*
    ofDrawCircle(a, 3);
    ofDrawCircle(b, 3);
    ofDrawCircle(c, 3);
    */
    drawCrossmark(a, 7);
    drawCrossmark(b, 7);
    drawCrossmark(c, 7);
    
    ofSetLineWidth(2);
    ofDrawLine(a, b);
    ofDrawLine(b, c);
    ofPopStyle();
    
    if (maxIndex > 0) {
        float maxFreq = getFreqFromIndex(maxIndex);
        ofRectangle maxFreqLabel = getBitMapStringBoundingBox(ofToString((maxFreq)));
        float x = ofMap(maxIndex, lowClampIndex, highClampIndex, 0, wavePlotter.getWidth()) - maxFreqLabel.getWidth() * 0.5;
        ofDrawBitmapString(ofToString(maxFreq), x, wavePlotter.getHeight() * 0.25);
    }
    wavePlotter.end();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPushMatrix();
    ofTranslate(margin, margin);
    metaInfo.draw(0, 0);
    ofTranslate(0, metaInfo.getHeight() + 10);
    wavePlotter.draw(0, 0);
    ofTranslate(0, wavePlotter.getHeight() + 10);
    positionIndicator.draw(0, 0);
    ofPopMatrix();
    
    // triggers each time new FFT is processed
    if (needsRecord) {
        ofSaveScreen("out/" + filename + "_" + zeroPad(msFromStart, 5) + ".png");
        needsRecord = false;
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
    
    if(needsRecord) return;
    
    for (int i = 0; i < bufferSize; i++) {
        double sound = soundClip.play();
        position ++;
        if(position >= length) position = 0;
        
        if (fft.process(sound)) {
            magnitudes = fft.magnitudes; //fft.magsToDB();
            float magMax = 1;
            maxIndex = -1;
            for (int j = lowClampIndex; j < highClampIndex; j++) {
                if (magMax < magnitudes[j]) {
                    magMax = magnitudes[j];
                    maxIndex = j;
                }
            }
            needsRecord = true;
        }
        
        /* don't wanna hear
         output[i * nChannels] = sound;
         output[i * nChannels + 1] = sound;
         */
        
    }
}

int ofApp::convertPositionToMS (const int & position, const int & sampleRate) {
    return (int) (((float) position) / ((float) sampleRate) * 1000);
}

float ofApp::getFreqFromIndex (const int & index, const int & sampleRate) {
    return (index + 0.5) * ((float) sampleRate / (float) bufferSize);
}

int ofApp::getIndexFromFreq (const float & frequency, const int & sampleRate) {
    float unitFreq = (float) sampleRate / (float) bufferSize;
    return (int) (frequency / unitFreq);
}
