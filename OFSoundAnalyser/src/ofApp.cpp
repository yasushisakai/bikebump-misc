#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

  // I want to refresh only if new fft.process is true
  ofSetBackgroundAuto(false);
  innerWidth = ofGetWidth() - margin * 2;

  fft.setup(bufferSize, 512, 256);
  magnitudes = fft.magnitudes; // fft.magsToDB();

  soundClipDir = ofDirectory{ofToDataPath("soundClips")};
  soundClipDir.allowExt("wav");
  soundClipDir.listDir();

  lowClampIndex = Goodies::getIndexFromFreq(lowClampFreq, sampleRate);
  highClampIndex = Goodies::getIndexFromFreq(highClampFreq, sampleRate);

  /* initializing rendering components */
  metaInfoPanel = MetaInfoPanel{ innerWidth, 50 };
  wavePlotter = WavePlotter{ innerWidth, 202, lowClampIndex, highClampIndex };
  positionIndicator = PositionIndicator{ innerWidth, 50 };
  leftDeltaPlotter = DeltaHistoryPlotter{ innerWidth, 100 };
  rightDeltaPlotter = DeltaHistoryPlotter{ innerWidth, 100 };
  detectionIndicator = DetectionIndicator{ innerWidth, 50 };

  initializeSoundClip( fileCount );

  ofSoundStreamSetup(2, 2, this, soundInfoPtr -> sampleRate, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update(){

  if (!needsRecord) return;

  soundInfoPtr -> update();

  metaInfoPanel.update();
  wavePlotter.update(magnitudes);
  positionIndicator.update();

  int targetScopeIndex = wavePlotter.getTargetScopeIndex();

  float deltaLeft = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex - WavePlotter::slopeNeighbors], 0.0);
  float deltaRight = MAX(magnitudes[targetScopeIndex] - magnitudes[targetScopeIndex + WavePlotter::slopeNeighbors], 0.0);

  leftDeltaPlotter.update(deltaLeft);
  rightDeltaPlotter.update(deltaRight);

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

  /*
     ofSaveScreen(
     "out/" +
     soundInfoPtr -> filename + "/" +
     soundInfoPtr -> filename + "_" +
     Goodies::zeroPad(soundInfoPtr -> positionMS, 5) +
     ".png"
     );
  */

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

void ofApp::initializeSoundClip(const int & _newFileCount) {

  string filename = soundClipDir.getName(_newFileCount);

  soundClip.load(ofToDataPath("soundClips/" + filename));
  soundInfoPtr = std::make_shared<SoundClipInfo> (SoundClipInfo(filename, soundClip.getLength(), soundClip.getSummary()));

  needsRecord = true;

  // clear folder and make dir
  auto saveDir = ofDirectory{ "/out/" + filename };

  if (saveDir.exists()) {
    saveDir.remove(true);
  }

  // saveDir.create(true); // permission denied
  ofDirectory::createDirectory("out/" + filename);

  changeFile();
}


void ofApp::changeFile () {
  metaInfoPanel.changeFile(soundInfoPtr);
  wavePlotter.changeFile(soundInfoPtr);
  positionIndicator.changeFile(soundInfoPtr);
  leftDeltaPlotter.changeFile(soundInfoPtr);
  rightDeltaPlotter.changeFile(soundInfoPtr);
  detectionIndicator.changeFile(soundInfoPtr);
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

      detectionIndicator.closeLeaving();

      vector<vector<float>> waitingRanges{ detectionIndicator.getWaitingRanges() };
      vector<vector<float>> leavingRanges{ detectionIndicator.getLeavingRanges() };

      ofFile file;
      file.open(ofToDataPath(fileOut), ofFile::Append, false);
      file << soundInfoPtr -> filename << ", ";
      file << ", " << DeltaHistoryPlotter::threshold << ", " << detectionIndicator.thresholdLength;

      file << ", waiting";
      for (auto range : waitingRanges) {
        for (auto key : range) {
          file << ", " << soundInfoPtr -> parameterToMSFromStart(key);
        }
      }

      file << ", leaving";
      for (auto range : leavingRanges) {
        for (auto key : range) {
          file << ", " << soundInfoPtr -> parameterToMSFromStart(key);
        }
      }

      // num of detections = number of waitings
      uint8_t numWaitings = waitingRanges.size();
      bool didPass{false};

      if (soundInfoPtr -> isDoubleDing) {
        didPass = numWaitings == 2;
      } else {
        didPass = numWaitings == 1;
      }

      file << ", didPass, " << didPass;

      if (numWaitings == 2) {
        int betweenWaitings = soundInfoPtr -> parameterToMSFromStart(waitingRanges[1][0] - waitingRanges[1][0]);

        file << ", detectionBtwn, " << betweenWaitings;
      }
      
      if (numWaitings > 0) {
        int betweenWhole = soundInfoPtr -> parameterToMSFromStart(leavingRanges[leavingRanges.size() - 1][1] - waitingRanges[0][0]);
        file << ", detectionWhole," << betweenWhole;

        if (leavingRanges[leavingRanges.size() - 1][1] == 1.0f) {
          file << "+";
        }
      }

      file << endl;

      file.close();

      leftDeltaPlotter.init();
      rightDeltaPlotter.init();
      detectionIndicator.init();

      if (DeltaHistoryPlotter::incrementThreshold()) {
        ofLogNotice("incrementThreshold capped");
        if (detectionIndicator.incrementThresholdLength()) {
          ofLogNotice("increment Threshold Length capped");
          fileCount ++;
          if (fileCount < soundClipDir.size()) {
            initializeSoundClip(fileCount);
          } else {
            ofExit();
          }
        }
      }
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




