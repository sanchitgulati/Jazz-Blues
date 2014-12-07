//
//  Player.cpp
//  jazz
//
//  Created by Sanchit Gulati on 13/08/14.
//
//

#include "Player.h"
using namespace cocos2d;

Player* Player::create(b2Body* body,int pIndex)
{
    Player *pRet = new Player();
    if (pRet && pRet->initWithBody(body,pIndex))
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

bool Player::initWithBody(b2Body* body,int pIndex)
{
    _sprite = LFSpriteNode::create(IMG_WALL); //hack, transparent sprite
    if(pIndex == pFemale)
    {
        _sprite->setColor(RGB_GIRL);
        _shape = Sprite::create(IMG_FEMALE);
    }
    else if(pIndex == pMale)
    {
        _sprite->setColor(RGB_BOY);
        _shape = Sprite::create(IMG_MALE);
    }
    _shape->setColor(RGB_SKIN);
    
    _pIndex = pIndex;
    _sprite->setB2Body(body);
    _sprite->setPTMRatio(kPixelsPerMeter);
    _sprite->getTexture()->setAntiAliasTexParameters();
    addChild(_sprite);
    
    auto size = _sprite->getBoundingBox().size;
    _shape->setPosition(size.width/2.0, size.height/2.0);
    _sprite->addChild(_shape);
    
    
    _jumpHeight = 1536/kPixelsPerMeter;
    _gravity = -1000/kPixelsPerMeter;
    _maxspeed = Point(240/kPixelsPerMeter,1020/kPixelsPerMeter);
    
    //Add Physics Restrictions
        
    _time = 0;
    _died = false;
    _isAtFinish = false;
    
    _airTime = 0.0f;
    _jumping = false;
    _walking = false;
    
    
    _pressedLeft = false;
    _pressedRight = false;
    _pressedUp = false;
    
    _isPoisonStarted = false;
    _isPoisonFinished = false;
    
    _isTouchingFloor = 0;
    _invert = 1;
    _facing = directionRight;
    
    
    
    schedule(schedule_selector(Player::update));
    return true;
}

Player::Player()
{
}

Player::~Player()
{
    log("in ~");
}

Player::Player(float x,float y)
{
    log("in param constructor");
}


void Player::update(float dt)
{
//    _streak->setPosition(_sprite->getPosition());
    auto body = _sprite->getB2Body();
    
    //control mechanics
    if(_pressedLeft)
    {
        body->ApplyLinearImpulse(b2Vec2((-1*(_pIndex == pFemale ? -1 : 1)*_invert*_maxspeed.x)/4.0, 0),body->GetWorldCenter(), true);
        _facing = _pIndex == pFemale ? directionRight : directionLeft;
    }
    else if(_pressedRight)
    {
        body->ApplyLinearImpulse(b2Vec2((1*(_pIndex == pFemale ? -1 : 1)*_invert*_maxspeed.x)/4.0, 0),body->GetWorldCenter(), true);
        _facing = _pIndex == pFemale ? directionLeft : directionRight;
    }
    else
    {
        auto getVelocity = body->GetLinearVelocity();
        body->SetLinearVelocity(b2Vec2(getVelocity.x/16,getVelocity.y));
    }
    
    if(_pressedUp)
    {
        _pressedUp = false;
        _jumping = true;
        _walking = false;
        
        if(_isTouchingFloor > 0)
        {
            auto jumpHeight = b2Vec2(0,_jumpHeight);
            if(_isPoisonFinished)
                jumpHeight.y *= 1.2;
            body->ApplyLinearImpulse(jumpHeight,body->GetWorldCenter(),true);
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_JUMP);
        }
    }
    
    auto velocity = body->GetLinearVelocity();
    if(_jumping && velocity.y < 0)
    {
        body->SetTransform( body->GetPosition(), velocity.y/60 * (_facing == directionRight ? 1 : -1) );
        //angle equals to "invesrse of" velocity.y /60
    }
    else
    {
        body->SetTransform( body->GetPosition(), 0 );
        //angle reset to zero
    }
    
    auto finalVelocity = body->GetLinearVelocity();
    auto clampedVelocity = b2Vec2(clampf(finalVelocity.x,-1*_maxspeed.x,1*_maxspeed.x), clampf(finalVelocity.y, -1*_maxspeed.y, 1*_maxspeed.y));
    body->SetLinearVelocity(clampedVelocity);
    
    if(_isPoisonFinished)
    {
        auto finalVelocity = body->GetLinearVelocity();
        auto clampedVelocity = b2Vec2(clampf(finalVelocity.x,-0.25*_maxspeed.x,0.25*_maxspeed.x), clampf(finalVelocity.y, -2*_maxspeed.y, 2*_maxspeed.y));
        body->SetLinearVelocity(clampedVelocity);
    }
    
}


Player* Player::createPlayerFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height,int pIndex)
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
    
    
    auto widthHalf = (tileSize.width / kPixelsPerMeter) * 0.5f * width;
    auto heightHalf = (tileSize.height / kPixelsPerMeter) * 0.5f * height;
    
    // define the shape
    b2PolygonShape shape;
    shape.SetAsBox(
                   (tileSize.width / kPixelsPerMeter) * 0.5f * width * 0.90,
                   (tileSize.height / kPixelsPerMeter) * 0.5f * height *0.50
                   ,b2Vec2(0, widthHalf * 0.25),0);
    
    //define bottom
    b2CircleShape shapeC;
    shapeC.m_radius = (tileSize.width / kPixelsPerMeter) * 0.5f * width;
    shapeC.m_p = b2Vec2(0, -heightHalf*0.50);
    
    
    
    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shapeC; //changed from shape
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.userData = (void*)(new userdataFormat(sensorNone,pIndex));
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    //    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    
    
    //define top
    shapeC.m_radius = (tileSize.width / kPixelsPerMeter) * 0.5f * width *0.8;
    shapeC.m_p = b2Vec2(0, heightHalf*0.20);
    
    // create the fixture
    fixtureDef.shape = &shapeC; //changed from shape
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.userData = (void*)(new userdataFormat(sensorNone,pIndex));
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    //    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    
    
    fixtureDef.shape = &shape; //changed from shape
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.userData = (void*)(new userdataFormat(sensorNone,pIndex));
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    //    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    
    
    
    
    
    //prepare a shape definition
    b2PolygonShape sensorShape;
    
    b2FixtureDef sensorFixture;
    sensorFixture.shape = &sensorShape;
    sensorFixture.isSensor = true;
    sensorFixture.density = 0;
    
    b2Vec2 center;
//    //Right
//    center = b2Vec2(widthHalf, 0);
//    sensorFixture.userData = (void*)(new userdataFormat(sensorRight,pIndex));
//    sensorShape.SetAsBox(widthHalf/4, heightHalf/4.0, center, 0);
//    body->CreateFixture(&sensorFixture);
//    
//    //Left
//    center = b2Vec2(-widthHalf, 0);
//    sensorFixture.userData = (void*)(new userdataFormat(sensorLeft,pIndex));
//    sensorShape.SetAsBox(widthHalf/4, heightHalf/4.0, center, 0);
//    body->CreateFixture(&sensorFixture);
    
    //Bottom
    center = b2Vec2(0, -heightHalf);
    sensorFixture.userData = (void*)(new userdataFormat(sensorBottom,pIndex));
    sensorShape.SetAsBox(widthHalf/2, heightHalf/4.0, center, 0);
    body->CreateFixture(&sensorFixture);
    
    
    auto ret = Player::create(body,pIndex);
    return ret;
}

void Player::setIsGround(bool val)
{
    if(val == true)
    {
        _isTouchingFloor ++;
        if(_airTime > 1.2)
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_OUCH);
        }
        _jumping = false;
        _airTime = 0.0;
    }
    else
    {
        _isTouchingFloor --;
        log("Checking _isTouchingFloor %d",_isTouchingFloor);
    }
}

void Player::setSpriteNode(LFSpriteNode *node)
{
    _sprite = node;
}

LFSpriteNode* Player::getSpriteNode()
{
    return _sprite;
}

b2Body* Player::getB2Body()
{
    return _sprite->getB2Body();
}


LFSpriteNode* Player::getSprite()
{
    return _sprite;
}

bool Player::getIsAlive()
{
    return !_died;
}

void Player::setIsDied(bool val)
{
    _died = val;
}

void Player::setAtFinish(bool val)
{
    _isAtFinish = val;
}

bool Player::getAtFinish()
{
    return _isAtFinish;
}

void Player::invert()
{
    _invert *= -1;
}

void Player::poison()
{
    if(!_isPoisonStarted)
    {
        _isPoisonStarted = true;
        auto callFunc = CallFunc::create([this](){
            this->_isPoisonFinished = true;
        });
        auto tint = TintTo::create(5, 0, 162, 0);
        _shape->runAction(Sequence::create(tint,callFunc, NULL));
    }
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(!_died)
    {
        switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                _pressedUp = true;
                break;
            case  EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                _pressedLeft = true;
                break;
            case  EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                _pressedRight = true;
                break;
            default:
                break;
        }
    }
    
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    switch (keyCode) {
        case  EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _pressedLeft = false;
            break;
        case  EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _pressedRight = false;
            break;
        default:
            break;
    }
}


void Player::onButtonPressed(void* userData)
{
    auto index = static_cast<userdataFormat *>(userData)->a;
    switch (index) {
        case bLeft:
            _pressedLeft = true;
            break;
        case bRight:
            _pressedRight = true;
            break;
        case bUp:
            _pressedUp = true;
            break;
        default:
            break;
    }
}


void Player::onButtonReleased(void* userData)
{
    auto index = static_cast<userdataFormat *>(userData)->a;
    switch (index) {
        case bLeft:
            _pressedLeft = false;
            break;
        case bRight:
            _pressedRight = false;
            break;
        default:
            break;
    }
}