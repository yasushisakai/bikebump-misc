//
//  DeltaHistoryPoltter.hpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/28.
//
//

#ifndef DeltaHistoryPlotter_hpp
#define DeltaHistoryPlotter_hpp

#include <stdio.h>
#include "ofMain.h"
#include "constants.h"

class DeltaHistoryPlotter {
    ofFbo fbo;
    float x{ 0.0f }; // current cursor
    std::vector<float> deltaValues;

    std::vector<ofPoint> points;
    
    std::vector<ofPoint> maxPoints;
    std::vector<ofPoint> averagePoints;
    
    constexpr static float maxDelta { 50.0f };
    constexpr static unsigned short nAverage { 5 };
    
    void pushDelta(const float & _positionParamter, const float & _deltaValue);
    
public:
    DeltaHistoryPlotter() = default;
    DeltaHistoryPlotter(const int& _width, const int& _height);

    void update (const float & _potionParamter, const float & _deltaValue);
    inline void draw () const { fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };
    inline void clearDeltaValues() { deltaValues.clear(); points.clear(); };
    
    inline static void updateAverageCount () { averageCount = averageCount < nAverage ? averageCount + 1 : 0; };
    static unsigned short averageCount;
    
    bool isAbove;
};

#endif /* DeltaHistoryPlotter_hpp */
