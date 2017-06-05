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
    std::shared_ptr<SoundClipInfo> info;
public:
    
    MetaInfoPanel() = default;
    inline MetaInfoPanel(const std::shared_ptr<SoundClipInfo> & _info, const int& _width, const int& _height): info{_info} {
        fbo.allocate(_width, _height, GL_RGB);
    };

    void update() const;
    
    inline void draw() { fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };
    
};


#endif /* MetaInfoPanel_hpp */
