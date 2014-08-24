//
//  Block.h
//  adam
//
//  Created by Sanchit Gulati on 24/08/14.
//
//

#ifndef __adam__Block__
#define __adam__Block__

#include <iostream>
#include "cocos2d.h"
#include "LFSpriteNode.h"
#include "Resources.h"

class Block: public cocos2d::Node
{
private:
    LFSpriteNode* _sprite;
    int itemType;
public:
    Block();
    //    void reset(float x,float y);
    static Block* createFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height);
    ~Block();
    static Block* create(b2Body* body);
    bool initWithBody(b2Body* body);
    //    void update(float dt);
};
#endif /* defined(__adam__Block__) */
