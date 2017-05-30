#include "ofApp.h"

using namespace Goodies;

//--------------------------------------------------------------
void ofApp::setup(){
    
    soundClip.load(ofToDataPath("soundClips/" + filename));
    
    soundInfo = SoundClipInfo::SoundClipInfo(filename, soundClip.getLength(), soundClip.getSummary());
    
    fft.setup(bufferSize, 512, 256);
    magnitudes = fft.magnitudes; // fft.magsToDB();
    maxIndex = -1;
    needsRecord = false;
    averageMagnitudes = vector<float>(bufferSize, 0.0f);
    
    lowClampIndex = getIndexFromFreq(lowClampFreq, soundInfo.sampleRate);
    highClampIndex = getIndexFromFreq(highClampFreq, soundInfo.sampleRate);
    targetScopeRangeCenter = getIndexFromFreq(targetScopeRangeCenterFreq, soundInfo.sampleRate);
    isAboveThreshold = false;
    thresholdRanges  = vector<vector<float>>(0, vector<float>(2));
    
    ofSoundStreamSetup(2, 2, this, soundInfo.sampleRate, bufferSize, 4);
    
    auto innerWidth = ofGetWidth() - margin * 2;
    
    // I want to refresh only if new fft.process is true
    ofSetBackgroundAuto(false);
    metaInfoPanel = MetaInfoPanel{ innerWidth, 50 };
    wavePlotter = WavePlotter{ soundInfo, innerWidth, 202, lowClampIndex, highClampIndex, targetScopeRangeCenter };
    positionIndicator = PositionIndicator{ innerWidth, 50 };
    leftDeltaPlotter = DeltaHistoryPlotter{ innerWidth, 100 };
    rightDeltaPlotter = DeltaHistoryPlotter{ innerWidth, 100 };
    detectionVisualizer.allocate(innerWidth, 25, GL_RGB);
    
    // clear folder and make dir
    auto saveDir = ofDirectory{ "/out/" + filename };
    if (saveDir.exists()) {
        saveDir.remove(true);
    }
    
    saveDir.create();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (!needsRecord) return;
    
    soundInfo.update();
    metaInfoPanel.update(soundInfo);
    wavePlotter.update(magnitudes);
    positionIndicator.update(soundInfo.positionParameter);
    
    int targetScopeIndex = wavePlotter.getTargetScopeIndex();
    
    float deltaLeft = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex  - neighborCells], 0.0);
    float deltaRight = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex + neighborCells], 0.0);
    
    // TODO: make it cleaner
    leftDeltaPlotter.pushDelta(soundInfo.positionParameter, deltaLeft);
    leftDeltaPlotter.update();
    rightDeltaPlotter.pushDelta(soundInfo.positionParameter, deltaRight);
    rightDeltaPlotter.update();
    
    DeltaHistoryPlotter::updateAverageCount();
    
    detectionVisualizer.begin();
    ofClear(white);
    
    for (int i = 0; i < thresholdRanges.size(); i++) {
        ofSetColor(black);
        float sx = thresholdRanges[i][0] * detectionVisualizer.getWidth();
        float positionDelta = thresholdRanges[i][1] - thresholdRanges[i][0];
        float dx = positionDelta * detectionVisualizer.getWidth();
        ofDrawRectangle(sx, 0, dx, detectionVisualizer.getHeight());
        ofSetColor(white);
        string duration = ofToString(positionDelta * soundInfo.msLength) + "ms";
        ofDrawBitmapString(duration,
                           sx + dx * 0.5 - getBitMapStringWidth(duration) * 0.5,
                           (detectionVisualizer.getHeight() + bitmapStringHeight) * 0.5
                           );
    }
    
    ofSetColor(black);
    
    if( isAboveThreshold ) {
        float sx = tempCursor * detectionVisualizer.getWidth();
        float positionDelta = soundInfo.positionParameter - tempCursor;
        float dx = positionDelta * detectionVisualizer.getWidth();
        ofDrawRectangle(sx, 0, dx, detectionVisualizer.getHeight());
        string duration = ofToString(positionDelta * soundInfo.msLength) + "ms";
        ofDrawBitmapString(duration,
                           soundInfo.positionParameter * detectionVisualizer.getWidth() + 20,
                           (detectionVisualizer.getHeight() + bitmapStringHeight) * 0.5
                           );
    }
    detectionVisualizer.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // everytime fft.process is true
    if (!needsRecord) return;
    
    ofClear(background);
    
    ofPushMatrix();
    ofTranslate(margin, margin);
    metaInfoPanel.draw();
    ofTranslate(0, margin);
    positionIndicator.draw();
    ofTranslate(0, margin);
    leftDeltaPlotter.draw();
    ofTranslate(0, margin);
    rightDeltaPlotter.draw();
    ofTranslate(0, margin);
    detectionVisualizer.draw(0, 0);
    wavePlotter.draw();
    ofPopMatrix();
    
    ofSaveScreen("out/" + filename + "/" + filename + "_" + zeroPad(soundInfo.positionMS, 5) + ".png");
    
    needsRecord = false;
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
        soundInfo.position ++;
        if(soundInfo.position >= soundInfo.length) {
            soundInfo.position = 0;
            leftDeltaPoints.clear();
            rightDeltaPoints.clear();
            ofExit();
        }
        
        if (fft.process(sound)) {
            
            // this will stop this thread
            // wait untill re-render
            needsRecord = true;

            magnitudes = fft.magnitudes; //fft.magsToDB();
            float magMax = 1;
            maxIndex = -1;
            
            for (int j = lowClampIndex; j < highClampIndex; j++) {
                if (magMax < magnitudes[j]) {
                    magMax = magnitudes[j];
                    maxIndex = j;
                }
            }
            
            for (int i = 0; i < bufferSize; i++) {
                // averageMagnitudes[i] += (float)(magnitudes[i] / (float)nAverage);
                averageMagnitudes[i] = MAX(magnitudes[i], averageMagnitudes[i]);
            }
            
            averageCount++;
            
            if (averageCount == nAverage) {
                averageCount = 0;
                averageMagnitudes = vector<float>(bufferSize, 0.0f);
            }
            

        }
        
        /* don't wanna hear
         output[i * nChannels] = sound;
         output[i * nChannels + 1] = sound;
         */
        
    }
}




