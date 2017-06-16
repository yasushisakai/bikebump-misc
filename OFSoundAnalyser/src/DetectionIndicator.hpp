//
//  DetectionIndicator.hpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/06/04.
//
//

#ifndef DetectionIndicator_hpp
#define DetectionIndicator_hpp

#include <stdio.h>
#include <memory>
#include "ofMain.h"
#include "constants.h"
#include "goodies.h"
#include "SoundClipInfo.hpp"

class DetectionIndicator {

  void drawBar (const float & start, const float & end) const;

  ofFbo fbo;
  std::shared_ptr<SoundClipInfo> info;

  std::vector<vector<float>> waitingRanges;
  std::vector<vector<float>> leavingRanges;
  float lastDetectionStart;

  public:

  DetectionIndicator () = default;
  DetectionIndicator (const int & width, const int & height): thresholdLength{thresholdLengthMin} {
    fbo.allocate(width, height);
  };

  bool incrementThresholdLength ();

  void changeFile (const std::shared_ptr<SoundClipInfo> newInfo) {
    info = newInfo;
  };

  inline void init () {
    lastDetectionStart = 0;
    waitingRanges = vector<vector<float>> (0, vector<float>(2));
    leavingRanges = vector<vector<float>> (0, vector<float>(2));
  };

  void update (const bool & _isLeftAbove, const bool & _isRightAbove);
  void closeLeaving ();

  inline void draw () const { fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };

  inline std::vector<vector<float>> getWaitingRanges () const {
    return waitingRanges;
  }

  inline std::vector<vector<float>> getLeavingRanges () const {
    return leavingRanges;

  }

  float thresholdLength{ thresholdLengthMin };
};

#endif /* DetectionIndicator_hpp */
