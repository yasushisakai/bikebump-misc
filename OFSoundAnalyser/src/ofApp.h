#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "goodies.h"


class ofApp : public ofBaseApp{
    
public:
    void setup ();
    void update ();
    void draw ();
    
    void keyPressed (int key);
    void keyReleased (int key);
    void mouseMoved (int x, int y );
    void mouseDragged (int x, int y, int button);
    void mousePressed (int x, int y, int button);
    void mouseReleased (int x, int y, int button);
    void mouseEntered (int x, int y);
    void mouseExited (int x, int y);
    void windowResized (int w, int h);
    void dragEvent (ofDragInfo dragInfo);
    void gotMessage (ofMessage msg);
    
    void audioOut (float * output, int bufferSize, int nChannels);
    int convertPositionToMS (const int & positiond);
    float getFreqFromIndex (const int & indexd);
    int getIndexFromFreq (const float & frequency);
    
    // string filename = "testRecording.wav";
    const string filename = "single_ding_silver.wav";
    // "1kHz_44100Hz_16bit_05sec.wav";
    // "250Hz_44100Hz_16bit_05sec.wav";
    maxiSample soundClip;
    Goodies::SoundClipInfo soundInfo;
    long length, position;
    int msFromStart;
    const int bufferSize { 1024 };

    ofxMaxiFFT fft;
    int nFFT;
    float * magnitudes;
    int maxIndex;
    bool needsRecord; // triggers everytime when fft occurs
    const float lowClampFreq { 1500 };
    const float highClampFreq { 4000 };
    int lowClampIndex, highClampIndex;
    const float targetScopeRangeCenterFreq { 2420 };
    int targetScopeRangeCenter;
    // the radius of the targetScope;
    // it will find the strongest band between targetScopeRangeCenter +- targetScopeRange;
    const int targetScopeRange{ 2 };
    // the radius where to compare in order to get the slope
    const int neighborCells { 2 }; // focus range
    const float threshold { 2.0f };
    const int thresholdLength { 400 };
    bool isAboveThreshold;
    float tempCursor;
    vector<vector<float>> thresholdRanges;
    
    double outputs[2];
    
    const int margin = 20;
    const ofColor white { ofColor(244) };
    const ofColor background { ofColor(240) };
    const ofColor grey { ofColor(120) };
    const ofColor black { ofColor(10) };
    
    ofFbo metaInfo;
    ofFbo wavePlotter;
    ofFbo positionIndicator;
    ofFbo leftDeltaPlotter, rightDeltaPlotter;
    ofFbo detectionVisualizer;
    vector<ofPoint> leftDeltaPoints;
    vector<ofPoint> rightDeltaPoints;
};
