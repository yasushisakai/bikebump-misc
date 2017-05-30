//
//  goodies.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/18.
//
//

#include "goodies.h"

ofRectangle Goodies::getBitMapStringBoundingBox (const std::string text) {
    
    std::vector<std::string> lines = ofSplitString(text, "/n");
    int maxLineWidth = 0;
    // we are comparing the actual width, not the number of characters in one line
    for (int i = 0; i < (int)lines.size(); i++) {
        const std::string & line(lines[i]);
        int currentLineWidth = Goodies::getBitMapStringWidth(line);
        maxLineWidth = MAX(maxLineWidth, currentLineWidth);
    }
    
    int padding = 4;
    float leading = 1.7;
    int height = lines.size() * Goodies::bitmapStringWidth * leading - 1;
    
    return ofRectangle(0, 0, maxLineWidth, height);
}

int Goodies::getBitMapStringWidth (const std::string text) {
    
    int lineWidth = 0;
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == 9) {
            lineWidth += 8 - (lineWidth % 8);
        } else {
            lineWidth ++;
        }
    }
    return lineWidth * Goodies::bitmapStringWidth;
}


std::string Goodies::zeroPad(const int& num, const int& digits) {
    std::string numString = ofToString(num);
    int digitLeft = digits - numString.size();
    
    string zeros;
    
    for(int i = 0; i < digitLeft; i++) {
        zeros = "0" + zeros;
    }
    
    return zeros + numString;
    
}


void Goodies::drawCrossmark (const ofPoint& point, const int& size) {
    const auto r = size / 2;
    const int x = point.x;
    const int y = point.y;
    ofDrawLine(x - r, y - r, x + r, y + r);
    ofDrawLine(x + r, y - r, x - r, y + r);
}


float Goodies::getFreqFromIndex (const int & index, const int & sampleRate) {
    return (index + 0.5) * ((float) sampleRate / (float) bufferSize);
}

int Goodies::getIndexFromFreq (const float & frequency, const int & sampleRate) {
    float unitFreq = (float) sampleRate / (float) bufferSize;
    return (int) (frequency / unitFreq);
}
