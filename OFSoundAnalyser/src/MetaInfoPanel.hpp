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
#include "ofMain.h"
#include "SoundClipInfo.hpp"
#include "constants.h"
#include "goodies.h"

class MetaInfoPanel {
    ofFbo fbo;
    SoundClipInfo info;
public:
    
    MetaInfoPanel() = default;
    MetaInfoPanel(const int&, const int&);

    void update(const SoundClipInfo& info);
    
    inline void draw() { fbo.draw(0, 0); ofTranslate(0, fbo.getHeight()); };
    
};


#endif /* MetaInfoPanel_hpp */
