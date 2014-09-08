//
//  ;
//  Adam
//
//  Created by Sanchit Gulati on 02/08/14.
//
//

#include "LevelSelector.h"
#include "GameScene.h"

using namespace cocos2d;

LevelSelector::LevelSelector() {
    
}

LevelSelector::~LevelSelector() {
    
}

bool LevelSelector::init() {
    bool bRet = false;
    
    do {
        
        CC_BREAK_IF(! LayerColor::init());
        this->setColor(Color3B::WHITE);
        this->setOpacity(255);
        
        Size screenSize = Director::getInstance()->getWinSize();
        
        
        bRet = true;
    } while(0);
    
    
    this->scheduleUpdate();
    
    return bRet;
}


void LevelSelector::onEnter() {
    
    Layer::onEnter();
    

}


// a selector callback
void LevelSelector::menuCallback(cocos2d::Ref* pSender)
{
    auto obj = (Node*)pSender;
    log("Reached menuCallback from %d",obj->getTag());
    kCurrentLevel = 1;
    auto scene = (Scene*)GameScene::create();
    Director::getInstance()->replaceScene(scene);
    
}


void LevelSelector::onExit() {
    Layer::onExit();
}
void LevelSelector::update(float delta)
{
    
}