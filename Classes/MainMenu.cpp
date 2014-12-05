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
        
        
        auto delta = 20;
        auto move = MoveBy::create(8, Vec2(-delta, 0));
        auto move_reverse = MoveBy::create(8, Vec2(delta, 0));
        auto sequence = Sequence::create(move,move_reverse,NULL);
        auto repeat = RepeatForever::create(sequence);
        
        
        _gameLogo = Node::create();
        this->addChild(_gameLogo);
        auto gameLogo0 = Sprite::create(IMG_GAME_LOGO_0);
        gameLogo0->setPosition(Point(screenSize.width/2,screenSize.height*0.80));
        gameLogo0->setScale(Util::getScreenRatioHeight(gameLogo0)*0.4);
        _gameLogo->addChild(gameLogo0);
        
        auto gameLogo1 = Sprite::create(IMG_GAME_LOGO_1);
        gameLogo1->setPosition(Point(screenSize.width/2,screenSize.height*0.80));
        gameLogo1->setScale(Util::getScreenRatioHeight(gameLogo1)*0.4);
        _gameLogo->addChild(gameLogo1);
        
        sequence = Sequence::create(move_reverse,move,NULL);
        repeat = RepeatForever::create(sequence);
        gameLogo1->runAction(repeat);
        
        
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
        
        _prop = Node::create();
        this->addChild(_prop);
        
        auto prop0 = Sprite::create(IMG_JAZZ_0);
        prop0->setScale(Util::getScreenRatioHeight(prop0));
        prop0->setPosition(screenSize.width*0.70, screenSize.height*0.50);
        _prop->addChild(prop0);
        delta = 5;
        move = MoveBy::create(12, Vec2(-delta, 0));
        move_reverse = MoveBy::create(12, Vec2(delta, 0));
        sequence = Sequence::create(move,move_reverse,NULL);
        repeat = RepeatForever::create(sequence);
        prop0->runAction(repeat);
        
        auto prop1 = Sprite::create(IMG_JAZZ_1);
        prop1->setScale(Util::getScreenRatioHeight(prop1));
        prop1->setPosition(screenSize.width*0.70, screenSize.height*0.50);
        _prop->addChild(prop1);
        
        delta = 5;
        move = MoveBy::create(15, Vec2(-delta, 0));
        sequence = Sequence::create(move,move->reverse(),NULL);
        repeat = RepeatForever::create(sequence);
        prop1->runAction(repeat);
        
        auto prop2 = Sprite::create(IMG_JAZZ_2);
        prop2->setScale(Util::getScreenRatioHeight(prop2));
        prop2->setPosition(screenSize.width*0.70, screenSize.height*0.50);
        _prop->addChild(prop2);
        auto rotate = RotateBy::create(10,3);
        prop2->runAction(RepeatForever::create(Sequence::create(rotate,rotate->reverse(), NULL)));
        
        _menu = Menu::create(menuItemPlay,menuItemNew, NULL);
        _menu->setPosition(screenSize.width*0.30, screenSize.height*0.30);
        this->addChild(_menu);
        
        
        _table = Node::create();
        _table->setPosition(Vec2(0,0));
        _table->setOpacity(0);
        _table->setVisible(false);
        this->addChild(_table);
        
        

        
        auto margin = 5;
        
        auto bracketLeft = Sprite::create(FNT_BRACKET);
        bracketLeft->setScale(Util::getScreenRatioHeight(bracketLeft)*0.40);
        bracketLeft->setPosition(0.0 + margin, screenSize.height - margin);
        bracketLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        bracketLeft->setOpacity(0);
        _table->addChild(bracketLeft);
        
        auto bracketRight = Sprite::create(FNT_BRACKET);
        bracketRight->setFlippedX(true);
        bracketRight->setFlippedY(true);
        bracketRight->setScale(Util::getScreenRatioHeight(bracketRight)*0.40);
        bracketRight->setPosition(screenSize.width - margin, 0 + margin);
        bracketRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        bracketRight->setOpacity(0);
        _table->addChild(bracketRight);
        
        

        
        
        bRet = true;
    } while(0);
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SFX_BG_INTRO,true);
    this->scheduleUpdate();
    
    return bRet;
}

void MainMenu::createLevelMenu()
{
    Size screenSize = Director::getInstance()->getWinSize();
    cocos2d::Vector<MenuItem *> lvlList;
    for (int i = 0; i < LVLS; i++) {
        auto lbl = Label::createWithTTF(level[i].c_str(), FONT_JANE, 21);
        auto sptr = Sprite::create(IMG_RECORD);
        auto item = MenuItemSprite::create(sptr,sptr, CC_CALLBACK_1(MainMenu::levelCallback, this));
        item->setContentSize(Size(screenSize.height*0.24,screenSize.height*0.20));
        sptr->setScale(Util::getScreenRatioHeight(sptr)*0.15);
        item->setTag(i+1);
        lvlList.pushBack(item);
        lbl->setColor(RGB_ROSE); //229,31,46
        lbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbl->setPosition(Vec2(item->getBoundingBox().size.width/2,0));
        item->addChild(lbl);
        sptr->setOpacity(0);
        lbl->setOpacity(0);
        sptr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sptr->setPosition(item->getBoundingBox().size.width/2, item->getBoundingBox().size.height/2);
        auto time = 3 + random()%3;
        sptr->runAction(RepeatForever::create(RotateBy::create(time, 360)));
        sptr->runAction(Sequence::create(DelayTime::create(1),FadeTo::create(1,255), NULL));
        lbl->runAction(Sequence::create(DelayTime::create(1),FadeTo::create(1,255), NULL));
    }
    auto menu = Menu::createWithArray(lvlList);
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    menu->setPosition(screenSize.width*0.50, screenSize.height*0.50);
    menu->setTag(3);
    _table->addChild(menu);
    menu->alignItemsInColumns(3,3,3,3, NULL);
}


void MainMenu::onEnter() {
    
    Layer::onEnter();
    

}


void MainMenu::levelCallback(cocos2d::Ref *pSender)
{
    auto obj = (Node*)pSender;
    UserDefault::getInstance()->setIntegerForKey("continue",obj->getTag());
    UserDefault::getInstance()->flush();
    auto scene = (Scene*)GameScene::create();
    auto t = TransitionFade::create(1.0f, scene, Color3B::WHITE);
    Director::getInstance()->replaceScene(t);
}

// a selector callback
void MainMenu::menuCallback(cocos2d::Ref* pSender)
{
    auto obj = (Node*)pSender;
    
    switch (obj->getTag()) {
        case bPlay:
        {
            auto scene = (Scene*)GameScene::create();
            auto t = TransitionFade::create(1.0f, scene, Color3B::WHITE);
            Director::getInstance()->replaceScene(t);
            break;
        }
        case bLevelSelect:
        {
            createLevelMenu();
            _table->setVisible(true);
            for(auto c : _table->getChildren())
            {
                if(c->getTag() == 1)
                    c->runAction(Sequence::create(FadeIn::create(1),FadeOut::create(1), NULL));
                else if(c->getTag() == 2)
                    c->runAction(Sequence::create(DelayTime::create(2),FadeIn::create(1),FadeOut::create(1), NULL));
                else if(c->getTag() == 3)
                {
                    //was buggy, using new approach now.
                }
                else
                {
                    c->runAction(Sequence::create(DelayTime::create(1),FadeTo::create(1,200), NULL));
                }
                    
            }
            
            auto callFunc = CallFunc::create([this](){
                _menu->setVisible(false);
            });
            _menu->runAction(Sequence::create(FadeOut::create(1),callFunc, NULL));
            
            _prop->runAction(MoveBy::create(1, Vec2(100, 0)));
            for(auto c : _prop->getChildren())
            {
                c->runAction(FadeOut::create(1));
            }
            for(auto c : _gameLogo->getChildren())
            {
                c->runAction(FadeOut::create(1));
            }

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