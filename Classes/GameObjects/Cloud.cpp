//
//  Cloud.cpp
//  badazz
//
//  Created by Sanchit Gulati on 17/08/14.
//
//

#include "Cloud.h"
using namespace cocos2d;

Cloud::Cloud()
{
    
}

Cloud::~Cloud()
{
    
}

Cloud* Cloud::create(b2Body* body,float scaleX,float scaleY)
{
    Cloud *pRet = new Cloud();
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

bool Cloud::initWithBody(b2Body* body,float scaleX,float scaleY)
{
    _sprite = LFSpriteNode::create(IMG_CLOUD);
    auto contentSize = _sprite->getContentSize();
    _sprite->setTextureRect(Rect(0, 0, contentSize.width*scaleX, contentSize.height*scaleY));
    _sprite->setB2Body(body);
    _sprite->setPTMRatio(kPixelsPerMeter);
    this->addChild(_sprite);
    
    auto animation = Animation::create();
    animation->setDelayPerUnit(0.5);
    
    
    animation->addSpriteFrameWithFile(IMG_CLOUD_0);
    animation->addSpriteFrameWithFile(IMG_CLOUD_1);
    animation->addSpriteFrameWithFile(IMG_CLOUD_2);
    animation->addSpriteFrameWithFile(IMG_CLOUD_1);
    animation->addSpriteFrameWithFile(IMG_CLOUD_0);
    auto animate = Animate::create(animation);
    _sprite->runAction(RepeatForever::create(animate));
     

    return true;
}


Cloud* Cloud::createFixture(b2World* world, TMXLayer* layer, int x, int y, float width, float height,int length,bool vertical)
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
    auto userdata = (void*)(new userdataFormat(tmxCloud));
    fixtureDef.userData = userdata;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    auto ret = Cloud::create(body,widthMultiplier,heightMultiplier);
    ((userdataFormat*)userdata)->node = ret;
    return ret;

}

void Cloud::del()
{
    auto random = DelayTime::create(Util::randf()*3);
    auto del = CallFunc::create([this](){
        auto world = this->_sprite->getB2Body()->GetWorld();
        world->DestroyBody(_sprite->getB2Body());
        _sprite->removeFromParentAndCleanup(true);
    });
    _sprite->runAction(Sequence::create(random,del, NULL));
}