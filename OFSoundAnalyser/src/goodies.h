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

    const int bitmapStringHeight = 12;

    // https://forum.openframeworks.cc/t/how-to-get-size-of-ofdrawbitmapstring/22578/6
    ofRectangle getBitMapStringBoundingBox (const std::string text);

    std::string zeroPad (const int& num, const int& digits);

    void drawCrossmark (const ofPoint& point, const int& size = 5);
}

#endif /* goodies_hpp */
