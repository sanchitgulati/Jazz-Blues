//
//  Male.h
//  adameve
//
//  Created by Sanchit Gulati on 21/08/14.
//
//

#include "cocos2d.h"

class Male : public cocos2d::Node
{
public:
    cocos2d::Sprite* _sprite;
    CREATE_FUNC(Male);
    virtual bool init();
    void setSprite(cocos2d::Texture2D* tex);
};