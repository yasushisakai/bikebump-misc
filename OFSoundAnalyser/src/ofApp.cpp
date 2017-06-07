#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    // I want to refresh only if new fft.process is true
    ofSetBackgroundAuto(false);
    innerWidth = ofGetWidth() - margin * 2;
    
    fft.setup(bufferSize, 512, 256);
    magnitudes = fft.magnitudes; // fft.magsToDB();
    
    // get the list of sound clips
    ofDirectory dir{ofToDataPath("soundClips")};
    //only show png files
    dir.allowExt("wav");
    //populate the directory object
    dir.listDir();
    
    for (uint8_t i = 0; i < dir.size(); i++) {
        ofLogNotice("files", dir.getPath(i));
    }
    
    filename = dir.getName(0);
    
    soundClip.load(ofToDataPath("soundClips/" + filename));
    soundInfoPtr = std::make_shared<SoundClipInfo>(SoundClipInfo(filename, soundClip.getLength(), soundClip.getSummary()));
    
    needsRecord = true;
    
    lowClampIndex = Goodies::getIndexFromFreq(lowClampFreq, soundInfoPtr -> sampleRate);
    highClampIndex = Goodies::getIndexFromFreq(highClampFreq, soundInfoPtr -> sampleRate);
    targetScopeRangeCenter = Goodies::getIndexFromFreq(targetScopeRangeCenterFreq, soundInfoPtr -> sampleRate);
    
    ofSoundStreamSetup(2, 2, this, soundInfoPtr -> sampleRate, bufferSize, 4);
    
    initializeRenderComponents(soundInfoPtr);
    
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
    
    float deltaLeft = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex - neighborCells], 0.0);
    float deltaRight = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex + neighborCells], 0.0);
    
    leftDeltaPlotter.update(soundInfoPtr -> positionParameter, deltaLeft);
    rightDeltaPlotter.update(soundInfoPtr -> positionParameter, deltaRight);
    
    DeltaHistoryPlotter::updateAverageCount(); // incrementing static counter for delta plotter
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

void ofApp::initializeSoundClip( const string & fileName) {
    
    // close any open sound stream
    ofSoundStreamClose();
    
    soundClip.load(ofToDataPath("soundClips/" + fileName));
    
    
}

void ofApp::initializeRenderComponents (const std::shared_ptr<SoundClipInfo> & _soundInfoPtr) {
   metaInfoPanel = MetaInfoPanel{ _soundInfoPtr, innerWidth, 50 };
    wavePlotter = WavePlotter{ _soundInfoPtr, innerWidth, 202, lowClampIndex, highClampIndex, targetScopeRangeCenter };
    positionIndicator = PositionIndicator{ innerWidth, 50 };
    leftDeltaPlotter = DeltaHistoryPlotter{ innerWidth, 100 };
    rightDeltaPlotter = DeltaHistoryPlotter{ innerWidth, 100 };
    detectionIndicator = DetectionIndicator{ _soundInfoPtr, innerWidth, 50 };
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
            
            vector<vector<float>> waitingRanges{ detectionIndicator.getWaitingRanges() };
            
            ofFile file;
            file.open(ofToDataPath(fileOut), ofFile::Append, false);
            file << filename << ", " << soundInfoPtr -> msLength;
            
            for (auto range : waitingRanges) {
                file << ", " << range[0];
            }
            
            file << endl;
            
            file.close();
            ofExit();
        }
        
        if (fft.process(sound)) {
            
            // this will stop this thread
            // wait untill re-render
            if (record) {
                needsRecord = true;
            }

            magnitudes = fft.magnitudes; //fft.magsToDB();
            

        }
        if (!record) {
            output[i * nChannels] = sound;
            output[i * nChannels + 1] = sound;
        }
        
    }
}




