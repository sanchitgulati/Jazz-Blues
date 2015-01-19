//
//  LogoSplash.cpp
//  roundels
//
//  Created by Sanchit Gulati on 02/08/14.
//
//

#include "LogoSplash.h"
#include "MainMenu.h"
#include "Util.h"
#include "Resources.h"

using namespace cocos2d;

LogoSplash::LogoSplash() {
    
}

LogoSplash::~LogoSplash() {
    
}

bool LogoSplash::init() {
    bool bRet = false;
    
    do {
        
        CC_BREAK_IF(! LayerColor::init());
        
        auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
        auto touchListener = EventListenerTouchAllAtOnce::create();
        touchListener->onTouchesEnded = CC_CALLBACK_2(LogoSplash::onTouchesEnded, this);
        touchListener->onTouchesBegan = CC_CALLBACK_2(LogoSplash::onTouchesBegan, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        this->setColor(Color3B::WHITE);
        this->setOpacity(255);
        
        Size screenSize = Director::getInstance()->getWinSize();
        
        logoSprite = CCSprite::create("images/logo.png");
        float logoWidth = logoSprite->boundingBox().size.width;
        logoSprite->setScale(clampf(screenSize.width/logoWidth*0.8,0,1));
        
        this->addChild(logoSprite);
        logoSprite->setPosition(Point(screenSize.width/2,screenSize.height/2));
        logoSprite->setOpacity(0);
        
        bRet = true;
    } while(0);
    
    
    _fmod = FmodHelper::getInstance();
    
    _fmod->loadEvent("Music");
    _fmod->playEvent("Music");
    _fmod->changeParam("Music", "Menu",0);
    _fmod->changeParam("Music", "Track1",0);
    _fmod->changeParam("Music", "Track2",0);
    _fmod->changeParam("Music", "Track3",0);
    _fmod->changeParam("Music", "Track4",0);
    _fmod->changeParam("Music", "Track5",0);
    _fmod->changeParam("Music", "Menu",0);
    
    GlobalClass::soundtrack = 0;
    GlobalClass::lock = 0;
    
    _fmod->loadEvent("Typewriter");
    _fmod->loadEvent("SocietyLaugh");
    _fmod->loadEvent("Police");
    _fmod->loadEvent("Jump");
    _fmod->loadEvent("Thunder");
    _fmod->loadEvent("BlueScream");
    _fmod->loadEvent("CrowdAww");
    _fmod->loadEvent("CrowdScream");
    
    
    this->scheduleUpdate();
    
    int a = (Util::randf()*LOSE_QOUTES);
    int b = (Util::randf()*WIN_QOUTES);
    GlobalClass::qouteLose = a;
    GlobalClass::qouteWin = b;
    log("lose %d-%d & win %d-%d",a,GlobalClass::qouteLose,b,GlobalClass::qouteWin);
    
    return bRet;
}


void LogoSplash::onEnter() {
    
    Layer::onEnter();
    
    auto nextSceneFunc = CallFuncN::create(CC_CALLBACK_1(LogoSplash::nextScene,this));
    
    
    ActionInterval* pFadeIn = FadeIn::create(1.0f);
    ActionInterval* pFadeOut = FadeOut::create(1.0f);
    DelayTime* delayTimeShort = DelayTime::create(1.0f);
    
    FiniteTimeAction* pSeq = CCSequence::create(pFadeIn, delayTimeShort, pFadeOut,nextSceneFunc, NULL );
    
    logoSprite->runAction(pSeq);
}

void LogoSplash::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    this->nextScene(this);
}


void LogoSplash::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{}



void LogoSplash::onExit() {
    Layer::onExit();
}

void LogoSplash::nextScene(Node *)
{
    Scene* scene = Scene::create();
    auto layer = MainMenu::create();
    scene->addChild(layer);
    Director::getInstance()->replaceScene(scene);
}

void LogoSplash::update(float delta)
{
    _fmod->update();
    
}