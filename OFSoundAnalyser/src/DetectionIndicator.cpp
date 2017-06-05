//
//  DetectionIndicator.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/06/04.
//
//

#include "DetectionIndicator.hpp"

DetectionIndicator::DetectionIndicator (const std::shared_ptr<SoundClipInfo> & _info, const int & _width, const int & _height): info{_info} {
    detectRanges = vector<vector<float>> (0, vector<float>(2));
    fbo.allocate(_width, _height);
}

void DetectionIndicator::update (const bool & _isLeftAbove, const bool & _isRightAbove) {
   
    bool isNextAboveThreshold = _isLeftAbove && _isRightAbove;
    
    if ( !isAboveThreshold && isNextAboveThreshold ) {
        lastDetectionStart = info -> positionParameter;
    }
    
    if ( isAboveThreshold && !isNextAboveThreshold ) {
        detectRanges.push_back(vector<float>{ lastDetectionStart, info -> positionParameter});
    }
    
    isAboveThreshold = isNextAboveThreshold;
    
    fbo.begin();
    
    ofClear(white);
    // draw previous detection regions
    for (vector<float> range : detectRanges) {
        drawBar(range[0], range[1]);
    }
    
    if (isAboveThreshold) {
        // draw bar from lastDetectionStart from current position
        drawBar(lastDetectionStart, info -> positionParameter);
    }
    
    fbo.end();
}

void DetectionIndicator::drawBar (const float & start, const float & end) const {
    ofSetColor(black);
    float sx = start * fbo.getWidth();
    float duration = end - start;
    float width = duration * fbo.getWidth();
    ofDrawRectangle(sx, 0, width, fbo.getHeight());
    
    string durationMS = ofToString((int)(duration * info -> msLength)) + " ms";
    float durationMSWidth = Goodies::getBitMapStringWidth(durationMS);
    if (width > durationMSWidth) {
        ofSetColor(white);
        ofDrawBitmapString(durationMS, sx + (width - durationMSWidth) * 0.5, (fbo.getHeight() + Goodies::bitmapStringHeight)*0.5);
    }
    
}
