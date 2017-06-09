//
//  PositionIndicator.hpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/28.
//
//

#ifndef PositionIndicator_hpp
#define PositionIndicator_hpp

#include <stdio.h>
#include "ofMain.h"
#include "SoundClipInfo.hpp"
#include "constants.h"

class PositionIndicator {
    std::shared_ptr<SoundClipInfo> info;
    ofFbo fbo;
public:
    
    PositionIndicator() = default;
    inline PositionIndicator(const int& _width, const int& _height) { fbo.allocate(_width, _height, GL_RGB); };
   
    inline void changeFile (const shared_ptr<SoundClipInfo> & newInfo) { info = newInfo; };
    void update () const;
    inline void draw () const { fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };
};

#endif /* PositionIndicator_hpp */
