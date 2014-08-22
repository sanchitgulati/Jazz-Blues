//
//  Util.cpp
//  jazz
//
//  Created by Sanchit Gulati on 13/08/14.
//
//

#include "Util.h"

using namespace cocos2d;

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
