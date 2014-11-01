//
//  ;
//  Adam
//
//  Created by Sanchit Gulati on 02/08/14.
//
//

#include "MainMenu.h"
#include "GameScene.h"
#include "LevelSelector.h"

using namespace cocos2d;

enum {bPlay,bLevelSelect};

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
        
        auto bg = Sprite::create("images/paper.png",cocos2d::Rect(0,0,screenSize.width,screenSize.height));
        bg->setPosition(screenSize.width/2, screenSize.height/2);
        bg->getTexture()->setTexParameters({GL_LINEAR, GL_LINEAR,GL_REPEAT,GL_REPEAT});
        this->addChild(bg);
        
        auto gameLogo = Sprite::create(IMG_GAME_LOGO);
        gameLogo->setPosition(Point(screenSize.width/2,screenSize.height*0.80));
        gameLogo->setScale(Util::getScreenRatioHeight(gameLogo)*0.4);
        this->addChild(gameLogo);
        
        
        auto labelPlay = Label::createWithTTF("Continue", FONT, 64);
        labelPlay->setColor(RGB_BLACK);
        auto menuItemPlay = MenuItemLabel::create(labelPlay, CC_CALLBACK_1(MainMenu::menuCallback, this));
        menuItemPlay->setPositionY(120);
        menuItemPlay->setTag(bPlay);
        
        auto labelNew = Label::createWithTTF("Level Select", FONT, 46);
        labelNew->setColor(RGB_BLACK);
        auto menuItemNew = MenuItemLabel::create(labelNew, CC_CALLBACK_1(MainMenu::menuCallback, this));
        menuItemNew->setPositionY(0);
        menuItemNew->setTag(bLevelSelect);
        
        auto prop = Sprite::create(IMG_JAZZ);
        prop->setScale(Util::getScreenRatioHeight(prop));
        prop->setPosition(screenSize.width*0.70, screenSize.height*0.50);
        this->addChild(prop);
        
        auto menu = Menu::create(menuItemPlay,menuItemNew, NULL);
        menu->setPosition(screenSize.width*0.30, screenSize.height*0.30);
        this->addChild(menu);
        
        bRet = true;
    } while(0);
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SFX_BG_INTRO,true);
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
//    log("Reached menuCallback from %d",obj->getTag());
    
    switch (obj->getTag()) {
        case bPlay:
        {
            auto scene = (Scene*)GameScene::create();
            Director::getInstance()->replaceScene(scene);
            break;
        }
        case bLevelSelect:
        {
            auto scene = (Scene*)LevelSelector::create();
            Director::getInstance()->replaceScene(scene);
            break;
        }
        default:
            break;
    }
    
}


void MainMenu::onExit() {
    Layer::onExit();
}
void MainMenu::update(float delta)
{
    
}