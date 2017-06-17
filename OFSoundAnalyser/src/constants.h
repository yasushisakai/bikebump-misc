//
//  constants.h
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/26.
//
//

#ifndef constants_h
#define constants_h

#include <cstdint>
#include "ofMain.h"

//
// global
//

constexpr bool record{true};

constexpr int bufferSize { 1024 };
constexpr float sampleRate { 48000 };

constexpr float lowClampFreq { 1500 };
constexpr float highClampFreq { 4000 };

constexpr int nAverage { 10 };

constexpr float thresholdLengthMin = {100};
constexpr float thresholdLengthMax = {201};
constexpr float thresholdLengthInc = {25};

constexpr float thresholdMin {3.0f};
constexpr float thresholdMax {5.1f};
constexpr float thresholdInc {1.0f};

// out

const string fileOut = "out_res1.csv";

//
// layout
//

constexpr int margin = 20;

//
// colors
//

const ofColor white { ofColor(244) };
const ofColor background { ofColor(240) };
const ofColor lightGray { ofColor(200) };
const ofColor grey { ofColor(120) };
const ofColor black { ofColor(10) };

#endif /* constants_h */
