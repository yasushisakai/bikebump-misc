//
//  DetectionIndicator.hpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/06/04.
//
//

#ifndef DetectionIndicator_hpp
#define DetectionIndicator_hpp

#include <stdio.h>
#include <memory>
#include "ofMain.h"
#include "constants.h"
#include "goodies.h"
#include "SoundClipInfo.hpp"

class DetectionIndicator {
    
    ofFbo fbo;
    std::vector<vector<float>> waitingRanges;
    std::vector<vector<float>> leavingRanges;
    float lastDetectionStart;
    std::shared_ptr<SoundClipInfo> info;
    DetectionStates status;
    
    void drawBar (const float & start, const float & end) const;
    
    
public:
    DetectionIndicator () = default;
    DetectionIndicator (const std::shared_ptr<SoundClipInfo> & info, const int & width, const int & height);
    
    void update (const bool & _isLeftAbove, const bool & _isRightAbove) ;
    inline void draw () const { fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };
    
};

#endif /* DetectionIndicator_hpp */
