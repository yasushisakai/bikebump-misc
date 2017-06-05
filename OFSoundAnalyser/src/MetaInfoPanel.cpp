//
//  MetaInfoPanel.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/27.
//
//

#include "MetaInfoPanel.hpp"


void MetaInfoPanel::update() const {
    fbo.begin();
    
    ofClear(white);
    ofSetColor(black);
    
    ofPushMatrix();
    ofTranslate(0, Goodies::bitmapStringHeight);
    string filenameInfo = "filename: " + filename;
    ofDrawBitmapString(filenameInfo, 0, 0);
    ofTranslate(Goodies::getBitMapStringWidth(filenameInfo) + 20, 0);
    string msPosition = Goodies::zeroPad(info -> positionMS, 4) + "/" + Goodies::zeroPad((int)info ->msLength, 4);
    ofDrawBitmapString(msPosition, 0, 0);
    ofTranslate(Goodies::getBitMapStringWidth(msPosition) + 20, 0);
    ofDrawBitmapString("(" + Goodies::zeroPad((int)info -> position, 5) + ")", 0, 0);
    ofPopMatrix();
    
    // line --- break
    
    ofPushMatrix();
    ofTranslate(0, Goodies::bitmapStringHeight * 2.25);
    string channels = "channels: " + ofToString(info -> nChannels);
    ofDrawBitmapString(channels, 0, 0);
    ofTranslate(Goodies::getBitMapStringWidth(channels) + 20, 0);
    string sampleRateString = "sample rate: "+ ofToString(info -> sampleRate);
    ofDrawBitmapString(sampleRateString, 0, 0);
    ofPopMatrix();
    
    fbo.end();
}
