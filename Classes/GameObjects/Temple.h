//
//  Win.h
//  adam
//
//  Created by Sanchit Gulati on 24/08/14.
//
//

#ifndef __adam__Temple__
#define __adam__Temple__

#include <iostream>
#include "cocos2d.h"
#include "LFSpriteNode.h"
#include "Resources.h"

class Temple: public cocos2d::Node
{
private:
    LFSpriteNode* _sprite;
    int itemType;
public:
    Temple();
    //    void reset(float x,float y);
    static Temple* createFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height);
    ~Temple();
    static Temple* create(b2Body* body);
    bool initWithBody(b2Body* body);
    //    void update(float dt);
};

#endif /* defined(__adam__Temple__) */
