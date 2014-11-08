//
//  Teddy.cpp
//  adam
//
//  Created by Sanchit Gulati on 24/08/14.
//
//

#include "Teddy.h"

using namespace cocos2d;

Teddy::Teddy()
{
    
}

Teddy::~Teddy()
{
    
}

Teddy* Teddy::create(b2Body* body)
{
    Teddy *pRet = new Teddy();
    if (pRet && pRet->initWithBody(body))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool Teddy::initWithBody(b2Body* body)
{
    _sprite = LFSpriteNode::create(IMG_TEDDY);
    _sprite->setB2Body(body);
    _sprite->setPTMRatio(kPixelsPerMeter);
    this->addChild(_sprite);
    return true;
}


Teddy* Teddy::createFixture(b2World* world, TMXLayer* layer, int x, int y, float width, float height)
{
    // get position & size
    auto p = layer->getPositionAt(Point(x,y));
    auto tileSize = layer->getMapTileSize(); // was _tm->getTileSize // Equal in our case
    // note: creating the 'world' member variable
    // is discussed in the next chapter
    // create the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(
                         (p.x + (tileSize.width / 2.0f)) / kPixelsPerMeter,
                         (p.y + (tileSize.height / 2.0f)) / kPixelsPerMeter
                         );
    b2Body* body = world->CreateBody(&bodyDef);
    
    
    // define the shape
    b2PolygonShape shape;
    shape.SetAsBox(
                   (tileSize.width / kPixelsPerMeter) * 0.50f * width,
                   (tileSize.height / kPixelsPerMeter) * 0.50f * height
                   );
    
    
    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape; //changed from shape
    fixtureDef.density = 5.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.userData = (void*)(new userdataFormat(tmxTeddy));
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    //    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    
    
    return Teddy::create(body);
}

LFSpriteNode* Teddy::getSprite()
{
    return _sprite;
}