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

enum {bPlay,bLevelSelect,bMenu,bSound,bQuit};

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
        
        cocos2d::Size screenSizeSize = Director::getInstance()->getWinSize();
        
        auto bg = Sprite::create("images/paper.png",cocos2d::Rect(0,0,screenSizeSize.width,screenSizeSize.height));
        bg->setPosition(screenSizeSize.width/2, screenSizeSize.height/2);
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
        gameLogo0->setPosition(cocos2d::Point(screenSizeSize.width/2,screenSizeSize.height*0.80));
        gameLogo0->setScale(Util::getScreenRatioHeight(gameLogo0)*0.4);
        _gameLogo->addChild(gameLogo0);
        
        auto gameLogo1 = Sprite::create(IMG_GAME_LOGO_1);
        gameLogo1->setPosition(cocos2d::Point(screenSizeSize.width/2,screenSizeSize.height*0.80));
        gameLogo1->setScale(Util::getScreenRatioHeight(gameLogo1)*0.4);
        _gameLogo->addChild(gameLogo1);
        
        sequence = Sequence::create(move_reverse,move,NULL);
        repeat = RepeatForever::create(sequence);
        gameLogo1->runAction(repeat);
        
        auto strplay = "continue";
        
        auto t = UserDefault::getInstance()->getIntegerForKey("continue",1);
        if(t == 1)
        {
            strplay = "new game";
        }
        auto labelPlay = Label::createWithTTF(strplay, FONT, 54);
        labelPlay->setColor(RGB_BLACK);
        auto menuItemPlay = MenuItemLabel::create(labelPlay, CC_CALLBACK_1(MainMenu::menuCallback, this));
        menuItemPlay->setPositionY(80);
        menuItemPlay->setTag(bPlay);
        
        
        auto labelNew = Label::createWithTTF("level selection", FONT, 36);
        labelNew->setColor(RGB_BLACK);
        auto menuItemNew = MenuItemLabel::create(labelNew, CC_CALLBACK_1(MainMenu::menuCallback, this));
        menuItemNew->setPositionY(0);
        menuItemNew->setTag(bLevelSelect);
        
        auto labelMusic = Label::createWithTTF("soundtracks", FONT, 36);
        labelMusic->setColor(RGB_BLACK);
        auto menuItemMusic = MenuItemLabel::create(labelMusic, CC_CALLBACK_1(MainMenu::menuCallback, this));
        menuItemMusic->setPositionY(-60);
        menuItemMusic->setTag(bSound);
        
        auto labelQuit = Label::createWithTTF("quit", FONT, 24);
        labelQuit->setColor(RGB_BLACK);
        auto menuItemQuit = MenuItemLabel::create(labelQuit, CC_CALLBACK_1(MainMenu::menuCallback, this));
        menuItemQuit->setPositionY(-120);
        menuItemQuit->setTag(bQuit);
        
        _prop = Node::create();
        this->addChild(_prop);
        
        auto prop0 = Sprite::create(IMG_JAZZ_0);
        prop0->setScale(Util::getScreenRatioHeight(prop0));
        prop0->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        prop0->setPosition(screenSizeSize.width, screenSizeSize.height*0.50);
        _prop->addChild(prop0);
        delta = 5;
        move = MoveBy::create(12, Vec2(delta, 0));
        move_reverse = MoveBy::create(12, Vec2(delta, 0));
        sequence = Sequence::create(move,move_reverse,NULL);
        repeat = RepeatForever::create(sequence);
        //prop0->runAction(repeat);
        
        auto prop1 = Sprite::create(IMG_JAZZ_1);
        prop1->setScale(Util::getScreenRatioHeight(prop1));
        prop1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        prop1->setPosition(screenSizeSize.width, screenSizeSize.height*0.50);
        _prop->addChild(prop1);
        
        delta = 5;
        move = MoveBy::create(15, Vec2(delta, 0));
        sequence = Sequence::create(move,move->reverse(),NULL);
        repeat = RepeatForever::create(sequence);
        //prop1->runAction(repeat);
        
        auto prop2 = Sprite::create(IMG_JAZZ_2);
        prop2->setScale(Util::getScreenRatioHeight(prop2));
        prop2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        prop2->setPosition(screenSizeSize.width, screenSizeSize.height*0.50);
        _prop->addChild(prop2);
        auto rotate = RotateBy::create(20,3);
        prop2->runAction(RepeatForever::create(Sequence::create(rotate,rotate->reverse(), NULL)));
        
        _menu = Menu::create(menuItemPlay,menuItemMusic,menuItemQuit,menuItemNew, NULL);
        _menu->setPosition(screenSizeSize.width*0.30, screenSizeSize.height*0.35);
        this->addChild(_menu);
        
        
        _table = Node::create();
        _table->setPosition(Vec2(0,0));
        _table->setOpacity(0);
        _table->setVisible(false);
        this->addChild(_table);
        
        _player = Node::create();
        _player->setPosition(Vec2(0,0));
        _player->setOpacity(0);
        _player->setVisible(false);
        this->addChild(_player);
        
        
        
        auto margin = 5;
        
        auto bracketLeft = Sprite::create(FNT_BRACKET);
        bracketLeft->setScale(Util::getScreenRatioHeight(bracketLeft)*0.40);
        bracketLeft->setPosition(0.0 + margin, screenSizeSize.height - margin);
        bracketLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        bracketLeft->setOpacity(0);
        _table->addChild(bracketLeft);
        
        auto bracketRight = Sprite::create(FNT_BRACKET);
        bracketRight->setFlippedX(true);
        bracketRight->setFlippedY(true);
        bracketRight->setScale(Util::getScreenRatioHeight(bracketRight)*0.40);
        bracketRight->setPosition(screenSizeSize.width - margin, 0 + margin);
        bracketRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        bracketRight->setOpacity(0);
        _table->addChild(bracketRight);
        
        
        _fmod = FmodHelper::getInstance();
        _fmod->changeParam("Music","FinalLevel",0);
        _fmod->changeParam("Music","Menu",0);
        
        bRet = true;
    } while(0);
    
    
    this->scheduleUpdate();
    
    return bRet;
}

void MainMenu::createLevelMenu()
{
    _fmod->changeParam("Music","Menu",0);
    _fmod->changeParam("Music","LevelTransition",1);
    _table->setVisible(true);
    cocos2d::Size screenSizeSize = Director::getInstance()->getWinSize();
    cocos2d::Vector<MenuItem *> lvlList;
    for (int i = 0; i < LVLS; i++) {
        auto lbl = Label::createWithTTF(level[i].c_str(), FONT, 21);
        auto sptr = Sprite::create(IMG_RECORD);
        auto item = MenuItemSprite::create(sptr,sptr, CC_CALLBACK_1(MainMenu::levelCallback, this));
        
        item->setContentSize(cocos2d::Size(screenSizeSize.height*0.24,screenSizeSize.height*0.20));
        sptr->setScale(Util::getScreenRatioHeight(sptr)*0.15);
        item->setTag(i+1);
        lvlList.pushBack(item);
        lbl->setColor(RGB_ROSE);
        lbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        lbl->setPosition(Vec2(item->getBoundingBox().size.width/2,0));
        item->addChild(lbl);
        sptr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sptr->setPosition(item->getBoundingBox().size.width/2, item->getBoundingBox().size.height/2);
        
        item->setScale(0.8);
        item->setVisible(false);
        auto popOut = EaseBounceOut::create(ScaleTo::create(1, 1));
        auto callFunc = CallFunc::create([item](){item->setVisible(true);});
        item->runAction(Sequence::create(DelayTime::create(1),callFunc, popOut,NULL));
        
        auto v = UserDefault::getInstance()->getIntegerForKey("max",1);
        if((i+1) > v || (i+1) >= 11) // 9 is an hack
        {
            sptr->setOpacity(50);
            item->setEnabled(false);
            
        }
        else{auto time = 3 + Util::randf()*3;
            sptr->runAction(RepeatForever::create(RotateBy::create(time, 360)));
        }
    }
    auto menu = Menu::createWithArray(lvlList);
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    menu->setPosition(screenSizeSize.width*0.50, screenSizeSize.height*0.50);
    menu->setTag(3);
    this->addChild(menu);
    menu->alignItemsInColumns(3,3,3,3, NULL);
}


void MainMenu::onEnter() {
    
    Layer::onEnter();
    
    
}

void MainMenu::createSoundPlayer()
{
    _fmod->changeParam("Music", "Menu",1);
    
    _player->setVisible(true);
    cocos2d::Size screenSizeSize = Director::getInstance()->getWinSize();
    cocos2d::Vector<MenuItem *> soundList;

    
    auto trackName = Label::createWithTTF("NOW PLAYING", FONT, 42);
    trackName->setPosition(screenSizeSize.width*0.30, screenSizeSize.height - 50);
    trackName->setColor(RGB_BLACK);
    
    trackName->setOpacity(0);
    trackName->setVisible(false);
    auto fadeIn = FadeIn::create(1);
    auto callFunc = CallFunc::create([trackName](){trackName->setVisible(true);});
    trackName->runAction(Sequence::create(DelayTime::create(1),callFunc, fadeIn,NULL));
    
    
    _player->addChild(trackName);
    
    
    auto back = Label::createWithTTF("back to menu", FONT, 24);
    back->setColor(RGB_ROSE);
    back->setOpacity(0);
    back->setVisible(false);
    fadeIn = FadeIn::create(1);
    callFunc = CallFunc::create([back](){back->setVisible(true);});
    back->runAction(Sequence::create(DelayTime::create(1),callFunc, fadeIn,NULL));
    
    auto backItem = MenuItemLabel::create(back,CC_CALLBACK_1(MainMenu::menuCallback, this));
    
    auto menuback = Menu::create(backItem, NULL);
    backItem->setTag(bMenu);
    menuback->setPosition(screenSizeSize.width*0.30 - 45, screenSizeSize.height - 50 - trackName->getContentSize().height*1.25);
    _player->addChild(menuback);
    
    for (int i = 0; i < TRACKS; i++) {
        auto lbl = Label::createWithTTF(StringUtils::format("%d.  %s\nperformed by : %s",i+1,tracks[i].c_str(),artists[i].c_str()).c_str(), FONT, 21);
        
        lbl->setTag(i+1);
        if(i+1 == GlobalClass::soundtrack)
            lbl->setColor(RGB_ROSE);
        else
            lbl->setColor(RGB_BLACK);
        
        _tracksTTF.pushBack(lbl);
        lbl->setHorizontalAlignment(TextHAlignment::LEFT);
        lbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        auto item = MenuItemLabel::create(lbl, CC_CALLBACK_1(MainMenu::soundCallback, this));
        
        item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        item->setTag(i+1);
        soundList.pushBack(item);
        
        item->setOpacity(0);
        item->setVisible(false);
        auto fadeIn = FadeIn::create(1);
        auto callFunc = CallFunc::create([item](){item->setVisible(true);});
        item->runAction(Sequence::create(DelayTime::create(1),callFunc, fadeIn,NULL));
        
    }
    
    auto credits = "A game by Sanchit Gulati\nMusic Curation & Sound Design by 3 Quavers\nsfx from freesfx.co.uk\nSpecial thanks to Manish Basetia\nThanks Fubar Ghetto, Drift The Trio & Refuge for their music\nAudio Engine : FMOD Studio by Firelight Technologies";
    
    auto creditsTTF = Label::createWithTTF(credits, FONT, 16);
    creditsTTF->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    creditsTTF->setPosition(Vec2(10,10));
    creditsTTF->setColor(RGB_BLACK);
    
    creditsTTF->setOpacity(0);
    creditsTTF->setVisible(false);
    fadeIn = FadeIn::create(1);
    callFunc = CallFunc::create([creditsTTF](){creditsTTF->setVisible(true);});
    creditsTTF->runAction(Sequence::create(DelayTime::create(1),callFunc, fadeIn,NULL));
    _player->addChild(creditsTTF);
    
    
    auto menu = Menu::createWithArray(soundList);
    menu->alignItemsVerticallyWithPadding(10);
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    menu->setPosition(screenSizeSize.width*0.20, screenSizeSize.height*0.50);
    menu->setTag(4);
    _player->addChild(menu);
}


void MainMenu::levelCallback(cocos2d::Ref *pSender)
{
    auto obj = (Node*)pSender;
    UserDefault::getInstance()->setIntegerForKey("continue",obj->getTag());
    UserDefault::getInstance()->flush();
    //    auto scene = (Scene*)GameScene::create();
    //    auto t = TransitionFade::create(1.0f, scene, Color3B::WHITE);
    //    Director::getInstance()->replaceScene(t);
    transitionToGameScene();
}

void MainMenu::soundCallback(cocos2d::Ref *pSender)
{

    
    auto obj = (Node*)pSender;
    
    for(auto t : _tracksTTF)
    {
        if(t->getTag() == obj->getTag())
            t->setColor(RGB_ROSE);
        else
            t->setColor(RGB_BLACK);
    }
    
    _fmod->changeParam("Music","Menu",1);
    _fmod->changeParam("Music","Track1",0);
    _fmod->changeParam("Music","Track2",0);
    _fmod->changeParam("Music","Track3",0);
    _fmod->changeParam("Music","Track4",0);
    _fmod->changeParam("Music","Track5",0);
    _fmod->changeParam("Music","Track6",0);
    _fmod->changeParam("Music","LevelTransition",0);
    
    if(obj->getTag() == 1)
        _fmod->changeParam("Music","Menu",1);
    
    auto s = obj->getTag();
    auto t = StringUtils::format("Track%d",s);
    _fmod->changeParam("Music",t,1);
    
    
    GlobalClass::soundtrack = s;
}

// a selector callback
void MainMenu::menuCallback(cocos2d::Ref* pSender)
{
    
    _menu->setEnabled(false);
    auto obj = (Node*)pSender;
    
    switch (obj->getTag()) {
        case bPlay:
        {
            _prop->runAction(MoveBy::create(1, Vec2(100, 0)));
            for(auto c : _prop->getChildren())
            {
                c->runAction(FadeOut::create(1));
            }
            for(auto c : _gameLogo->getChildren())
            {
                c->runAction(FadeOut::create(1));
            }
            transitionToGameScene();
            break;
        }
        case bSound:
        {
            hideButtons();
            createSoundPlayer();
            break;
        }
        case bLevelSelect:
        {
            createLevelMenu();
            hideButtons(true);
            break;
        }
        case bMenu:
        {
            showButtons();
            break;
        }
        case bQuit:
        {
            Director::getInstance()->end();
            exit(EXIT_SUCCESS);
            break;
        }
        default:
            break;
    }
    
}

void MainMenu::showButtons()
{
    for(auto c : _player->getChildren())
    {
        c->runAction(FadeOut::create(1));
    }
    _menu->setEnabled(true);
    
    
    auto callFunc0 = CallFunc::create([this](){
        _player->setVisible(false);
    });
    _fmod->changeParam("Music", "Menu",0);
    
    _player->runAction(Sequence::create(FadeOut::create(1),callFunc0, NULL));
    for(auto c : _gameLogo->getChildren())
    {
        c->runAction(Sequence::create(DelayTime::create(1),FadeIn::create(1), NULL) );
    }
    auto callFunc = CallFunc::create([this](){
        _menu->setVisible(true);
    });
    _menu->runAction(Sequence::create(DelayTime::create(1),FadeIn::create(1),callFunc, NULL));
}

void MainMenu::hideButtons(bool guitar)
{
    if(guitar){
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
        
        _prop->runAction(MoveBy::create(1, Vec2(100, 0)));
        
        
        for(auto c : _prop->getChildren())
        {
            c->runAction(FadeOut::create(1));
        }
    }
    
    
    for(auto c : _gameLogo->getChildren())
    {
        c->runAction(FadeOut::create(1));
    }
    auto callFunc = CallFunc::create([this](){
        _menu->setVisible(false);
    });
    _menu->runAction(Sequence::create(FadeOut::create(1),callFunc, NULL));
}

void MainMenu::onExit() {
    Layer::onExit();
}
void MainMenu::update(float delta)
{
    _fmod->update();
}

void MainMenu::transitionToGameScene()
{
    auto Size = Director::getInstance()->getWinSize();
    auto clipper = ClippingNode::create();
    
    clipper->setAnchorPoint(cocos2d::Point(0.5f, 0.5f));
    clipper->setPosition(Size.width / 2, Size.height / 2);
    clipper->setAlphaThreshold(0.05f);
    clipper->setInverted(true);
    
    Sprite* blackRect = Sprite::create("images/black_screen.png");
    blackRect->setScale(Size.width/blackRect->getContentSize().width, Size.height/blackRect->getContentSize().height);
    clipper->addChild(blackRect);
    
    auto heart = Sprite::create("images/heart.png");
    heart->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    heart->setScale((Size.width/heart->getContentSize().width)*1.3);
    clipper->setStencil(heart);	//set the cut triangle in the ClippingNode.
    
    this->addChild(clipper, 500);
    auto callFunc = CallFuncN::create(CC_CALLBACK_0(MainMenu::toGameScene,this));
    heart->runAction(EaseSineOut::create(Spawn::create(ScaleTo::create(2.5f, 0.0f, 0.0f),
                                                       RotateBy::create(2.5f, 540),
                                                       Sequence::create(DelayTime::create(2.5),
                                                                        callFunc, NULL), NULL)));
    
    _fmod->changeParam("Music", "Menu",1); //tranisition
}

void MainMenu::toGameScene()
{
    //get the game scene and run it.
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
}

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}