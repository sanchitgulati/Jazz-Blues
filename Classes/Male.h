//
//  Male.h
//  adameve
//
//  Created by Sanchit Gulati on 21/08/14.
//
//

#include "cocos2d.h"

class Male : public cocos2d::Node
{
public:
    cocos2d::Sprite* _sprite;
    bool _inverted;
    
    //Movements
    float mainSpeed;
    bool mainJumping;
    float jumpSpeedLimit;
    float jumpSpeed;
    
    //Map Lose ref
    cocos2d::TMXTiledMap* _map;
    
    CREATE_FUNC(Male);
    virtual bool init();
    void setSprite(cocos2d::Texture2D* tex,int x,int y,float width,float height);
    void setInverted(bool val = false);
    
    void setMapRef(cocos2d::TMXTiledMap* map);
    
    void moveLeft();
    void moveRight();
    void jump(bool start = false);
    
};