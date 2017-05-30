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
#include "constants.h"

class PositionIndicator {
    ofFbo fbo;
public:
    
    PositionIndicator() = default;
    inline PositionIndicator(const int& _width, const int& _height) {fbo.allocate(_width, _height, GL_RGB); };
    
    void update (const float & positionParameter);
    
    inline void draw () {fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };
};

#endif /* PositionIndicator_hpp */
