//
//  Platform.h
//  badazz
//
//  Created by Sanchit Gulati on 17/08/14.
//
//

#ifndef __badazz__Society__
#define __badazz__Society__

#include <iostream>
#include "cocos2d.h"
#include "box2d/box2d.h"
#include "Resources.h"
#include "Util.h"
#include "LFSpriteNode.h"

class Society : public cocos2d::Node
{
private:
    LFSpriteNode* _sprite;
    int itemType;
    float lastLaugh;
    bool _notMoving;
public:
    Society();
    static Society* createFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height,int length = 1,bool vertical = false);
    ~Society();
    static Society* create(b2Body* body,float scaleX,float scaleY);
    bool initWithBody(b2Body* body,float scaleX,float scaleY);
    void laugh();
    void setDynamic();
    void move();
    cocos2d::Vec2 getPos();
    void update(float dt);
};
#endif /* defined(__badazz__Society__) */
