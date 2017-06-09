//
//  MetaInfoPanel.hpp
//  OFSoundAnalyser
//
//  Created by Yasushi Sakai on 2017/05/27.
//
//

#ifndef MetaInfoPanel_hpp
#define MetaInfoPanel_hpp

#include <stdio.h>
#include <memory>
#include "ofMain.h"
#include "SoundClipInfo.hpp"
#include "constants.h"
#include "goodies.h"

class MetaInfoPanel {
    ofFbo fbo;
    std::string filename;
    std::shared_ptr<SoundClipInfo> info;
public:
    
    MetaInfoPanel() = default;
    
    inline MetaInfoPanel(const int& _width, const int& _height) {
        fbo.allocate(_width, _height, GL_RGB);
    };
    
    inline void changeFile (const std::shared_ptr<SoundClipInfo> & newInfo) { info = newInfo; };

    void update() const;
    
    inline void draw() { fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };
    
};


#endif /* MetaInfoPanel_hpp */
