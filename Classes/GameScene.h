//
//  GameScene.h
//  jazz
//
//  Created by Sanchit Gulati on 11/08/14.
//
//

#ifndef __jazz__GameScene__
#define __jazz__GameScene__

#include "cocos2d.h"
#include "GameObjects/Types.h"
#include "Util.h"
#include "Resources.h"
#include <iostream>
#include "Box2d/box2d.h"
#include "Button.h"
#include "GLESDebugDraw.h"

class GameScene : public cocos2d::Layer, public b2ContactListener
{
private:
    Player* _male;
    Player* _female;
    
    cocos2d::Node* _parent;
    cocos2d::Node* _playerGroup;
    cocos2d::Node* _platformsGroup;
    
    // stuff the player overlaps for overlap check will go here in this group!
    cocos2d::Node* _overlapGroup;
    
    cocos2d::Node* _bgGroup;
    cocos2d::Node* _fgGroup;
    
    // have we quit in some way?
    bool _quit;
    cocos2d::TMXTiledMap* _tm;
    
    bool _gsGamePlaying;
    
    int kCurrentLevel;
    
    //Box2d
    b2World* _world;
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    
    cocos2d::Point _origin;
    cocos2d::Size _visibleSize;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void update(float dt);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
protected:
    void loadData();
    void loadLevel(int level);
    void loadInstuctions();
    
    void setupObjects();
    void addObjects();
    
    void createPhysicalWorld();
    void prepareLayers();
    
    void createFixturesFirstPass(cocos2d::TMXLayer* layer);
    void createFixturesSecondPass(cocos2d::TMXLayer* layer);
    
    
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    //Rendering
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    
    GLESDebugDraw *_debugDraw;
    cocos2d::CustomCommand _customCommand;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif /* defined(__jazz__GameScene__) */
