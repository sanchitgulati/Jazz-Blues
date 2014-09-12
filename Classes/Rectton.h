//
//  Rectton.h
//  roundels
//
//  Created by Sanchit Gulati on 08/06/14.
//
//

#ifndef __roundels__Rectton__
#define __roundels__Rectton__

#include <iostream>
#include "cocos2d.h"
#include "Constants.h"
#include "Util.h"


class Rectton : public cocos2d::MenuItem
{
    cocos2d::Sprite* backSprite;
    cocos2d::Label* lblText;
    float _originalScale;
    Point _originalPosition;
public:
    Rectton(std::string text,Color3B color);
    static Rectton* create(std::string text,Color3B color);
    void selected();
    void unselected();
    
};

#endif /* defined(__roundels__Rectton__) */