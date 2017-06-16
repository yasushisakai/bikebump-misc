//
//  DeltaHistoryPoltter.hpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/28.
//
//

#ifndef DeltaHistoryPlotter_hpp
#define DeltaHistoryPlotter_hpp

#include <memory>
#include <stdio.h>
#include "ofMain.h"
#include "constants.h"
#include "SoundClipInfo.hpp"

class DeltaHistoryPlotter {

  void pushDelta(
      const float & _positionParamter,
      const float & _deltaValue
      );

  constexpr static float maxDelta { 50.0f };
  constexpr static unsigned short nAverage { 5 };

  ofFbo fbo;
  std::shared_ptr<SoundClipInfo> info;
  std::vector<float> deltaValues;

  float x{ 0.0f }; // current cursor

  // list of points to draw
  std::vector<ofPoint> points;
  std::vector<ofPoint> maxPoints;
  std::vector<ofPoint> averagePoints;

  public:

  DeltaHistoryPlotter() = default;
  DeltaHistoryPlotter(const int& _width, const int& _height): isAbove{ false } {
    fbo.allocate(_width, _height, GL_RGB);
  };

  static bool incrementThreshold ();
  inline static void updateAverageCount () {
    averageCount = averageCount < nAverage ? averageCount + 1 : 0; 
  };

  inline void init() { points.clear(); maxPoints.clear(); averagePoints.clear(); };
  inline void changeFile (const std::shared_ptr<SoundClipInfo> & newInfo) { info = newInfo; };
  inline void clearDeltaValues() { fbo.clear(); deltaValues.clear(); points.clear(); };
  void update (const float & _deltaValue);
  inline void draw () const { fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };

  static unsigned short averageCount;
  static float threshold;
  bool isAbove;
};

#endif /* DeltaHistoryPlotter_hpp */
