//
//  Win.cpp
//  adam
//
//  Created by Sanchit Gulati on 24/08/14.
//
//

#include "Win.h"
using namespace cocos2d;

Win::Win()
{
    
}

Win::~Win()
{
    
}

Win* Win::create(b2Body* body,bool alt)
{
    Win *pRet = new Win();
    if (pRet && pRet->initWithBody(body,alt))
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

bool Win::initWithBody(b2Body* body,bool alt)
{
    if(alt)
        _sprite = LFSpriteNode::create(IMG_WIN_ALT);
    else
        _sprite = LFSpriteNode::create(IMG_WIN);
    _sprite->setB2Body(body);
    _sprite->setPTMRatio(kPixelsPerMeter);
    this->addChild(_sprite);
    return true;
}


Win* Win::createFixture(b2World* world, TMXLayer* layer, int x, int y, float width, float height,bool alt)
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
                   (tileSize.width / kPixelsPerMeter) * 0.50f * width,
                   (tileSize.height / kPixelsPerMeter) * 0.50f * height
                   );
    
    
    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape; //changed from shape
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = true;
    fixtureDef.userData = (void*)(new userdataFormat(tmxWin));
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    //    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    
    
    return Win::create(body,alt);
    
}


void Win::fade()
{
    _sprite->runAction(FadeOut::create(1));
}

void Win::convert()
{
    auto random = DelayTime::create(Util::randf()*3);
    auto fall = CallFunc::create([this](){
        this->_sprite->getB2Body()->SetType(b2BodyType::b2_dynamicBody);
        this->_sprite->getB2Body()->GetFixtureList()[0].SetSensor(false);
    });
    _sprite->runAction(Sequence::create(random,fall, NULL));
}