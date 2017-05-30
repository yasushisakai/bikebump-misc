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
#include "ofMain.h"
#include "constants.h"
#include "goodies.h"
#include "SoundClipInfo.hpp"

class WavePlotter {
public:
    WavePlotter() = default;
    
    WavePlotter(
                const SoundClipInfo & _info,
                const int & _width,
                const int & _height,
                const int & _lowIndex,
                const int & _hightIndex,
                const int & _targetRangeCenter
                );
   
    inline int getTargetScopeIndex () { return targetScopeIndex; };
    
    void update (float* magnitudes);
    void draw ();

private:

    ofFbo fbo;
    SoundClipInfo info;
    int lowIndex, highIndex, indicesBetween;
    constexpr static float maxMagnitude { 100.0f };
    int targetRangeCenter, targetRangeRadius;
    int targetScopeIndex;
    int slopeNeighbors;

};

#endif /* WavePlotter_hpp */
