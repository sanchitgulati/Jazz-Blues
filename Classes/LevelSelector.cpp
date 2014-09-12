//
//  ;
//  Adam
//
//  Created by Sanchit Gulati on 02/08/14.
//
//

#include "LevelSelector.h"
#include "GameScene.h"
#include "MainMenu.h"

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
        auto bgLeft = Sprite::create("images/bg.png");
        bgLeft->setColor(RGB_BLACK);
        bgLeft->setPosition(screenSize.width*0.75, screenSize.height/2);
        bgLeft->setScaleX(Util::getScreenRatioWidth(bgLeft)*0.5);
        bgLeft->setScaleY(Util::getScreenRatioHeight(bgLeft));
        this->addChild(bgLeft);
        
        auto bgRight = Sprite::create("images/bg.png");
        bgRight->setColor(RGB_WHITE);
        bgRight->setPosition(screenSize.width*0.25, screenSize.height/2);
        bgRight->setScaleX(Util::getScreenRatioWidth(bgRight)*0.5);
        bgRight->setScaleY(Util::getScreenRatioHeight(bgRight));
        this->addChild(bgRight);
        
        auto gameLogo = MenuItemImage::create(IMG_GAME_LOGO,IMG_GAME_LOGO,CC_CALLBACK_1(LevelSelector::menuCallback, this));
        gameLogo->setPosition(Point(screenSize.width/2,screenSize.height*0.80));
        gameLogo->setScale(Util::getScreenRatioHeight(gameLogo)*0.4);
        auto menu = Menu::create(gameLogo, NULL);
        menu->setPosition(Point(0,0));
        this->addChild(menu);
        
        //TODO: ANIMATION
        auto scaleDown = ScaleBy::create(VFX_CONSTANT, 0.3);
        auto moveUp = MoveTo::create(VFX_CONSTANT, Point(gameLogo->getPositionX(),screenSize.height*0.90));
        auto c = RGB_ROSE;
        auto roseIt = TintTo::create(VFX_CONSTANT, c.r, c.g, c.b);
        
        gameLogo->runAction(scaleDown);
        gameLogo->runAction(moveUp);
        gameLogo->runAction(roseIt);

        cocos2d::Vector<MenuItem *> sideAList;

        for (int i = 0; i < LVL_A; i++) {
            auto lbl = Label::createWithTTF(sideA[i].c_str(), FONT, 24);
            auto lblItem = MenuItemLabel::create(lbl, CC_CALLBACK_1(LevelSelector::menuACallback, this));
            lblItem->setPosition(screenSize.width*0.25, 50*(LVL_A-i));
            lblItem->setTag(i);
            lblItem->setColor(RGB_ROSE);
            sideAList.pushBack(lblItem);
        }
        
        auto menuSideA = Menu::createWithArray(sideAList);
        menuSideA->setPosition(screenSize.width*0.00, screenSize.height*0.10);
        this->addChild(menuSideA);
        
        cocos2d::Vector<MenuItem *> sideBList;
        
        for (int i = 0; i < LVL_B; i++) {
            auto lbl = Label::createWithTTF(sideB[i].c_str(), FONT, 24);
            auto lblItem = MenuItemLabel::create(lbl, CC_CALLBACK_1(LevelSelector::menuBCallback, this));
            lblItem->setPosition(screenSize.width*0.25, 50*(LVL_B-i));
            lblItem->setTag(i);
            lblItem->setColor(RGB_ROSE);
            sideBList.pushBack(lblItem);
        }
        
        auto menuSideB = Menu::createWithArray(sideBList);
        menuSideB->setPosition(screenSize.width*0.50, screenSize.height*0.10);
        this->addChild(menuSideB);
        
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
    switch (obj->getTag()) {
        default:
        {
            auto scene = (Scene*)MainMenu::create();
            Director::getInstance()->replaceScene(scene);
            break;
        }
    }
    
}

// a selector callback
void LevelSelector::menuACallback(cocos2d::Ref* pSender)
{
    auto obj = (Node*)pSender;
//    kCurrentLevel = obj->getTag();
    switch (obj->getTag()) {
        default:
        {
            auto scene = (Scene*)GameScene::create();
            Director::getInstance()->replaceScene(scene);
            break;
        }
    }
}

// a selector callback
void LevelSelector::menuBCallback(cocos2d::Ref* pSender)
{
    auto obj = (Node*)pSender;
//    kCurrentLevel = LVL_A + obj->getTag();
    switch (obj->getTag()) {
        default:
        {
            auto scene = (Scene*)GameScene::create();
            Director::getInstance()->replaceScene(scene);
            break;
        }
    }
    
}


void LevelSelector::onExit() {
    Layer::onExit();
}
void LevelSelector::update(float delta)
{
    
}