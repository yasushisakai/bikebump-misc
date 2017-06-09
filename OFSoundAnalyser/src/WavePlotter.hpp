//
//  wavePlotter.hpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/26.
//
//

#ifndef WavePlotter_hpp
#define WavePlotter_hpp

#include <stdio.h>
#include <memory>
#include "ofMain.h"
#include "constants.h"
#include "goodies.h"
#include "SoundClipInfo.hpp"

class WavePlotter {
public:
    WavePlotter() = default;
    
    WavePlotter( const int & _width, const int & _height, const int & _lowIndex, const int & _hightIndex):
        lowIndex{ _lowIndex },
        highIndex{ _hightIndex }
    {
        indicesBetween = highIndex - lowIndex;
        fbo.allocate(_width, _height);
    };
    
    inline void changeFile ( const std::shared_ptr<SoundClipInfo> & newSoundInfo) {
        info = newSoundInfo;
        targetRangeCenter = Goodies::getIndexFromFreq(info -> targetFrequency, info -> sampleRate);
    };
   
    inline int getTargetScopeIndex () { return targetScopeIndex; };
    
    void update (float* magnitudes);
    void draw ();
    
    constexpr static int slopeNeighbors { 2 };

private:

    ofFbo fbo;
    std::shared_ptr<SoundClipInfo> info;
    int lowIndex, highIndex, indicesBetween;
    constexpr static float maxMagnitude { 100.0f };
    constexpr static int targetRangeRadius { 2 };
    int targetRangeCenter;
    int targetScopeIndex;

};

#endif /* WavePlotter_hpp */
