//
//  Male.cpp
//  adameve
//
//  Created by Sanchit Gulati on 21/08/14.
//
//

#include <stdio.h>
#include "Male.h"

using namespace cocos2d;

bool Male::init()
{
    return true;
}

void Male::setSprite(cocos2d::Texture2D *tex)
{
    _sprite = Sprite::createWithTexture(tex);
    addChild(_sprite);
}