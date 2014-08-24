//
//  Player.h
//  jazz
//
//  Created by Sanchit Gulati on 13/08/14.
//
//

#ifndef __jazz__Player__
#define __jazz__Player__

#include <iostream>
#include "cocos2d.h"
#include "box2d/box2d.h"
#include "LFSpriteNode.h"
#include "Resources.h"


class Player : public cocos2d::Node
{
private:
    float _jumpHeight;
    bool _wallStickTimer;
    float _wallStickTime;
    int _lastWall;
    bool _wallJumping;
    float _gravity;
    cocos2d::Point _maxspeed;
    bool _wallFloorJump;
    cocos2d::Point _wallHit;
    
    LFSpriteNode* _sprite;
    
    int _pIndex;
    
    bool _isAtFinish;
    
    int _health;
    
    bool _died;
    
    float _time;
    float _airTime;
    bool _jumping;
    bool _walking;
    
    bool _doJump;
    
    bool _pressedLeft;
    bool _pressedRight;
    bool _pressedUp;
    
    int _facing;
    
    bool _isTouchingFloor;
    bool _isTouchingWall;
    
    bool _isTouchingWallLeft;
    bool _isTouchingWallRight;
    
protected:
public:
    Player();
    Player(float x,float y);
    bool initWithBody(b2Body* body,int pIndex);
    
    LFSpriteNode* getSpriteNode();
    void setSpriteNode(LFSpriteNode* node);
    b2Body* getB2Body();
    LFSpriteNode* getSprite();
    
    void setIsTouching(int sensorEnum[],int val = true,bool died = false);
    bool getIsAlive();
    void setIsDied(bool val);
    
    static Player* createPlayerFixture(b2World* world,cocos2d::TMXLayer* layer, int x, int y, float width, float height,int pIndex);
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    void setAtFinish(bool val);
    bool getAtFinish();
    
    void update(float dt);
    ~Player();
    static Player* create(b2Body* body,int pIndex);
};

#endif /* defined(__jazz__Player__) */
