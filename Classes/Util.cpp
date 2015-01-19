//
//  Util.cpp
//  jazz
//
//  Created by Sanchit Gulati on 13/08/14.
//
//

#include "Util.h"

using namespace cocos2d;

int GlobalClass::qouteLose = 0;
int GlobalClass::qouteWin = 0;
int GlobalClass::soundtrack = 0;
int GlobalClass::lock = 0;

float Util::getScreenRatio(Node* obj)
{
    Size winSize = Director::getInstance()->getWinSize();
    if(winSize.width > winSize.height)
    {
        return (winSize.height/obj->getBoundingBox().size.height);
    }
    else
    {
        return (winSize.width/obj->getBoundingBox().size.width);
    }
}

float Util::getScreenRatioWidth(Node* obj)
{
    
    Size winSize = Director::getInstance()->getWinSize();
    return (winSize.width/obj->getBoundingBox().size.width);
    
}

float Util::getScreenRatioHeight(Node* obj)
{
    
    Size winSize = Director::getInstance()->getWinSize();
    return (winSize.height/obj->getBoundingBox().size.height);
}

float Util::randf()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

bool Util::toss()
{
    return ((Util::randf() > 0.5 ) ? true : false);
}