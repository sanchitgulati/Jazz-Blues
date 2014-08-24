//
//  ;
//  Adam
//
//  Created by Sanchit Gulati on 02/08/14.
//
//

#include "MainMenu.h"
#include "GameScene.h"

using namespace cocos2d;

MainMenu::MainMenu() {
    
}

MainMenu::~MainMenu() {
    
}

bool MainMenu::init() {
    bool bRet = false;
    
    do {
        
        CC_BREAK_IF(! LayerColor::init());
        this->setColor(Color3B::WHITE);
        this->setOpacity(255);
        
        Size screenSize = Director::getInstance()->getWinSize();
        
        auto bg = Sprite::create("images/bg.png");
        bg->setColor(Color3B::BLACK);
        bg->setPosition(screenSize.width/2, screenSize.height/2);
        bg->setScaleX(Util::getScreenRatioWidth(bg));
        bg->setScaleY(Util::getScreenRatioHeight(bg));
        this->addChild(bg);
        
        _gameLabel = Label::createWithTTF("JAZZ AND BLUES", FONT, 72);
        _gameLabel->setPosition(Point(screenSize.width/2,screenSize.height*0.80));
        this->addChild(_gameLabel);
        
        
        auto labelPlay = Label::createWithTTF("CONTINUE", FONT, 64);
        auto menuItemPlay = MenuItemLabel::create(labelPlay, CC_CALLBACK_1(MainMenu::menuCallback, this));
        menuItemPlay->setPositionY(60);
        
        auto labelNew = Label::createWithTTF("NEW GAME", FONT, 46);
        auto menuItemNew = MenuItemLabel::create(labelNew, CC_CALLBACK_1(MainMenu::menuCallback, this));
        menuItemNew->setPositionY(5);
        
        
        auto menu = Menu::create(menuItemPlay,menuItemNew, NULL);
        menu->setPosition(screenSize.width/2, screenSize.height*0.30);
        this->addChild(menu);
        
        bRet = true;
    } while(0);
    
    
    this->scheduleUpdate();
    
    return bRet;
}


void MainMenu::onEnter() {
    
    Layer::onEnter();
    

}


// a selector callback
void MainMenu::menuCallback(cocos2d::Ref* pSender)
{
    auto obj = (Node*)pSender;
    log("Reached menuCallback from %d",obj->getTag());
    kCurrentLevel = 1;
    auto scene = (Scene*)GameScene::create();
    Director::getInstance()->replaceScene(scene);
    
}


void MainMenu::onExit() {
    Layer::onExit();
}
void MainMenu::update(float delta)
{
    
}