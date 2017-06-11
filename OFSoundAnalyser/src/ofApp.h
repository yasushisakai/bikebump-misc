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
    void initializeSoundClip( const int & _fileCount);
    void changeFile ();
   
    ofDirectory soundClipDir;
    uint8_t fileCount{0};
    // string filename;
    
    maxiSample soundClip;
    std::shared_ptr<SoundClipInfo> soundInfoPtr;
    int msFromStart;
    double outputs[2];

    ofxMaxiFFT fft;
    float * magnitudes;

    int maxIndex;
    bool needsRecord; // triggers everytime when fft occurs

    int lowClampIndex, highClampIndex;
    int targetScopeRangeCenter;
    // the radius of the targetScope;
    // it will find the strongest band between targetScopeRangeCenter +- targetScopeRange;
    
    int innerWidth;
    
    MetaInfoPanel metaInfoPanel;
    WavePlotter wavePlotter;
    PositionIndicator positionIndicator;
    DeltaHistoryPlotter leftDeltaPlotter, rightDeltaPlotter;
    DetectionIndicator detectionIndicator;
};
