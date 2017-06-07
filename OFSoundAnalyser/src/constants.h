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

const bool record{true};

// string filename = "testRecording.wav";
// const string filename { "single_ding_silver.wav" };
// single ding
// const string filename { "testRecord_1495803068741.wav" };
// double ding
// const string filename { "testRecord_1495803102954.wav" };
// single bronze ding
// const string filename { "testRecord_1495803131775.wav" };
// double bronze ding
// const string filename { "testRecord_1495804060159.wav" };

const int bufferSize { 1024 };

const float targetScopeRangeCenterFreq { 2414 };
// const float targetScopeRangeCenterFreq { 3600 };
const float lowClampFreq { 1500 };
const float highClampFreq { 4000 };

const uint8_t targetScopeRange{ 2 };
// the radius where to compare in order to get the slope
const uint8_t neighborCells { 2 }; // focus range
const float threshold { 8.0f };
const int thresholdLength { 100 };

const int nAverage { 10 };

// out

const string fileOut = "out.csv";

//
// layout
//

const int margin = 20;

//
// colors
//

const ofColor white { ofColor(244) };
const ofColor background { ofColor(240) };
const ofColor lightGray { ofColor(200) };
const ofColor grey { ofColor(120) };
const ofColor black { ofColor(10) };


#endif /* constants_h */
