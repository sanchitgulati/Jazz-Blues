//
//  Util.h
//  jazz
//
//  Created by Sanchit Gulati on 13/08/14.
//
//

#ifndef __jazz__Util__
#define __jazz__Util__

#include <iostream>
#include "cocos2d.h"

class Util
{
public:
    static float getScreenRatio(cocos2d::Node*);
    static float getScreenRatioWidth(cocos2d::Node*);
    static float getScreenRatioHeight(cocos2d::Node*);
};
#endif /* defined(__jazz__Util__) */
