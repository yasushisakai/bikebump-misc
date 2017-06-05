#pragma once

#include <memory>
#include "ofMain.h"
#include "ofxMaxim.h"
#include "constants.h"
#include "goodies.h"
#include "SoundClipInfo.hpp"
#include "MetaInfoPanel.hpp"
#include "PositionIndicator.hpp"
#include "DeltaHistoryPlotter.hpp"
#include "WavePlotter.hpp"
#include "DetectionIndicator.hpp"


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

    
    maxiSample soundClip;
    std::shared_ptr<SoundClipInfo> soundInfoPtr;
    int msFromStart;
    double outputs[2];

    ofxMaxiFFT fft;
    float * magnitudes;
    vector<float> averageMagnitudes;
    int averageCount { 0 };

    int maxIndex;
    bool needsRecord; // triggers everytime when fft occurs

    int lowClampIndex, highClampIndex;
    int targetScopeRangeCenter;
    // the radius of the targetScope;
    // it will find the strongest band between targetScopeRangeCenter +- targetScopeRange;
    
    /*
     states:
        0 : no detection
        1 : over threshold, pending
        2 : over threshold, met certain duration
     */

    bool isAboveThreshold;
    float tempCursor;
    vector<vector<float>> thresholdRanges;
    
    
    MetaInfoPanel metaInfoPanel;
    WavePlotter wavePlotter;
    PositionIndicator positionIndicator;
    DeltaHistoryPlotter leftDeltaPlotter, rightDeltaPlotter;
    DetectionIndicator detectionIndicator;
};
