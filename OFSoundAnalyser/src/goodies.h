//
//  goodies.h
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/18.
//
//

#ifndef goodies_hpp
#define goodies_hpp

#include <stdio.h>
#include "ofMain.h"
#include "constants.h"

namespace Goodies {

    const int bitmapStringWidth { 8 }; // equivalent to fontSize;
    const int bitmapStringHeight { 12 };

    // https://forum.openframeworks.cc/t/how-to-get-size-of-ofdrawbitmapstring/22578/6
    ofRectangle getBitMapStringBoundingBox (const std::string text);
    
    // intended for single lines;
    int getBitMapStringWidth (const std::string text);

    std::string zeroPad (const int& num, const int& digits);

    void drawCrossmark (const ofPoint& point, const int& size = 5);
    
    float getFreqFromIndex (const int & index, const int & sampleRate);
    int getIndexFromFreq (const float & frequency, const int & sampleRate);
    
}

#endif /* goodies_hpp */
