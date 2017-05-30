//
//  SoundMetaInfo.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/28.
//
//

#include "SoundClipInfo.hpp"

SoundClipInfo::SoundClipInfo (const std::string & _filename, const long & _length, const char * _summary): filename{_filename}, length{_length} {

    auto summaryList = ofSplitString(std::string(_summary), "\n");
    nChannels = ofToInt(ofTrim(ofSplitString(summaryList[1], ":")[1]));
    sampleRate = ofToInt(ofTrim(ofSplitString(summaryList[2], ":")[1]));
    bitsPerSample = ofToInt(ofTrim(ofSplitString(summaryList[5], ":")[1]));
    dataSize = ofToInt(ofTrim(ofSplitString(summaryList[6], ":")[1]));
    duration = (dataSize * 1000.0f) / (nChannels * sampleRate * bitsPerSample / 8); // ms
    msLength = convertPositionToMS(length);
}

void SoundClipInfo::update () {
    positionParameter = (float) position / (float) length;
    positionMS = convertPositionToMS(position);
}
