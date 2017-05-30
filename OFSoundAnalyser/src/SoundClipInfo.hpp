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
    std::string filename;
    long length;
    int nChannels;
    int sampleRate;
    int bitsPerSample;
    int dataSize;
    float duration;
    int msLength;
    
    long position {0};
    float positionParameter; // 0.0 - 1.0
    int positionMS;
    
    SoundClipInfo() = default;
    
    SoundClipInfo(const string& _filename, const long& _length, const char * _summary);
    
    void update ();
    
    inline void updatePosition () { position++;};
};

#endif /* SoundMetaInfo_hpp */
