//
//  DeltaHistoryPoltter.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/28.
//
//

#include "DeltaHistoryPlotter.hpp"

unsigned short DeltaHistoryPlotter::averageCount{ 0 };

DeltaHistoryPlotter::DeltaHistoryPlotter (const int& _width, const int& _height) {
    fbo.allocate(_width, _height, GL_RGB);
}

void DeltaHistoryPlotter::pushDelta (const float& _positionParameter, const float & _deltaValue) {
    deltaValues.push_back(_deltaValue);
    
    x = _positionParameter * fbo.getWidth();
    float y = ofMap(_deltaValue, maxDelta, 0.0f, 0.0f, fbo.getHeight());
    
    points.push_back(ofPoint{ x, y });
    
    if (averageCount == nAverage - 1) {
        
        /* average
        float tempAverage{ 0.0f };
        // get last nAverage values
        std::for_each(deltaValues.end() - nAverage, deltaValues.end(), [&] (float _num) {
            tempAverage += _num;
        });
        
        tempAverage /= (float) nAverage;
        
        float y = ofMap(tempAverage, maxDelta, 0.0f, 0.0f, fbo.getHeight());
        */
        
        /* max */
        float maxValue{ 0.0f };
        std::for_each(deltaValues.end() - nAverage, deltaValues.end(), [&] (float _num) {
            maxValue = MAX(maxValue, _num);
        });
        
        float y = ofMap(maxValue, maxDelta, 0.0f, 0.0f, fbo.getHeight());
        
        
        averagePoints.push_back(ofPoint{x, y});
    }

}

void DeltaHistoryPlotter::update() {
    fbo.begin();
    
    ofClear(white);
    ofSetColor(lightGray);
    ofPolyline(points).draw();
    ofSetColor(black);
    ofDrawLine(x, 0, x, fbo.getHeight());
    string delta = ofToString(deltaValues.back());
    ofDrawBitmapString(delta, x + 10, fbo.getHeight() * 0.5);
    ofPolyline(averagePoints).draw();
    
    fbo.end();
}
