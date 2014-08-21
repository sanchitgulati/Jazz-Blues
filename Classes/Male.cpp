//
//  Male.cpp
//  adameve
//
//  Created by Sanchit Gulati on 21/08/14.
//
//

#include <stdio.h>
#include "Male.h"

using namespace cocos2d;

bool Male::init()
{
    _inverted = false;
    setAnchorPoint(Point(0.5,0.5));
    
    auto dn = DrawNode::create();
    dn->drawDot(getPosition(), 2, Color4F::RED);
    addChild(dn,99);
    
    mainSpeed = 7;
    jumpSpeedLimit = 15;
    jumpSpeed = jumpSpeedLimit;
    
    return true;
}

void Male::setSprite(cocos2d::Texture2D *tex,int x,int y,float width,float height)
{
    _sprite = Sprite::createWithTexture(tex,Rect(x, y, width, height));
//    _sprite->setPosition(width/2, height/2);
    _sprite->setAnchorPoint(Point(0,0));
    addChild(_sprite);
}

void Male::setInverted(bool val)
{
    _inverted = true;
}

void Male::jump(bool start)
{
    if(!mainJumping && start)
    {
        mainJumping = true;
        jumpSpeed = jumpSpeedLimit;
        this->setPositionY(getPositionY() + jumpSpeed);
    }
    else
    {
        if(jumpSpeed < 0){
            jumpSpeed *= 1 - jumpSpeedLimit/75;
            if(jumpSpeed > -jumpSpeedLimit/5){
                jumpSpeed *= -1;
            }
        }
        if(jumpSpeed > 0 && jumpSpeed <= jumpSpeedLimit){
            jumpSpeed *= 1 + jumpSpeedLimit/50;
        }
        this->setPositionY(getPositionY() + (-1*jumpSpeed));
        //if main hits the floor, then stop jumping
        //of course, we'll change this once we create the level
    }
}

void Male::moveLeft()
{
    auto newPosition = getPositionX() - mainSpeed*(_inverted ? -1 : 1);
    auto newPositionTileLeft = newPosition/32;
    auto GID = _map->getLayer("level")->getTileGIDAt(Point(newPositionTileLeft,getPositionY()/32));
    if(GID == 1 || GID == 2)
    {		
        
    }
    else
    {
        this->setPositionX(newPosition);
    }
}

void Male::moveRight()
{
    auto newPosition = getPositionX() + mainSpeed*(_inverted ? -1 : 1);
    auto newPositionTileRight = newPosition/32;
    
    auto GID = _map->getLayer("level")->getTileGIDAt(Point(newPositionTileRight,getPositionY()/32));
    if(GID == 1 || GID == 2)
    {
        
    }
    else
    {
        this->setPositionX(newPosition);
    }
}

void Male::setMapRef(cocos2d::TMXTiledMap *map)
{
    _map = map;
}

