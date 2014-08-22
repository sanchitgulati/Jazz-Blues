//
//  Death.cpp
//  badazz
//
//  Created by Sanchit Gulati on 18/08/14.
//
//

#include "Death.h"

using namespace cocos2d;

Death::Death()
{
    
}

Death::~Death()
{
    
}

Death* Death::create(b2Body* body)
{
    Death *pRet = new Death();
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

bool Death::initWithBody(b2Body* body)
{
    _sprite = LFSpriteNode::create(IMG_SPIKE);
    _sprite->setColor(Color3B::RED);
    _sprite->setB2Body(body);
    _sprite->setPTMRatio(kPixelsPerMeter);
    _sprite->runAction(RepeatForever::create(RotateBy::create(2.0f, 360)));
    this->addChild(_sprite);
    return true;
}


Death* Death::createFixture(b2World* world, TMXLayer* layer, int x, int y, float width, float height)
{
    // get position & size
    auto p = layer->getPositionAt(Point(x,y));
    auto tileSize = layer->getMapTileSize(); // was _tm->getTileSize // Equal in our case
    // note: creating the 'world' member variable
    // is discussed in the next chapter
    // create the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(
                         (p.x + (tileSize.width / 2.0f)) / kPixelsPerMeter,
                         (p.y + (tileSize.height / 2.0f)) / kPixelsPerMeter
                         );
    b2Body* body = world->CreateBody(&bodyDef);
    
    
    // define the shape
    b2PolygonShape shape;
    shape.SetAsBox(
                   (tileSize.width / kPixelsPerMeter) * 0.35f * width,
                   (tileSize.width / kPixelsPerMeter) * 0.35f * height
                   );
    
    //define alternative shape
    b2CircleShape shapeC;
    shapeC.m_radius = (tileSize.width / kPixelsPerMeter) * 0.5f * width;
    
    
    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape; //changed from shape
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.userData = (void*)(new userdataFormat(tmxFire));
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    //    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    

    return Death::create(body);

}