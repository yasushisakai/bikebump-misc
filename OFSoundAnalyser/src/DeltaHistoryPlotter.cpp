//
//  DeltaHistoryPoltter.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/28.
//
//

#include "DeltaHistoryPlotter.hpp"

void DeltaHistoryPlotter::pushDelta (const float& _positionParameter, const float & _deltaValue) {
  deltaValues.push_back(_deltaValue);

  x = _positionParameter * fbo.getWidth();
  float y = ofMap(_deltaValue, maxDelta, 0.0f, 0.0f, fbo.getHeight());

  points.push_back(ofPoint{ x, y });

  if (averageCount == nAverage - 1) {

    /* average */
    float tempAverage{ 0.0f };
    // get last nAverage values
    std::for_each(deltaValues.end() - nAverage, deltaValues.end(), [&] (float _num) {
        tempAverage += _num;
        });

    tempAverage /= (float) nAverage;

    float y = ofMap(tempAverage, maxDelta, 0.0f, 0.0f, fbo.getHeight());

    averagePoints.push_back(ofPoint{x, y});

    /* max */
    float maxValue{ 0.0f };
    std::for_each(deltaValues.end() - nAverage, deltaValues.end(), [&] (float _num) {
        maxValue = MAX(maxValue, _num);
        });

    y = ofMap(maxValue, maxDelta, 0.0f, 0.0f, fbo.getHeight());

    isAbove = maxValue > threshold;

    maxPoints.push_back(ofPoint{x, y});
  }

}

bool DeltaHistoryPlotter::incrementThreshold () {
  threshold += thresholdInc;
  bool isCap = threshold > thresholdMax;

  if(isCap) {
    threshold = thresholdMin;
  }

  return isCap;
}

void DeltaHistoryPlotter::update(const float& _deltaValue) {

  pushDelta(info -> positionParameter, _deltaValue);

  fbo.begin();

  ofClear(white);
  ofSetColor(lightGray);
  ofPolyline(points).draw();
  ofSetColor(black);
  ofDrawLine(x, 0, x, fbo.getHeight());
  string delta = ofToString(deltaValues.back());
  ofDrawBitmapString(delta, x + 10, fbo.getHeight() * 0.5);
  ofPolyline(maxPoints).draw();
  ofPolyline(averagePoints).draw();

  fbo.end();
}

unsigned short DeltaHistoryPlotter::averageCount{ 0 };

float DeltaHistoryPlotter::threshold { thresholdMin };

