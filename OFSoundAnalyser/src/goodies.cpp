//
//  goodies.cpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/18.
//
//

#include "goodies.h"

ofRectangle getBitMapStringBoundingBox (const std::string text) {
    
    std::vector<std::string> lines = ofSplitString(text, "/n");
    int maxLineLength = 0;
    for (int i = 0; i < (int)lines.size(); i++) {
        const std::string & line(lines[i]);
        int currentLineLength = 0;
        for (int j= 0; j < (int)line.size(); j++) {
            if (line[j] == '/t') {
                currentLineLength += 8 - (currentLineLength % 8);
            } else {
                currentLineLength ++;
            }
        }
        maxLineLength = MAX(maxLineLength, currentLineLength);
    }
    
    int padding = 4;
    int fontSize = 8;
    float leading = 1.7;
    int height = lines.size() * fontSize * leading -1;
    int width = maxLineLength * fontSize;
    
    return ofRectangle(0, 0, width, height);
}

