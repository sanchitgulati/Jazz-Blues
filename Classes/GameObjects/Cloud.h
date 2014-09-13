//
//  Cloud.h
//  badazz
//
//  Created by Sanchit Gulati on 17/08/14.
//
//

#ifndef __badazz__Cloud__
#define __badazz__Cloud__

#include <iostream>
#include "cocos2d.h"
#include "box2d/box2d.h"
#include "Resources.h"
#include "LFSpriteNode.h"

class Cloud : public cocos2d::Node
{
private:
    LFSpriteNode* _sprite;
    int itemType;
public:
    Cloud();
//    void reset(float x,float y);
    static Cloud* createFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height,int length = 1,bool vertical = false);
    ~Cloud();
    static Cloud* create(b2Body* body,float scaleX,float scaleY);
    bool initWithBody(b2Body* body,float scaleX,float scaleY);
//    void update(float dt);
};
#endif /* defined(__badazz__Cloud__) */
