//
//  Platform.cpp
//  badazz
//
//  Created by Sanchit Gulati on 17/08/14.
//
//

#include "Platform.h"
using namespace cocos2d;

Platform::Platform()
{
    
}

Platform::~Platform()
{
    
}

Platform* Platform::create(b2Body* body,float scaleX,float scaleY)
{
    Platform *pRet = new Platform();
    if (pRet && pRet->initWithBody(body, scaleX, scaleY))
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

bool Platform::initWithBody(b2Body* body,float scaleX,float scaleY)
{
    _sprite = LFSpriteNode::create(IMG_PLATFORM);
    _sprite->setColor(Color3B::BLACK);
    _sprite->setB2Body(body);
    _sprite->setScaleX(scaleX);
    _sprite->setScaleY(scaleY);
    _sprite->setPTMRatio(kPixelsPerMeter);
    this->addChild(_sprite);
    return true;
}


Platform* Platform::createFixture(b2World* world, TMXLayer* layer, int x, int y, float width, float height,int length,bool vertical)
{
    auto widthMultiplier = MAX((length*(!vertical)), 1);
    auto heightMultiplier = MAX((length*(vertical)), 1);
    // get position & size
    auto p = layer->getPositionAt(Point(x,y));
    auto tileSize = layer->getMapTileSize(); // was _tm->getTilesize
    // note: creating the 'world' member variable
    // is discussed in the next chapter
    // create the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(
                         (p.x + (tileSize.width * widthMultiplier / 2.0f)) / kPixelsPerMeter,
                         (p.y + tileSize.height/2 + (vertical*tileSize.height/2) + (-1*vertical)*(tileSize.height * heightMultiplier / 2.0f)) / kPixelsPerMeter
                         );
    b2Body* body = world->CreateBody(&bodyDef);
    // define the shape
    b2PolygonShape shape;
    shape.SetAsBox(
                   ((tileSize.width / kPixelsPerMeter) * 0.5f * width)* widthMultiplier,
                   ((tileSize.width / kPixelsPerMeter) * 0.5f * height)* heightMultiplier
                   );
    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.userData = (void*)(new userdataFormat(tmxPlatform));
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    return Platform::create(body,widthMultiplier,heightMultiplier);

}