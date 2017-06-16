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

constexpr float thresholdLengthMin = {0};
constexpr float thresholdLengthMax = {400};
constexpr float thresholdLengthInc = {100};

constexpr float thresholdMin {1.0f};
constexpr float thresholdMax {8.0f};
constexpr float thresholdInc {2.0f};

// out

const string fileOut = "out_res0.csv";

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
