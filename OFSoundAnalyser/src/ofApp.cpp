#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    soundClip.load(ofToDataPath("soundClips/" + filename));
    
    soundInfoPtr = std::make_shared<SoundClipInfo>(SoundClipInfo(filename, soundClip.getLength(), soundClip.getSummary()));
    
    fft.setup(bufferSize, 512, 256);
    magnitudes = fft.magnitudes; // fft.magsToDB();
    maxIndex = -1;
    needsRecord = true;
    averageMagnitudes = vector<float>(bufferSize, 0.0f);
    
    lowClampIndex = Goodies::getIndexFromFreq(lowClampFreq, soundInfoPtr -> sampleRate);
    highClampIndex = Goodies::getIndexFromFreq(highClampFreq, soundInfoPtr -> sampleRate);
    targetScopeRangeCenter = Goodies::getIndexFromFreq(targetScopeRangeCenterFreq, soundInfoPtr -> sampleRate);
    isAboveThreshold = false;
    thresholdRanges  = vector<vector<float>>(0, vector<float>(2));
    
    ofSoundStreamSetup(2, 2, this, soundInfoPtr -> sampleRate, bufferSize, 4);
    
    auto innerWidth = ofGetWidth() - margin * 2;
    
    // I want to refresh only if new fft.process is true
    ofSetBackgroundAuto(false);
    metaInfoPanel = MetaInfoPanel{ soundInfoPtr, innerWidth, 50 };
    wavePlotter = WavePlotter{ soundInfoPtr, innerWidth, 202, lowClampIndex, highClampIndex, targetScopeRangeCenter };
    positionIndicator = PositionIndicator{ innerWidth, 50 };
    leftDeltaPlotter = DeltaHistoryPlotter{ innerWidth, 100 };
    rightDeltaPlotter = DeltaHistoryPlotter{ innerWidth, 100 };
    detectionIndicator = DetectionIndicator{ soundInfoPtr, innerWidth, 50 };
    
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
    
    soundInfoPtr -> update();
    
    metaInfoPanel.update();
    wavePlotter.update(magnitudes);
    positionIndicator.update(soundInfoPtr -> positionParameter);
    
    int targetScopeIndex = wavePlotter.getTargetScopeIndex();
    
    float deltaLeft = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex  - neighborCells], 0.0);
    float deltaRight = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex + neighborCells], 0.0);
    
    leftDeltaPlotter.update(soundInfoPtr -> positionParameter, deltaLeft);
    rightDeltaPlotter.update(soundInfoPtr -> positionParameter, deltaRight);
    
    DeltaHistoryPlotter::updateAverageCount();
    
    detectionIndicator.update(leftDeltaPlotter.isAbove, rightDeltaPlotter.isAbove);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // everytime fft.process is true
    if (record) {
        if (!needsRecord) return;
    }
    
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
    detectionIndicator.draw();
    ofTranslate(0, margin);
    
    // detectionVisualizer.draw(0, 0);
    wavePlotter.draw();
    ofPopMatrix();
    
    ofSaveScreen("out/" + filename + "/" + filename + "_" + Goodies::zeroPad(soundInfoPtr -> positionMS, 5) + ".png");
    
    if (record) {
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
    
    if(record) {
        if(needsRecord) return;
    }
    
    for (int i = 0; i < bufferSize; i++) {
        double sound = soundClip.play();
        soundInfoPtr -> incrementPosition();
        if(soundInfoPtr -> doesNeedToReset() ) {
            soundInfoPtr -> position = 0;
            // write to out.csv
            ofExit();
        }
        
        if (fft.process(sound)) {
            
            // this will stop this thread
            // wait untill re-render
            if (record) {
                needsRecord = true;
            }

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
        if (!record) {
            output[i * nChannels] = sound;
            output[i * nChannels + 1] = sound;
        }
        
    }
}




