//
//  Platform.cpp
//  badazz
//
//  Created by Sanchit Gulati on 17/08/14.
//
//

#include "Police.h"
using namespace cocos2d;

Police::Police()
{
    
}

Police::~Police()
{
    
}

Police* Police::create(b2Body* body,float scaleX,float scaleY)
{
    Police *pRet = new Police();
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

bool Police::initWithBody(b2Body* body,float scaleX,float scaleY)
{
    _sprite = LFSpriteNode::create(IMG_POLICE);
    _sprite->setColor(RGB_SKIN);
    auto contentSize = _sprite->getContentSize();
    _sprite->setTextureRect(cocos2d::Rect(0, 0, contentSize.width*scaleX, contentSize.height*scaleY));
    _sprite->setB2Body(body);
    _sprite->setPTMRatio(kPixelsPerMeter);
    this->addChild(_sprite);
    _sprite->runAction(RepeatForever::create(Sequence::create(DelayTime::create(5),FlipX::create(true),DelayTime::create(5),FlipX::create(false),NULL)));
    return true;
}


Police* Police::createFixture(b2World* world, TMXLayer* layer, int x, int y, float width, float height,int length,bool vertical)
{
    auto widthMultiplier = MAX((length*(!vertical)), 1);
    auto heightMultiplier = MAX((length*(vertical)), 1);
    // get position & size
    auto p = layer->getPositionAt(cocos2d::Point(x,y));
    auto tileSize = layer->getMapTileSize(); // was _tm->getTilesize
    // note: creating the 'world' member variable
    // is discussed in the next chapter
    // create the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(
                         (p.x + (tileSize.width * widthMultiplier / 2.0f)) / kPixelsPerMeter,
                         (p.y + (vertical*tileSize.height/2) + (-1*vertical)*(tileSize.height * heightMultiplier / 2.0f)) / kPixelsPerMeter
                         );
    b2Body* body = world->CreateBody(&bodyDef);
    // define the shape
    b2PolygonShape shape;
    shape.SetAsBox(
                   ((tileSize.width / kPixelsPerMeter) * 0.5f * width)* widthMultiplier,
                   ((tileSize.height / kPixelsPerMeter) * 0.5f * height)* heightMultiplier
                   );
    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.userData = (void*)(new userdataFormat(tmxPolice));
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.isSensor = true;
//    fixtureDef.filter.categoryBits = 0x0002;
//    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    return Police::create(body,widthMultiplier,heightMultiplier);

}