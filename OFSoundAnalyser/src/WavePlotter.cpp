//
//  wavePlotter.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/26.
//
//

#include "WavePlotter.hpp"


void WavePlotter::update (float* magnitudes) {
    fbo.begin();
    ofPushStyle();
    ofClear(white);
    auto points = std::vector<ofPoint>(indicesBetween);
    
    for (int i = lowIndex; i < highIndex; i++) {
        float x = ofMap(i, lowIndex, highIndex, 0.0f, fbo.getWidth());
        float y = ofMap(magnitudes[i], maxMagnitude, 0.0f, 0, fbo.getHeight() - 2.0f);
        
        points[i - lowIndex] = ofPoint(x, y);
        
        // stripes
        if (i % 2 == 0) {
            ofSetColor(background);
            ofDrawRectangle(x, 0, fbo.getWidth() / (indicesBetween - 1), fbo.getHeight());
        }
        
    }
    
    ofSetColor(grey);
    ofPolyline(points).draw();
    
    float maxValue = -10000.0f;
    
    for (int i = -targetRangeRadius; i <= targetRangeRadius; i++) {
        if (maxValue < magnitudes[targetRangeCenter + i]) {
            maxValue = magnitudes[targetRangeCenter + i];
            targetScopeIndex = targetRangeCenter + i;
        }
    }
    
    auto slopePoints = std::vector<ofPoint>(3);
    
    ofSetColor(black);
    
    for (int i = 0; i < 3; i++) {
        float x = ofMap(targetScopeIndex + (i - 1) * WavePlotter::slopeNeighbors, lowIndex, highIndex, 0.0f, fbo.getWidth());
        float y = ofMap(magnitudes[targetScopeIndex + (i - 1) * WavePlotter::slopeNeighbors], maxMagnitude, 0.0f, 0.0f, fbo.getHeight() - 2);
        
        ofPoint point = ofPoint(x, y);
        Goodies::drawCrossmark(point, 7);
        slopePoints[i] = point;
    }
    
    ofSetLineWidth(2.0f);
    ofPolyline(slopePoints).draw();
    
    float frequency = Goodies::getFreqFromIndex(targetScopeIndex, info -> sampleRate);
   
    int freqWidth = Goodies::getBitMapStringWidth(ofToString(frequency));
    float x = ofMap(targetScopeIndex, lowIndex, highIndex, 0, fbo.getWidth());
    ofDrawBitmapString(ofToString(frequency), ofPoint(x, fbo.getHeight() * 0.25));
    
    ofPopStyle();
    fbo.end();
}

void WavePlotter::draw () {
    fbo.draw(0, 0);
    ofTranslate(0, fbo.getHeight());
}
