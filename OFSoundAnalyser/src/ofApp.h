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
    int convertPositionToMS (const int & position, const int & sampleRate = 48000);
    float getFreqFromIndex (const int & index, const int & sampleRate = 48000);
    int getIndexFromFreq (const float & frequency, const int & sampleRate = 48000);
    
    string filename = "single_ding_silver.wav";
    // "1kHz_44100Hz_16bit_05sec.wav";
    // "250Hz_44100Hz_16bit_05sec.wav";
    ofRectangle filenameBoundingBox;
    
    maxiSample soundClip;
    long length, position;
    int msFromStart;
    int bufferSize, sampleRate;

    ofxMaxiFFT fft;
    float * magnitudes;
    int maxIndex;
    bool needsRecord; // triggers everytime when fft occurs
    int lowClampIndex, highClampIndex;
    int targetScopeIndex, neighborCells; // focus range
    
    double outputs[2];
    
    const int margin = 20;
    const ofColor white {ofColor(240)};
    const ofColor grey {ofColor(80)};
    const ofColor black {ofColor(10)};
    
    ofFbo metaInfo;
    ofFbo wavePlotter;
    ofFbo positionIndicator;
    ofFbo magnitudeTracker;
};
