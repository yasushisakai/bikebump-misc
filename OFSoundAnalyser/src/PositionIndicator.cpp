//
//  PositionIndicator.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/28.
//
//

#include "PositionIndicator.hpp"

void PositionIndicator::update (const float & positionParameter) {
    fbo.begin();
    ofClear(white);
    ofSetColor(black);
    ofDrawLine(0,
               fbo.getHeight() * 0.5,
               fbo.getWidth(),
               fbo.getHeight() * 0.5);
    
    ofDrawCircle(fbo.getWidth() * positionParameter,
                 fbo.getHeight() * 0.5,
                 3);
    fbo.end();
}
