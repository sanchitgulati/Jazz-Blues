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
    if(pIndex == pFemale)
    {
        _sprite= LFSpriteNode::create(IMG_FEMALE);
    }
    else if(pIndex == pMale)
    {
        _sprite= LFSpriteNode::create(IMG_MALE);
    }
    
    _pIndex = pIndex;
    _sprite->setB2Body(body);
    _sprite->setPTMRatio(kPixelsPerMeter);
    _sprite->getTexture()->setAntiAliasTexParameters();
    addChild(_sprite);
    
    
    _jumpHeight = 560/kPixelsPerMeter;
    _gravity = -1000/kPixelsPerMeter;
    _maxspeed = Point(120/kPixelsPerMeter,360/kPixelsPerMeter);
    
    //Add Physics Restrictions
    _lastWall = sensorNone;
    _wallStickTime = 0.0f;
    _wallStickTimer = false;
    
    _wallJumping = false;
    _wallFloorJump = false;
    _wallHit = Point::ZERO;
        
    _time = 0;
    _died = false;
    _isAtFinish = false;
    
    _airTime = 0.0f;
    _jumping = false;
    _wallJumping = false;
    _walking = false;
    
    
    _pressedLeft = false;
    _pressedRight = false;
    _pressedUp = false;
    
    _isTouchingFloor = false;
    _isTouchingWall = false;
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
        body->ApplyLinearImpulse(b2Vec2((-1*(_pIndex == pFemale ? -1 : 1)*_maxspeed.x)/4.0, 0),body->GetWorldCenter(), true);
        _facing = directionLeft;
    }
    else if(_pressedRight)
    {
        body->ApplyLinearImpulse(b2Vec2((1*(_pIndex == pFemale ? -1 : 1)*_maxspeed.x)/4.0, 0),body->GetWorldCenter(), true);
        _facing = directionRight;
    }
    else
    {
        auto getVelocity = body->GetLinearVelocity();
        body->SetLinearVelocity(b2Vec2(getVelocity.x/16,getVelocity.y));
    }
    
    if(_pressedUp || _doJump)
    {
        _pressedUp = false;
        _doJump = false;
        
        _jumping = true;
        _walking = false;
        
        if(_isTouchingFloor)
        {
            auto jumpHeight = b2Vec2(0,_jumpHeight);
            body->ApplyLinearImpulse(jumpHeight,body->GetWorldCenter(),true);
            
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sfx/jump_0.mp3");
            //sound jump
        }
        }
    
    if(_facing == directionRight)
    {
        auto velocity = body->GetLinearVelocity();
        if(_jumping && velocity.y < 0)
        {
           body->SetTransform( body->GetPosition(), velocity.y/60 );
            //angle equals to "invesrse of" velocity.y /60
        }
        else
        {
            body->SetTransform( body->GetPosition(), 0 );
            //angle reset to zero
        }
    }
    else if(_facing == directionLeft)
    {
        auto velocity = body->GetLinearVelocity();
        if(_jumping && velocity.y < 0)
        {
            body->SetTransform( body->GetPosition(), -1*velocity.y/60 );
        }
        else
        {
            body->SetTransform( body->GetPosition(), 0 );
        }
    }
    
    auto finalVelocity = body->GetLinearVelocity();
    auto clampedVelocity = b2Vec2(clampf(finalVelocity.x, -4*_maxspeed.x, 4*_maxspeed.x), clampf(finalVelocity.y, -4*_maxspeed.y, 4*_maxspeed.y));
    body->SetLinearVelocity(clampedVelocity);
    
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
    
    
    // define the shape
    b2PolygonShape shape;
    shape.SetAsBox(
                   (tileSize.width / kPixelsPerMeter) * 0.5f * width,
                   (tileSize.width / kPixelsPerMeter) * 0.5f * height
                   );
    
    //define alternative shape
    b2CircleShape shapeC;
    shapeC.m_radius = (tileSize.width / kPixelsPerMeter) * 0.5f * width;
    
    
    // create the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape; //changed from shape
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.userData = (void*)(new userdataFormat(sensorNone,pIndex));
    fixtureDef.restitution = 0.0f;
    //    fixtureDef.filter.categoryBits = kFilterCategoryLevel;
    //    fixtureDef.filter.maskBits = 0xffff;
    body->CreateFixture(&fixtureDef);
    
    auto widthHalf = (tileSize.width / kPixelsPerMeter) * 0.5f * width;
    auto heightHalf = (tileSize.height / kPixelsPerMeter) * 0.5f * height;
    
    
    //prepare a shape definition
    b2PolygonShape sensorShape;
    
    b2FixtureDef sensorFixture;
    sensorFixture.shape = &sensorShape;
    sensorFixture.isSensor = true;
    sensorFixture.density = 0;
    
    b2Vec2 center;
    //Right
    center = b2Vec2(widthHalf, 0);
    sensorFixture.userData = (void*)(new userdataFormat(sensorRight,pIndex));
    sensorShape.SetAsBox(widthHalf/4, heightHalf/4.0, center, 0);
    body->CreateFixture(&sensorFixture);
    
    //Left
    center = b2Vec2(-widthHalf, 0);
    sensorFixture.userData = (void*)(new userdataFormat(sensorLeft,pIndex));
    sensorShape.SetAsBox(widthHalf/4, heightHalf/4.0, center, 0);
    body->CreateFixture(&sensorFixture);
    
    //Bottom
    center = b2Vec2(0, -heightHalf);
    sensorFixture.userData = (void*)(new userdataFormat(sensorBottom,pIndex));
    sensorShape.SetAsBox(widthHalf/2, heightHalf/4.0, center, 0);
    body->CreateFixture(&sensorFixture);
    
    //Top
    center = b2Vec2(0, heightHalf);
    sensorFixture.userData = (void*)(new userdataFormat(sensorTop,pIndex));
    sensorShape.SetAsBox(widthHalf/4, heightHalf/4.0, center, 0);
    body->CreateFixture(&sensorFixture);
    
    auto ret = Player::create(body,pIndex);
    return ret;
}

void Player::setIsTouching(int sensorEnum[],int val,bool died)
{
    if(died)
    {
        this->setIsDied(true);
    }
    for(int i =0;i < 4;i ++)
    {
        switch (sensorEnum[i]) {
            case sensorBottom:
                if(val == true)
                {
                    if(_airTime > 1.2) //1.2 is hardcoded
                    {
                        //shake screen
                        //hurt
                        //play sound
                        log("Ouch!");
                    }
                    _jumping = false;
                    _airTime = 0.0;
                    _wallFloorJump = false;
                    //play sound
                }
                _isTouchingFloor = val;
                break;
            case sensorLeft:
            case sensorRight:
                _isTouchingWall = val;
                if (val == true) {
                    _lastWall = sensorEnum[i];
                    
                    _walking = false;
                    _jumping = false;
                    _airTime = 0;
                    _wallStickTimer = true;
                }
                else
                {
                    _lastWall = sensorNone;
                }
                break;
            default:
                break;
        }
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
        case bUp:
            _pressedUp = false;
            break;
        default:
            break;
    }
}