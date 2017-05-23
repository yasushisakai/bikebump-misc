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

namespace Goodies {

    struct SoundClipInfo {
        string filename;
        int nChannels;
        int sampleRate;
        int bitsPerSample;
        int dataSize;
        float duration;
    };
    
    const int bitmapStringWidth = 8; // equivalent to fontSize;
    const int bitmapStringHeight = 12;

    // https://forum.openframeworks.cc/t/how-to-get-size-of-ofdrawbitmapstring/22578/6
    ofRectangle getBitMapStringBoundingBox (const std::string text);
    
    SoundClipInfo summaryToSoundClipInfo (const string & _filename, const char* _summary);
    
    // intended for single lines;
    int getBitMapStringWidth (const std::string text);

    std::string zeroPad (const int& num, const int& digits);

    void drawCrossmark (const ofPoint& point, const int& size = 5);
}

#endif /* goodies_hpp */
