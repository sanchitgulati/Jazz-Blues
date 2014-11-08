//
//  Teddy.h
//  adam
//
//  Created by Sanchit Gulati on 24/08/14.
//
//

#ifndef __adam__Teddy__
#define __adam__Teddy__

#include "cocos2d.h"
#include "LFSpriteNode.h"
#include "Resources.h"

class Teddy: public cocos2d::Node
{
private:
    LFSpriteNode* _sprite;
    int itemType;
public:
    Teddy();
    //    void reset(float x,float y);
    static Teddy* createFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height);
    ~Teddy();
    static Teddy* create(b2Body* body);
    bool initWithBody(b2Body* body);
    LFSpriteNode* getSprite();
};
#endif /* defined(__adam__Teddy__) */
