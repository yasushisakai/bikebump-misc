#pragma once

#include "ofMain.h"
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
    
    
    // sound stuff
    ofSoundPlayer ding;
    static const int nBandsToGet { 1024 };
    float * fft;
    float position;
    string positionMS;
    int maxIndex;
    
    float getFreqByIndex (const int & index);
    
    // file stuff
    string filename;
    ofRectangle filenameRect;
    const string outDir = "out/";
    bool isRecordingFrames = true;
    
    // layout stuff
    const ofColor backgroundColor { ofColor(240) };
    static const int margin { 20 };
    
    // meta info
    ofFbo metaInfo;
    
    // current position;
    ofFbo currentPosition;
    
    // frequency domain visualization
    ofFbo frequencyGraph;
    float bandWidth;
    
};
