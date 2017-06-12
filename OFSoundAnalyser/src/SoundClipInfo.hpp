//
//  SoundMetaInfo.hpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/28.
//
//

#ifndef SoundMetaInfo_hpp
#define SoundMetaInfo_hpp

#include <stdio.h>
#include "ofMain.h"

class SoundClipInfo {

  inline int convertPositionToMS (const int & _pos) {
    return (int) (((float) _pos) / ((float) sampleRate) * 1000);
  }

  public:

  SoundClipInfo() = default;
  SoundClipInfo(const string & _filename, const long& _length, const char * _summary);

  void update ();

  inline void incrementPosition () { position++; };
  inline int parameterToMSFromStart (const float & parameter) const { return (int)(parameter * msLength); };
  inline bool doesNeedToReset () const { return position >= length; };

  std::string filename;

  long length;
  int nChannels;
  int sampleRate;
  int bitsPerSample;
  int dataSize;
  float duration;
  int msLength;

  float targetFrequency;
  bool isRandom;
  bool isDoubleDing;

  long position {0};
  float positionParameter; // 0.0 - 1.0
  int positionMS;

};

enum DetectionStates: uint8_t {
  INITIAL = 0,
  WAITING,  // after detecting the threshold, it waits for a period to verify the ding
  LEAVING,  // is untill it vanishes
};

#endif /* SoundMetaInfo_hpp */
