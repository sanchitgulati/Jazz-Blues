//
//  Death.h
//  badazz
//
//  Created by Sanchit Gulati on 18/08/14.
//
//

#ifndef __badazz__Death__
#define __badazz__Death__

#include <iostream>
#include "cocos2d.h"
#include "box2d/box2d.h"
#include "Resources.h"
#include "LFSpriteNode.h"

class Death : public cocos2d::Node
{
private:
    LFSpriteNode* _sprite;
    int itemType;
public:
    Death();
    //    void reset(float x,float y);
    static Death* createFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height);
    ~Death();
    static Death* create(b2Body* body);
    bool initWithBody(b2Body* body);
    //    void update(float dt);
};
#endif /* defined(__badazz__Death__) */
