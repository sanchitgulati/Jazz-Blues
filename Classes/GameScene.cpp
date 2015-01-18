//
//  GameScene.cpp
//  jazz
//
//  Created by Sanchit Gulati on 11/08/14.
//
//

#include "GameScene.h"
#include "MainMenu.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    _night = nullptr;
    _playTimes = 0;
    _arrested = false;
    _god = false;
    _blood = 0;
    _mute = 0;
    _giftTurn = pFemale;
    _gameState = gsIntro;
    kCurrentLevel = UserDefault::getInstance()->getIntegerForKey("continue",1);

    auto max = UserDefault::getInstance()->getIntegerForKey("max",1);
    if(kCurrentLevel > max)
        UserDefault::getInstance()->setIntegerForKey("max",kCurrentLevel);
    
    UserDefault::getInstance()->flush();
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _screenOutPosition = Vec2(_visibleSize.width/2,-1*_visibleSize.height/2);
    _screenInPosition = Vec2(_visibleSize.width/2,_visibleSize.height/2);
    
    _eventDispatcher = Director::getInstance()->getEventDispatcher();
    _eventDispatcher->removeAllEventListeners();
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchlistener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
    
    _male = nullptr;
    _female = nullptr;
    
    _parent = Node::create();
    _parent->setAnchorPoint(Point(0.5,0.5));
    this->addChild(_parent,zGame);
    
    
    createPhysicalWorld();
    loadLevel(0);
    loadInstuctions();
    
    // schedule the update
    this->schedule(schedule_selector(GameScene::update), kUpdateInterval);
    
    //Menu
    auto levelSelector = MenuItemImage::create(IMG_LEVEL_SELECTOR_0, IMG_LEVEL_SELECTOR_1,CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    levelSelector->setTag(menuLevel);
    auto margin = levelSelector->getContentSize().width*1.2;
    levelSelector->setPositionX(margin*-1);
    
    auto refresh = MenuItemImage::create(IMG_REFRESH_0,IMG_REFRESH_1,CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    refresh->setTag(menuRefresh);
    refresh->setPositionX(margin);
    
    auto unmute = MenuItemImage::create(IMG_UNMUTE_0,IMG_UNMUTE_1);
    unmute->setTag(menuUnmute);
    
    auto mute = MenuItemImage::create(IMG_MUTE_0,IMG_MUTE_1);
    mute->setTag(menuMute);
    
    auto sfxToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(GameScene::menuCloseCallback, this), mute,unmute, NULL);
    sfxToggle->setTag(menuToggle);
    sfxToggle->setPositionX(0);
    
    auto menu = Menu::create(levelSelector,refresh,sfxToggle, NULL);
    if(kCurrentLevel == 5 || kCurrentLevel == 10 || kCurrentLevel == 12) //tunnel
        menu->setPosition(Vec2(_visibleSize.width/2,margin));
    else
        menu->setPosition(Vec2(_visibleSize.width/2,_visibleSize.height-margin));
    menu->setTag(tagMenu);
    menu->setOpacity(0);
    this->addChild(menu,zHUD);
    
    //setting up on-screen button for mobile
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto left = Button::create(IMG_BUTTON_LEFT_0, IMG_BUTTON_LEFT_1,_male,_female);
    left->setUserData((void*)(new userdataFormat(bLeft)));
    left->setPosition(128, 128);
    this->addChild(left,zControl);
    
    auto right = Button::create(IMG_BUTTON_RIGHT_0, IMG_BUTTON_RIGHT_1,_male,_female);
    right->setUserData((void*)(new userdataFormat(bRight)));
    right->setPosition(320,128);
    this->addChild(right,zControl);
    
    auto up = Button::create(IMG_BUTTON_UP_0, IMG_BUTTON_UP_1,_male,_female);
    up->setUserData((void*)(new userdataFormat(bUp)));
    up->setPosition(_visibleSize.width - 128,128);
    this->addChild(up,zControl);
#endif
    
    /* Entering box2d world */
//            _platformsGroup->setVisible(false);
//            _playerGroup->setVisible(false);
//            _bgGroup->setVisible(false);
    /*end*/
    
    
    auto valuekey = _tm->getProperties();
    auto invert = valuekey["invert"].asString().c_str();
    if(strcmp(invert,"true") == 0)
    {
        _male->invert();
        _female->invert();
    }
    log("value %s",invert);
    
    auto night = valuekey["time"].asString().c_str();
    _night = nullptr;
    if(strcmp(night,"night") == 0)
    {
        _night = Sprite::create(IMG_BG);
        _night->setScale(_visibleSize.width/_night->getBoundingBox().size.width, _visibleSize.height/_night->getBoundingBox().size.height);
        _night->setColor(Color3B::BLACK);
        _night->setOpacity(100);
        _night->setPosition(Vec2(_visibleSize.width/2, _visibleSize.height/2));
        this->addChild(_night,zNight);
    }
    log("value %s",night);
    
//    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
//    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SFX_BG_HAPPY,true);
    //
    
    if(kCurrentLevel == 12)
    {
        auto callFunc = CallFunc::create([this](){
            this->loadInstuctionsEnd();
        });
        runAction(Sequence::create(DelayTime::create(25),callFunc, NULL));
    }
    
    _fmod = FmodHelper::getInstance();
    this->toGameScene();
    return true;
}


void GameScene::skip()
{
    auto obj = this->getChildByTag(tagSkip);
    obj->setVisible(false);
    auto c = getChildByTag(tagIntro);
    for(auto child : c->getChildren())
        child->stopAllActions();
    c->stopAllActions();
    c->setVisible(false);
    this->animateMapIn();
    this->_gameState = gsStart;
//    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
}

void GameScene::menuCloseCallback(Ref* pSender)
{
    auto obj = (Node*)pSender;
    switch (obj->getTag()) {
        case menuLevel:
        {
            auto scene = (Scene*)MainMenu::create();
            Director::getInstance()->replaceScene(scene);
            break;
        }
        case menuRefresh:
        {
            transitionToGameScene();
            break;
        }
        case menuSkip:
        {
            skip();
            break;
        }
        case menuToggle:
        {
            if(_mute == 1){
                _mute = 0;
                _fmod->unmute();
            }
            else{
                _mute = 1;
                _fmod->mute();
            }
            break;
        }
        default:
            break;
    }
}

void GameScene::update(float dt)
{
    /* Taking inputs from On-Screen Button if any */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
#endif
    /* End */
    _world->Step(dt, 8, 1);
    _fmod->update();
    if(_gameState == gsStart)
    {
        //        if(_society != nullptr)
        //            _society->move();
        if(_female != nullptr)
        {
            //Dynamic Scaling
            auto diffX = fabs(_male->getSprite()->getPosition().x - _female->getSprite()->getPosition().x);
            auto diffY = fabs( _male->getSprite()->getPosition().y - _female->getSprite()->getPosition().y);
            auto maxScale = MAX(diffX, diffY);
            _parent->setScale(_visibleSize.height/(_parent->getContentSize().height + (maxScale*0.5)));
            
            if( (_female->getB2Body()->GetPosition().y <= -100/32) || (_male->getB2Body()->GetPosition().y <= -100/32))
            {
                loadDiedEnd();
            }
            
            
            
            //End
            if(_male->getAtFinish() && _female->getAtFinish() && !_arrested)
            {
                if(kCurrentLevel == 11)
                {
                    auto cf = CallFunc::create([this](){
                        this->loadInstuctionsEnd();});
                    
                    _night->stopAllActions();
                    _night->setOpacity(100);
                    auto dt = DelayTime::create(6);
                    auto finish = (Win*)_platformsGroup->getChildByTag(999);
                    finish->fade();
                    
                    _female->poison();
                    _arrested = true; //hack
                    
                    runAction(Sequence::create(dt,cf, NULL));
                }
                else{
                    _male->setLocalZOrder(0);
                    _female->setLocalZOrder(0);
                    for(auto c : _platformsGroup->getChildren())
                    {
                        if(c->getTag() == 999)
                        {
                            auto s = MoveBy::create(0.1, Vec2(5, 0));
                            c->runAction(Repeat::create(Sequence::create(s,s->reverse(),s->reverse(), NULL),2));
                        }
                    }
                    loadInstuctionsEnd();
                }
            }
            
            if(_playTimes == 3)
            {
                loadInstuctionsEnd();
                _playTimes = 1;
            }
            
            if(!_male->getIsAlive())
            {
                auto emitter = ParticleSystemQuad::create("particle_texture.plist");
                _platformsGroup->addChild(emitter, 999);
                emitter->setPosition(_male->getSprite()->getPosition());
                _world->DestroyBody(_male->getB2Body());
                _male->removeFromParentAndCleanup(true);
                _gameState = gsDied;
                loadDiedEnd();
            }
            if(!_female->getIsAlive())
            {
                auto emitter = ParticleSystemQuad::create("particle_texture.plist");
                _platformsGroup->addChild(emitter, 999);
                emitter->setPosition(_female->getSprite()->getPosition());
                _world->DestroyBody(_female->getB2Body());
                _female->removeFromParentAndCleanup(true);
                _gameState = gsDied;
                loadDiedEnd();
                
            }
            
            if(_blood == 1)
            {
                auto emitter = ParticleSystemQuad::create("particle_texture.plist");
                _platformsGroup->addChild(emitter, 999);
                emitter->setPosition(_society->getPos());
                
                
//                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_GRIND);
                _society->runAction(FadeOut::create(1));
                loadInstuctionsEnd();
                _blood++;
                
            }
        }
    }
}


void GameScene::loadData()
{
    //todo:
}

void GameScene::loadInstuctions()
{
    auto screenSize = Director::getInstance()->getVisibleSize();
    
    auto valuekey = _tm->getProperties();
    auto labelTitle = Label::createWithBMFont(BMP_FONT, valuekey["start"].asString().c_str());
    labelTitle->setWidth(screenSize.width/2);
    labelTitle->setColor(RGB_BLACK);
    auto len = valuekey["start"].asString().length() - 1;
    labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setPosition(screenSize.width/2,screenSize.height/2);
    
    
    
    auto menuLbl = Label::createWithTTF("- skip story -", FONT, 36);
    menuLbl->setColor(RGB_ROSE);
    auto menuItem = MenuItemLabel::create(menuLbl, CC_CALLBACK_1(GameScene::menuCloseCallback,this));
    menuItem->setTag(menuSkip);
    
    auto menuLblPC = Label::createWithTTF("press space to skip", FONT, 18);
    menuLblPC->setColor(RGB_ROSE);
    auto menuItemPC = MenuItemLabel::create(menuLblPC, CC_CALLBACK_1(GameScene::menuCloseCallback,this));
    menuItemPC->setTag(menuSkip);
    menuItemPC->setPositionY(-1*menuLbl->getContentSize().height*1);
    menuItemPC->setEnabled(false);
    
    auto menu = Menu::create(menuItem,menuItemPC,NULL);
    menu->setPosition(Vec2(screenSize.width/2,screenSize.height*0.30));
    menu->setTag(tagSkip);
    labelTitle->setTag(tagIntro);
    this->addChild(labelTitle);
    this->addChild(menu);
    
    auto fadeOutMessage = CallFunc::create([labelTitle](){
        labelTitle->runAction(FadeOut::create(0.2));
    });
    
    for(int i = 1; i <= len;i++)
    {
        auto c = labelTitle->getLetter(i);
        if(c == nullptr || c == NULL)
            continue;
        c->setOpacity(0);
        c->setColor(RGB_BLACK);
        auto dt = DelayTime::create(i*0.25);
        auto fi = FadeIn::create(0.16);
        
        CallFunc* cf;
        
        if(i == len)
        {
            cf = CallFunc::create([this,menu]()
                                  {
//                                      CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_TYPE_END);
                                      this->animateMapIn();
                                      menu->setVisible(false);
                                      this->_gameState = gsStart;
                                  });
            
            c->runAction(Sequence::create(dt,cf,fi,fadeOutMessage,NULL));
        }
        else
        {
            cf = CallFunc::create([this]()
                                  {
                                      this->_fmod->playEvent("Typewriter");
                                  });
            
            c->runAction(Sequence::create(dt,cf,fi, NULL));
        }
        
        
    }
    
}

void GameScene::loadInstuctionsEnd()
{
    _gameState = gsEnd;
    animateMapOut();
    unschedule(schedule_selector(GameScene::update));
    auto screenSize = Director::getInstance()->getVisibleSize();
    
    auto valuekey = _tm->getProperties();
    auto labelTitle = Label::createWithBMFont(BMP_FONT, valuekey["end"].asString().c_str());
    labelTitle->setWidth(screenSize.width/2);
    labelTitle->setColor(RGB_BLACK);
    
    labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setPosition(screenSize.width/2,screenSize.height/2);
    
    GlobalClass::qouteWin++;
    if(GlobalClass::qouteWin >= WIN_QOUTES)
        GlobalClass::qouteWin = 0;
    auto str = StringUtils::format("\"%s\"",win[GlobalClass::qouteWin].c_str());
    Label* winnerTitle = nullptr;
    if(kCurrentLevel == 12)
    {
        str = "This game is a dedication.\nTo Jazz,From Blues.";
        winnerTitle = Label::createWithTTF(str, FONT, 36);
        winnerTitle->setWidth(screenSize.width*0.90);
        winnerTitle->setColor(RGB_ROSE);
        
        winnerTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        winnerTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        winnerTitle->setPosition(screenSize.width/2,screenSize.height*0.70);
    }
    else
    {
        winnerTitle = Label::createWithTTF(str, FONT_JANE, 54);
        winnerTitle->setWidth(screenSize.width*0.90);
        winnerTitle->setColor(RGB_ROSE);
        
        winnerTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        winnerTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        winnerTitle->setPosition(screenSize.width/2,screenSize.height*0.70);
    }
    
    std::string nextText = "- next chapter -";
    if(kCurrentLevel == 12)
    {
        nextText = StringUtils::format("- home -");
    }
    
    auto menuLbl = Label::createWithTTF(nextText.c_str(),FONT,36);
    menuLbl->setColor(RGB_ROSE);
    auto menuItem = MenuItemLabel::create(menuLbl, CC_CALLBACK_1(GameScene::menuCloseCallback,this));
    menuItem->setTag(menuRefresh);
    auto menu = Menu::create(menuItem,NULL);
    menu->setPosition(Vec2(screenSize.width/2,screenSize.height*0.30));
    
    if(kCurrentLevel == 12)
    {
        menuItem->setTag(menuLevel);
    }
    
    this->addChild(winnerTitle);
    this->addChild(labelTitle);
    this->addChild(menu);
    winnerTitle->setOpacity(0);
    labelTitle->setOpacity(0);
    menuLbl->setOpacity(0);
    labelTitle->runAction(Sequence::create(DelayTime::create(1),FadeIn::create(1), NULL));
    winnerTitle->runAction(Sequence::create(DelayTime::create(2),FadeIn::create(1), NULL));
    menuLbl->runAction(Sequence::create(DelayTime::create(2),FadeIn::create(1), NULL));
    
    //update level
    
    kCurrentLevel ++; //increment level
    UserDefault::getInstance()->setIntegerForKey("continue", kCurrentLevel);
    UserDefault::getInstance()->flush();
}

void GameScene::loadDiedEnd()
{
    _gameState = gsEnd;
    animateMapOut();
    unschedule(schedule_selector(GameScene::update));
    auto screenSize = Director::getInstance()->getVisibleSize();
    
    GlobalClass::qouteLose++;
    if(GlobalClass::qouteLose >= LOSE_QOUTES)
        GlobalClass::qouteLose= 0;
    auto str = StringUtils::format("\"%s\"",lose[GlobalClass::qouteLose].c_str());
    auto labelTitle = Label::createWithTTF(str, FONT_JANE, 54);
    labelTitle->setWidth(screenSize.width*0.90);
    labelTitle->setColor(RGB_ROSE);
    
    labelTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    labelTitle->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    labelTitle->setPosition(screenSize.width/2,screenSize.height*0.70);
    
    auto menuLbl = Label::createWithTTF("- retry chapter-",FONT,36);
    menuLbl->setColor(RGB_ROSE);
    auto menuItem = MenuItemLabel::create(menuLbl, CC_CALLBACK_1(GameScene::menuCloseCallback,this));
    menuItem->setTag(menuRefresh);
    auto menu = Menu::create(menuItem,NULL);
    menu->setPosition(Vec2(screenSize.width/2,screenSize.height*0.30));
    
    
    labelTitle->setOpacity(0);
    menuLbl->setOpacity(0);
    this->addChild(labelTitle);
    this->addChild(menu);
    
    labelTitle->runAction(Sequence::create(DelayTime::create(1),FadeIn::create(1), NULL));
    menuLbl->runAction(Sequence::create(DelayTime::create(2),FadeIn::create(1), NULL));
}

void GameScene::createPhysicalWorld()
{
    _world = new b2World(b2Vec2(0.0f, kGravity)); //kGravity
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    auto _debugDraw = new GLESDebugDraw( kPixelsPerMeter );
    _world->SetDebugDraw(_debugDraw);
    _world->SetContactListener(this);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    //    flags += b2Draw::e_aabbBit;
    //    flags += b2Draw::e_pairBit;
    //    flags += b2Draw::e_centerOfMassBit;
    _debugDraw->SetFlags(flags);
    
    
}

void GameScene::loadLevel(int level)
{
    //TODO: check for game complete
    
    //load Animations
    
    //load bg
    _bgGroup = Node::create();
    this->addChild(_bgGroup,zBackground);
    
    auto screenSize = Director::getInstance()->getVisibleSize();
    log("screenSize %f %f",screenSize.width,screenSize.height);
    
    auto bg = Sprite::create("images/paper.png",cocos2d::Rect(0,0,screenSize.width,screenSize.height));
    bg->setPosition(screenSize.width/2, screenSize.height/2);
    bg->getTexture()->setTexParameters({GL_LINEAR, GL_LINEAR,GL_REPEAT,GL_REPEAT});
    _bgGroup->addChild(bg);
    
    auto loadLevelString = StringUtils::format("levels/%d.tmx",kCurrentLevel);
    _tm = TMXTiledMap::create(loadLevelString);
    _tm->setVisible(false);
    _platformsGroup = Node::create();
    _parent->addChild(_platformsGroup);
    
    _parent->setPosition(_screenOutPosition);
    _parent->setContentSize(Size(_tm->getMapSize().width * _tm->getTileSize().width,
                                 _tm->getMapSize().height * _tm->getTileSize().height));
    _platformsGroup->setContentSize(_parent->getContentSize());
    
    _tm->retain();
    
    
    std::string background_particle = "particle/autumn_texture.plist";
    auto valuekey = _tm->getProperties();
    auto val = valuekey["bg"].asString().c_str();
    if(strcmp(val,"") != 0)
        background_particle = StringUtils::format("particle/%s_texture.plist",val);
    
    auto particle_bg = ParticleSystemQuad::create(background_particle.c_str());
    particle_bg->setPosition(screenSize.width/2,screenSize.height);
    int z = zRain;
    Vec2 posVar = Vec2(screenSize.width/2, screenSize.height*0.75);
    if(strcmp(val, "autumn") == 0)
    {
        z = zBackground;
        posVar = Vec2(screenSize.width/2, screenSize.height*0.25);
        
    }
    particle_bg->setPosVar(posVar);
    this->addChild(particle_bg,z);
    
    _playerGroup = Node::create();
    _parent->addChild(_playerGroup);
    
    prepareLayers();
}


void GameScene::addObjects()
{
    // loop over the object groups in this tmx file
    auto objectGroups = _tm->getObjectGroups();
    for (auto& objectGroup : objectGroups)
    {
        auto objects = objectGroup->getObjects();
        for (auto& object : objects)
        {
            auto properties = object.asValueMap();
            auto type = properties.at("type");
            if (!type.isNull())
            {
                //                this->addObject(type.asString().c_str(), properties);
                //                this->objectCount++;
            }
        }
    }
}

void GameScene::prepareLayers()
{
    for (auto& object : _tm->getChildren())
    {
        // is this map child a tile layer?
        auto layer = dynamic_cast<TMXLayer*>(object);
        if (layer != nullptr)
        {
            this->createFixturesFirstPass(layer);
        }
    }
}

void GameScene::createFixturesFirstPass(TMXLayer* layer)
{
    // create all the rectangular fixtures for each tile
    Size layerSize = layer->getLayerSize();
    for (int y = 0; y < layerSize.height; y++)
    {
        for (int x = 0; x < layerSize.width; x++)
        {
            auto tileGID = layer->getTileGIDAt(Point(x,y));
            switch (tileGID) {
                case tmxEmpty:
                    break;
                case tmxPlatform: // platform
                {
                    auto platform = Platform::createFixture(_world,layer, x, y, 1.0f, 1.0f);
                    _platformsGroup->addChild(platform,0);
                    break;
                }
                case tmxFemale:
                    _female = Player::createPlayerFixture(_world,layer, x, y, 1.0f, 2.0f,pFemale);
                    _platformsGroup->addChild(_female,2);
                    break;
                case tmxMale:
                    _male = Player::createPlayerFixture(_world,layer, x, y, 1.0f, 2.0f,pMale);
                    _platformsGroup->addChild(_male,2);
                    break;
                case tmxFire:
                {
                    auto death = Death::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(death,1);
                    break;
                }
                case tmxWin:
                {
                    bool alt = false;
                    if(kCurrentLevel == 10)
                        alt = true;
                    auto win = Win::createFixture(_world, layer, x, y, 1.0, 2.0,alt); //bcoz image is 96*96
                    win->setTag(999);
                    _platformsGroup->addChild(win,1);
                    break;
                }
                case tmxWall:
                {
                    auto wall = Wall::createFixture(_world,layer, x, y, 1.0f, 1.0f);
                    _platformsGroup->addChild(wall,1);
                    break;
                    
                }
                case tmxBlock:
                {
                    auto block = Block::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(block,1);
                    break;
                }
                case tmxTemple:
                {
                    auto temple = Temple::createFixture(_world, layer, x, y, 3.0, 3.0);
                    _temple = temple;
                    _platformsGroup->addChild(temple,1);
                    break;
                }
                case tmxCloud:
                {
                    auto cloud = Cloud::createFixture(_world, layer, x, y, 2.0, 1.0);
                    _platformsGroup->addChild(cloud,1);
                    break;
                }
                case tmxDoor:
                {
                    auto door = Door::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _listOfDoors.pushBack(door);
                    _platformsGroup->addChild(door,1);
                    break;
                }
                case tmxPoison:
                {
                    auto poison = Poison::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(poison,1);
                    break;
                }
                case tmxTeddy:
                {
                    auto teddy = Teddy::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(teddy,1);
                    break;
                }
                case tmxTunnel:
                {
                    auto tunnel = Tunnel::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(tunnel,3);
                    break;
                }
                case tmxSociety:
                {
                    _society = Society::createFixture(_world, layer, x, y, 1.0, 3.0);
                    _platformsGroup->addChild(_society,0);
                    
                    if(kCurrentLevel == 9)
                        _society->setDynamic();
                    break;
                }
                case tmxPolice:
                {
                    auto police = Police::createFixture(_world, layer, x, y, 1.0, 2.0);
                    _platformsGroup->addChild(police,0);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void GameScene::animateMapIn()
{
    if(_night != nullptr)
    {
        auto thunder = CallFunc::create([this](){ /*CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_THUNDER);*/});
        _night->runAction(FadeTo::create(0.7, 250));
        _night->runAction(RepeatForever::create(Sequence::create(DelayTime::create(8),thunder, NULL)));
        _night->runAction(RepeatForever::create(Sequence::create(DelayTime::create(10),FadeTo::create(0.3, 100),FadeTo::create(0.3, 250), NULL)));
        _night->runAction(RepeatForever::create(Sequence::create(DelayTime::create(3),FadeTo::create(0.1, 100),FadeTo::create(0.1, 250), NULL)));
    }
    _parent->runAction(Sequence::create(DelayTime::create(1.0f),EaseCubicActionOut::create(MoveTo::create(1, _screenInPosition)),NULL));
}

void GameScene::animateMapOut()
{
    if(_night == nullptr)
    {
        _parent->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(1, _screenOutPosition),NULL));
    }
    else
    {
        _parent->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(1, _screenOutPosition),NULL));
        _night->stopAllActions();
//        _night->setOpacity(250);
        _night->runAction(Sequence::create(DelayTime::create(1.5),FadeTo::create(1,100),NULL));
    }
}

void GameScene::BeginContact(b2Contact* contact)
{
    b2Fixture* f1 = contact->GetFixtureA();
    b2Fixture* f2 = contact->GetFixtureB();
    auto data1 = static_cast<userdataFormat *>(f1->GetUserData());
    auto data2 = static_cast<userdataFormat *>(f2->GetUserData());
    
    
    
    if(data1->a == tmxTeddy || data2->a == tmxTeddy)
    {
        if(data1->b == pFemale || data2->b == pFemale) //Is Female
        {
            if(_giftTurn == pFemale){
                _female->gift();
                _giftTurn = pMale;
            }
        }
        else if(data1->b == pMale || data2->b == pMale) //Is Male
        {
            if(_giftTurn == pMale){
                _male->gift();
                _playTimes++;
                _giftTurn = pFemale;
            }
        }
    }
    
    if(data1->a == tmxSociety || data2->a == tmxSociety)
    {
        if(data1->a == tmxFire || data2->a == tmxFire)
        {
            _blood++;
        }
    }
    
    if(f1->IsSensor() || f2->IsSensor())
    {
        if(data1->a == tmxPlatform || data2->a == tmxPlatform )
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setIsGround(true);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setIsGround(true);
            }
        }
        if(data1->a == tmxCloud || data2->a == tmxCloud )
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setIsGround(true);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setIsGround(true);
            }
        }
        if(data1->a == tmxBlock || data2->a == tmxBlock )
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setIsGround(true);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setIsGround(true);
            }
        }
        
        if(data1->a == tmxFire || data2->a == tmxFire)
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setIsDied(true);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setIsDied(true);
            }
        }
        if(data1->a == tmxWin || data2->a == tmxWin)
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setAtFinish(true);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setAtFinish(true);
                
                if(kCurrentLevel == 9)
                {
                    if(_society != nullptr)
                    {
                        auto callFunc = CallFunc::create([this](){
                            this->_society->move();
                        });
                        runAction(callFunc);
                    }
                }
            }
        }
        if(data1->a == tmxPoison || data2->a == tmxPoison )
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->poison();
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->poison();
            }
        }
        if(data1->a == tmxSociety || data2->a == tmxSociety)
        {
            if(data1->b == pMale || data2->b == pMale || data1->b == pFemale || data2->b == pFemale )
            {
                if(!_arrested)
                {
                    _arrested = true;
//                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_LOCK);
                    auto delay = DelayTime::create(1);
                    auto callFunc = CallFunc::create([this](){
                        loadDiedEnd();
                    });
                    runAction(Sequence::create(delay,callFunc, NULL));
                }
            }
            else if(data1->a == tmxTeddy || data2->a == tmxTeddy)
            {
                _fmod->playEvent("SocietyLaugh");
                _society->laugh();
            }
        }
        if(data1->a == tmxPolice || data2->a == tmxPolice)
        {
            if(!_arrested)
            {
                _arrested = true;
                if(data1->b == pFemale || data2->b == pFemale) //Is Female
                {
                    
                    this->_fmod->playEvent("Police");
                    auto delay = DelayTime::create(5);
                    auto callFunc = CallFunc::create([this](){
                        loadDiedEnd();
                    });
                    runAction(Sequence::create(delay,callFunc, NULL));
                }
                if((data1->b == pMale || data2->b == pMale))
                {
                    this->_fmod->playEvent("Police");
                    auto delay = DelayTime::create(1);
                    auto callFunc = CallFunc::create([this](){
                        loadDiedEnd();
                    });
                    runAction(Sequence::create(delay,callFunc, NULL));
                }
            }
        }
        if(data1->a == tmxTemple || data2->a == tmxTemple)
        {
            if(!_god)
            {
                _god = true;
                for (auto c : _listOfDoors) {
                    c->fall();
                }
                _temple->setVisible(false);
                auto callFunc = CallFunc::create([this](){
                    this->loadInstuctionsEnd();
                });
                
//                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_NO);
                runAction(Sequence::create(DelayTime::create(5),callFunc, NULL));
            }
        }
    }
}

void GameScene::EndContact(b2Contact* contact)
{
    b2Fixture* f1 = contact->GetFixtureA();
    b2Fixture* f2 = contact->GetFixtureB();
    auto data1 = static_cast<userdataFormat *>(f1->GetUserData());
    auto data2 = static_cast<userdataFormat *>(f2->GetUserData());
    
    
    if(f1->IsSensor() || f2->IsSensor())
    {
        if(data1->a == tmxPlatform || data2->a == tmxPlatform )
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setIsGround(false);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setIsGround(false);
            }
        }
        if(data1->a == tmxBlock || data2->a == tmxBlock )
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setIsGround(false);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setIsGround(false);
            }
        }
        if(data1->a == tmxCloud || data2->a == tmxCloud )
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setIsGround(false);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setIsGround(false);
            }
            
            if(data1->a == tmxCloud)
            {
                static_cast<Cloud*>(data1->node)->del();
            }
            else
            {
                static_cast<Cloud*>(data2->node)->del();
            }
        }
        if(data1->a == tmxWin || data2->a == tmxWin)
        {
            if(data1->b == pFemale || data2->b == pFemale) //Is Female
            {
                _female->setAtFinish(false);
            }
            else if(data1->b == pMale || data2->b == pMale) //Is Male
            {
                _male->setAtFinish(false);
            }
        }
    }
}



void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _male->onKeyPressed(keyCode, event);
    if(_female != nullptr)
        _female->onKeyPressed(keyCode, event);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    _male->onKeyReleased(keyCode, event);
    if(_female != nullptr)
        _female->onKeyReleased(keyCode, event);
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_R:
            transitionToGameScene();
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            if(_gameState != gsEnd)
            {
                skip();
            }
            break;
        default:
            break;
    }
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}


void GameScene::onTouchEnded(Touch* touch, Event* event)
{
}

void GameScene::toGameScene()
{
    
    auto size = Director::getInstance()->getWinSize();
    auto clipper = ClippingNode::create();
    
    clipper->setAnchorPoint(Point(0.5f, 0.5f));
    clipper->setPosition(size.width / 2, size.height / 2);
    clipper->setAlphaThreshold(0.05f);
    clipper->setInverted(true);
    
    Sprite* blackRect = Sprite::create("images/black_screen.png");
    blackRect->setScale(size.width/blackRect->getContentSize().width, size.height/blackRect->getContentSize().height);
    clipper->addChild(blackRect);
    
    auto heart = Sprite::create("images/heart.png");
    heart->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    heart->setScale(0.0);
    clipper->setStencil(heart);	//set the cut triangle in the ClippingNode.
    
    this->addChild(clipper, 500);
    auto callFunc = CallFuncN::create(CC_CALLBACK_0(GameScene::startGame,this));
    heart->runAction(EaseSineIn::create(Spawn::create(ScaleTo::create(2.5f, (size.width/heart->getContentSize().width)*1.3),
                                                      RotateBy::create(2.5f, 540),
                                                      Sequence::create(DelayTime::create(2.5),
                                                                       callFunc, NULL), NULL)));
    
}

void GameScene::transitionToGameScene()
{
    auto size = Director::getInstance()->getWinSize();
    auto clipper = ClippingNode::create();
    
    clipper->setAnchorPoint(Point(0.5f, 0.5f));
    clipper->setPosition(size.width / 2, size.height / 2);
    clipper->setAlphaThreshold(0.05f);
    clipper->setInverted(true);
    
    Sprite* blackRect = Sprite::create("images/black_screen.png");
    blackRect->setScale(size.width/blackRect->getContentSize().width, size.height/blackRect->getContentSize().height);
    clipper->addChild(blackRect);
    
    auto heart = Sprite::create("images/heart.png");
    heart->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    heart->setScale((size.width/heart->getContentSize().width)*1.3);
    clipper->setStencil(heart);	//set the cut triangle in the ClippingNode.
    
    this->addChild(clipper, 500);
    auto callFunc = CallFuncN::create(CC_CALLBACK_0(GameScene::restartScene,this));
    heart->runAction(EaseSineOut::create(Spawn::create(ScaleTo::create(2.5f, 0.0f, 0.0f),
                                                       RotateBy::create(2.5f, 540),
                                                       Sequence::create(DelayTime::create(2.5),
                                                                        callFunc, NULL), NULL)));
    
}

void GameScene::restartScene()
{
    //get the game scene and run it.
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
}


void GameScene::nextTrack(std::string name)
{
    auto img = Sprite::create(IMG_BG);
    img->setScale(Util::getScreenRatioHeight(img)*0.05, Util::getScreenRatioHeight(img)*0.1);
    img->setPosition(Vec2(_visibleSize.width*0.50,_visibleSize.height*1.2));
    img->setColor(RGB_BLUE);
    this->addChild(img);
    
    auto l = Label::createWithTTF(name, FONT, 24);
    l->setColor(RGB_WHITE);
    l->setPositionX(10);
    l->setPositionY(img->getBoundingBox().size.width/2);
    l->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    img->addChild(l);
    
    auto move = MoveTo::create(1.0, Vec2(_visibleSize.width*0.50,_visibleSize.height*0.8));
    auto del = CallFunc::create([img](){img->removeFromParentAndCleanup(true);});
    img->runAction(Sequence::create(move,move->reverse(),del, NULL));
    
}

void GameScene::startGame()
{
    
    nextTrack("First");
    //TODO : something
    auto obj = getChildByTag(tagMenu);
    obj->runAction(FadeIn::create(1));
}

void GameScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(GameScene::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void GameScene::onDraw(const Mat4 &transform, uint32_t flags)
{
    
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when seting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    CHECK_GL_ERROR_DEBUG();
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    _world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    //end draw
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
