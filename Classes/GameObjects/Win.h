//
//  Win.h
//  adam
//
//  Created by Sanchit Gulati on 24/08/14.
//
//

#ifndef __adam__Win__
#define __adam__Win__

#include <iostream>
#include "cocos2d.h"
#include "LFSpriteNode.h"
#include "Resources.h"
#include "Util.h"

class Win: public cocos2d::Node
{
private:
    LFSpriteNode* _sprite;
    int itemType;
public:
    Win();
    //    void reset(float x,float y);
    static Win* createFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height);
    ~Win();
    static Win* create(b2Body* body);
    bool initWithBody(b2Body* body);
    void convert();
    //    void update(float dt);
};

#endif /* defined(__adam__Win__) */
