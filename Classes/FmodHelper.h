//
//  FmodHelper.h
//  jazz
//
//  Created by Sanchit Gulati on 18/01/15.
//
//

#ifndef __jazz__FmodHelper__
#define __jazz__FmodHelper__

#include <stdio.h>
#include "cocos2d.h"
#include "fmod/fmod.hpp"
#include "fmod/fmod_studio.hpp"
#include "fmod/fmod_errors.h"

using namespace FMOD;

class FmodHelper
{
private:
    Studio::System* system;
    Studio::MixerStrip* mixer;
    std::vector<std::pair<std::string,Studio::EventInstance*>> events;
public:
    static FmodHelper* instance;
    static FmodHelper* getInstance();
    static bool instanceFlag;
    bool init();
    bool loadEvent(std::string eventName);
    bool playEvent(std::string eventName);
    bool changeParam(std::string eventName,std::string paramName,float val = 1.0);
    void update();
    void mute();
    void unmute();
    
};

#endif /* defined(__jazz__FmodHelper__) */
