//
//  DetectionIndicator.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/06/04.
//
//

#include "DetectionIndicator.hpp"

DetectionIndicator::DetectionIndicator (const int & _width, const int & _height) {
    waitingRanges = vector<vector<float>> (0, vector<float>(2));
    leavingRanges = vector<vector<float>> (0, vector<float>(2));
    fbo.allocate(_width, _height);
    status = INITIAL;
}

void DetectionIndicator::update (const bool & _isLeftAbove, const bool & _isRightAbove) {
   
    bool isNextAboveThreshold = _isLeftAbove && _isRightAbove;
    
    if ( status == INITIAL && isNextAboveThreshold) {
        lastDetectionStart = info -> positionParameter;
        status = WAITING;
    }
    
    if ( status == WAITING && isNextAboveThreshold ) {
        float dingDuration = (info -> positionParameter - lastDetectionStart) * info -> msLength;
        if(dingDuration > thresholdLength) {
            waitingRanges.push_back(vector<float>{ lastDetectionStart, info -> positionParameter});
            lastDetectionStart = info -> positionParameter;
            
            status = LEAVING;
        }
    }
    
    if ( (status == LEAVING || status == WAITING) && !isNextAboveThreshold ) {
        leavingRanges.push_back(vector<float>{ lastDetectionStart, info -> positionParameter});
        status = INITIAL;
    }
    
    fbo.begin();
    
    ofClear(white);
    // draw previous detection regions
    for (vector<float> range : waitingRanges) {
        ofSetColor(black);
        drawBar(range[0], range[1]);
    }
    
    for (vector<float> range : leavingRanges) {
        ofSetColor(lightGray);
        drawBar(range[0], range[1]);
    }
    
    if (leavingRanges.size() == 2) {
        ofSetColor(black);
        float sx = leavingRanges[1][1] * fbo.getWidth() + margin;
        
        float y = (fbo.getHeight() + Goodies::bitmapStringHeight) * 0.5;
        string waitPitch = "<-> " + ofToString((waitingRanges[1][0] - waitingRanges[0][0]) * info -> msLength ) + " ms";
        
        ofDrawBitmapString(waitPitch, sx, y);
        
    }
    
    if (isNextAboveThreshold) {
        // draw bar from lastDetectionStart from current position
        switch (status) {
            case WAITING:
                ofSetColor(black);
                break;
            case LEAVING:
                ofSetColor(lightGray);
                break;
            default :
                break;
        }
        
        drawBar(lastDetectionStart, info -> positionParameter);
    }
    
    fbo.end();
}

void DetectionIndicator::drawBar (const float & start, const float & end) const {
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
