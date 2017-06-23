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

/* res 2-1
constexpr float thresholdLengthMin = {125};
constexpr float thresholdLengthMax = {150};
constexpr float thresholdLengthInc = {10};

constexpr float thresholdMin {5.0f};
constexpr float thresholdMax {7.1f};
constexpr float thresholdInc {1.0f};
*/

/* res 1-2 */
constexpr float thresholdLengthMin = {200};
constexpr float thresholdLengthMax = {301};
constexpr float thresholdLengthInc = {50};

constexpr float thresholdMin {3.0f};
constexpr float thresholdMax {5.1f};
constexpr float thresholdInc {1.0f};


// out

const string fileOut = "out_res1-2.csv";

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
