//
//  Platform.cpp
//  badazz
//
//  Created by Sanchit Gulati on 17/08/14.
//
//

#include "Society.h"
using namespace cocos2d;

Society::Society()
{
    lastLaugh = 0.0f;
}

Society::~Society()
{
    
}

Society* Society::create(b2Body* body,float scaleX,float scaleY)
{
    Society *pRet = new Society();
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

bool Society::initWithBody(b2Body* body,float scaleX,float scaleY)
{
    _sprite = LFSpriteNode::create(IMG_SOCIETY);
    _sprite->setColor(RGB_WHITE);
    auto contentSize = _sprite->getContentSize();
    _sprite->setTextureRect(Rect(0, 0, contentSize.width*scaleX, contentSize.height*scaleY));
    _sprite->setB2Body(body);
    _sprite->setPTMRatio(kPixelsPerMeter);
    this->addChild(_sprite);
    scheduleUpdate();
    return true;
}


Society* Society::createFixture(b2World* world, TMXLayer* layer, int x, int y, float width, float height,int length,bool vertical)
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
                   ((tileSize.height / kPixelsPerMeter) * 0.5f * height)* heightMultiplier
                   );
    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.userData = (void*)(new userdataFormat(tmxSociety));
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = true;
    fixtureDef.restitution = 0.0f;
    fixtureDef.filter.categoryBits = 0x0004;
//    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    return Society::create(body,widthMultiplier,heightMultiplier);
    
}

void Society::update(float dt)
{
    lastLaugh+=dt;
}

void Society::laugh()
{
    if(lastLaugh <= 2.0f)
        return;
    
    lastLaugh = 0.0f;
    int noOfLaugh = 3;
    for(int i = 0; i < noOfLaugh;i++)
    {
        auto val1 = (Util::toss() ? -1 : 1) * Util::randf()*_sprite->getBoundingBox().size.width/2;
        auto val2 = (Util::toss() ? -1 : 1) * Util::randf()*_sprite->getBoundingBox().size.height/4;
        auto val3 = Util::randf()+0.5;
        auto l = Sprite::create(IMG_LAUGH);
        l->setPosition(_sprite->getBoundingBox().size.width/2 + val1,_sprite->getBoundingBox().size.height + val2);
        auto callFunc = CallFunc::create([this,l](){this->removeChild(l);});
        l->runAction(Sequence::create(FadeOut::create(val3),callFunc, NULL));
        l->runAction(MoveBy::create(val3-0.2, Vec2(0, 5)));
        _sprite->addChild(l);
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_HAHA);
        log("va1 %f  va2 %f  va3 %f",val1,val2,val3);
    }
}


void Society::move()
{
    auto pos = _sprite->getB2Body()->GetPosition();
    pos.x += 0.1;
    _sprite->getB2Body()->SetTransform(pos, 0);
}

