#include "ofApp.h"

using namespace Goodies;

//--------------------------------------------------------------
void ofApp::setup(){
    
    soundClip.load(ofToDataPath("soundClips/" + filename));
    
    soundInfo = summaryToSoundClipInfo(filename, soundClip.getSummary());
    
    length = soundClip.getLength(); // TODO: have this data inside SoundClipInfo
    position = 0;
    
    fft.setup(bufferSize, 512, 256);
    magnitudes = fft.magnitudes; // fft.magsToDB();
    maxIndex = -1;
    needsRecord = false;
    nFFT = length / bufferSize;
    
    lowClampIndex = getIndexFromFreq(lowClampFreq);
    highClampIndex = getIndexFromFreq(highClampFreq);
    targetScopeRangeCenter = getIndexFromFreq(targetScopeRangeCenterFreq);
    isAboveThreshold = false;
    thresholdRanges  = vector<vector<float>>(0, vector<float>(2));
    
    ofSoundStreamSetup(2, 2, this, soundInfo.sampleRate, bufferSize, 4);
    
    auto innerWidth = ofGetWidth() - margin * 2;
    ofBackground(background);
    metaInfo.allocate(innerWidth, 50, GL_RGB);
    wavePlotter.allocate(innerWidth, 202, GL_RGB);
    positionIndicator.allocate(innerWidth, 50, GL_RGB);
    leftDeltaPlotter.allocate(innerWidth, 100, GL_RGB);
    rightDeltaPlotter.allocate(innerWidth, 100, GL_RGB);
    detectionVisualizer.allocate(innerWidth, 25, GL_RGB);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float positionParameter = ((float)position) / ((float) length);
    msFromStart = convertPositionToMS(position);
    int msLength = convertPositionToMS(length);
    
    metaInfo.begin();
    ofClear(white);
    ofSetColor(black);
    
    ofPushMatrix();
    ofTranslate(0, bitmapStringHeight);
    string filenameInfo = "filename: " + filename;
    ofDrawBitmapString(filenameInfo, 0, 0);
    ofTranslate(getBitMapStringWidth(filenameInfo) + 20, 0);
    string msPosition = zeroPad(msFromStart, 4) + "/" + zeroPad(msLength, 4);
    ofDrawBitmapString(msPosition, 0, 0);
    ofTranslate(getBitMapStringWidth(msPosition) + 20, 0);
    ofDrawBitmapString("(" + zeroPad(position, 5) + ")", 0, 0);
    ofPopMatrix();
    
    // second line
    ofPushMatrix();
    ofTranslate(0, bitmapStringHeight * 2.25);
    string channels = "channels: " + ofToString(soundInfo.nChannels);
    ofDrawBitmapString(channels, 0, 0);
    ofTranslate(getBitMapStringWidth(channels) + 20, 0);
    string sampleRateString = "sample rate: "+ ofToString(soundInfo.sampleRate);
    ofDrawBitmapString(sampleRateString, 0, 0);
    ofPopMatrix();
    
    metaInfo.end();
    
    
    wavePlotter.begin();
    ofClear(white);
    auto rangeBetween = highClampIndex - lowClampIndex + 1;
    auto points = vector<ofPoint>(rangeBetween);
    for(int i = lowClampIndex; i <= highClampIndex; i++) {
        float x = ofMap(i, lowClampIndex, highClampIndex, 0, wavePlotter.getWidth());
        float y = ofMap(magnitudes[i], 100.0f, 0.0f, 0, wavePlotter.getHeight() - 2);
        if (i % 2 == 0) {
            ofSetColor(background);
            ofDrawRectangle(x, 0, wavePlotter.getWidth() / (rangeBetween - 1), wavePlotter.getHeight());
        }
        ofSetColor(grey);
        // ofDrawCircle(x, y, 2);
        
        points[i - lowClampIndex] = ofPoint(x, y);
    }
    ofPolyline(points).draw();
    
    // three points to represent focused slopes
    int targetScopeIndex;
    float maxValue = -1000.0f;
    
    for (int i = targetScopeRangeCenter - targetScopeRange; i< targetScopeRangeCenter + targetScopeRange; i++) {
        if (maxValue < magnitudes[i]) {
            maxValue = magnitudes[i];
            targetScopeIndex = i;
        }
    }
    
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
    
    float deltaLeft = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex  - neighborCells], 0.0);
    float deltaRight = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex + neighborCells], 0.0);
    

    
    float x = leftDeltaPlotter.getWidth() * positionParameter;
    
    leftDeltaPoints.push_back(ofPoint(x, ofMap(deltaLeft, 50.0, 0.0, 0, leftDeltaPlotter.getHeight())));
    rightDeltaPoints.push_back(ofPoint(x, ofMap(deltaRight, 50.0, 0.0, 0, rightDeltaPlotter.getHeight())));
    
    leftDeltaPlotter.begin();
    ofClear(white);
    ofSetColor(black);
    ofDrawLine(x, 0, x, leftDeltaPlotter.getHeight());
    ofDrawBitmapString(ofToString((int)(deltaLeft / neighborCells)), x + 10, leftDeltaPlotter.getHeight() * 0.5);
    ofPolyline(leftDeltaPoints).draw();
    leftDeltaPlotter.end();
    
    rightDeltaPlotter.begin();
    ofClear(white);
    ofSetColor(black);
    ofDrawLine(x, 0, x, rightDeltaPlotter.getHeight());
    ofDrawBitmapString(ofToString((int)(deltaRight / neighborCells)), x + 10, leftDeltaPlotter.getHeight() * 0.5);
    ofPolyline(rightDeltaPoints).draw();
    rightDeltaPlotter.end();
    
    if (deltaLeft > threshold && deltaRight > threshold) {
        if (!isAboveThreshold) {
            tempCursor = positionParameter;
        }
        isAboveThreshold = true;
    } else {
        if (isAboveThreshold) {
            // close the loop
            if (msLength * (positionParameter - tempCursor) > thresholdLength) {
                thresholdRanges.push_back(vector<float>{tempCursor, positionParameter});
            }
        }
        isAboveThreshold = false;
    }
    
    detectionVisualizer.begin();
    ofClear(white);
    
    for (int i = 0; i < thresholdRanges.size(); i++) {
        ofSetColor(black);
        float sx = thresholdRanges[i][0] * detectionVisualizer.getWidth();
        float positionDelta = thresholdRanges[i][1] - thresholdRanges[i][0];
        float dx = positionDelta * detectionVisualizer.getWidth();
        ofDrawRectangle(sx, 0, dx, detectionVisualizer.getHeight());
        ofSetColor(white);
        string duration = ofToString(positionDelta * msLength) + "ms";
        ofDrawBitmapString(duration,
                           sx + dx * 0.5 - getBitMapStringWidth(duration) * 0.5,
                           (detectionVisualizer.getHeight() + bitmapStringHeight) * 0.5
                           );
    }
    
    ofSetColor(black);
    
    if( isAboveThreshold ) {
        float sx = tempCursor * detectionVisualizer.getWidth();
        float positionDelta = positionParameter - tempCursor;
        float dx = positionDelta * detectionVisualizer.getWidth();
        ofDrawRectangle(sx, 0, dx, detectionVisualizer.getHeight());
        string duration = ofToString(positionDelta * msLength) + "ms";
        ofDrawBitmapString(duration,
                           positionParameter * detectionVisualizer.getWidth() + 20,
                           (detectionVisualizer.getHeight() + bitmapStringHeight) * 0.5
                           );
    }
    detectionVisualizer.end();
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
    ofTranslate(0, positionIndicator.getHeight() + 10);
    leftDeltaPlotter.draw(0, 0);
    ofTranslate(0, leftDeltaPlotter.getHeight() + 10);
    rightDeltaPlotter.draw(0, 0);
    ofTranslate(0, rightDeltaPlotter.getHeight() + 10);
    detectionVisualizer.draw(0, 0);
    ofPopMatrix();
    
    // triggers each time new FFT is processed
    if (needsRecord) {
        ofSaveScreen("out/" + filename + "/" + filename + "_" + zeroPad(msFromStart, 5) + ".png");
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
        if(position >= length) {
            position = 0;
            leftDeltaPoints.clear();
            rightDeltaPoints.clear();
            ofExit();
        }
        
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

int ofApp::convertPositionToMS (const int & position) {
    return (int) (((float) position) / ((float) soundInfo.sampleRate) * 1000);
}

float ofApp::getFreqFromIndex (const int & index) {
    return (index + 0.5) * ((float) soundInfo.sampleRate / (float) bufferSize);
}

int ofApp::getIndexFromFreq (const float & frequency) {
    float unitFreq = (float) soundInfo.sampleRate / (float) bufferSize;
    return (int) (frequency / unitFreq);
}
