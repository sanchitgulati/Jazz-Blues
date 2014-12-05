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
    _gsGamePlaying = true;
    kCurrentLevel = UserDefault::getInstance()->getIntegerForKey("continue",1);
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _screenOutPosition = Vec2(_visibleSize.width/2,-1*_visibleSize.height/2);
    _screenInPosition = Vec2(_visibleSize.width/2,_visibleSize.height/2);
    
    _eventDispatcher = Director::getInstance()->getEventDispatcher();
    _eventDispatcher->removeAllEventListeners();
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
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
    levelSelector->setPositionX(margin);
    
    auto refresh = MenuItemImage::create(IMG_REFRESH_0,IMG_REFRESH_1,CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    refresh->setTag(menuRefresh);
    refresh->setPositionX(margin*2);
    
    auto unmute = MenuItemImage::create(IMG_UNMUTE_0,IMG_UNMUTE_1);
    unmute->setTag(menuUnmute);
    
    auto mute = MenuItemImage::create(IMG_MUTE_0,IMG_MUTE_1);
    mute->setTag(menuMute);
    
    auto sfxToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(GameScene::menuCloseCallback, this), mute,unmute, NULL);
    sfxToggle->setTag(menuToggle);
    sfxToggle->setPositionX(margin*3);
    
    auto menu = Menu::create(levelSelector,refresh,sfxToggle, NULL);
    menu->setPosition(Point(0,_visibleSize.height-margin));
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
//    _platformsGroup->setVisible(false);
//    _playerGroup->setVisible(false);
//    _bgGroup->setVisible(false);
    /*end*/
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SFX_BG_HAPPY,true);
    return true;
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
            
            auto scene = (Scene*)GameScene::create();
            Director::getInstance()->replaceScene(scene);
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
    if(_gsGamePlaying)
    {
        if(_female != nullptr)
        {
            //Dynamic Scaling
            auto diffX = fabs(_male->getSprite()->getPosition().x - _female->getSprite()->getPosition().x);
            auto diffY = fabs( _male->getSprite()->getPosition().y - _female->getSprite()->getPosition().y);
            auto maxScale = MAX(diffX, diffY);
            _parent->setScale(_visibleSize.height/(_parent->getContentSize().height + (maxScale*0.5)));
            
            //End
            if(_male->getAtFinish() && _female->getAtFinish())
            {
                kCurrentLevel ++; //increment level
                UserDefault::getInstance()->setIntegerForKey("continue", kCurrentLevel);
                UserDefault::getInstance()->flush();
                loadInstuctionsEnd();
            }
            
            if(!_male->getIsAlive())
            {
                auto emitter = ParticleSystemQuad::create("particle_texture.plist");
                _platformsGroup->addChild(emitter, 999);
                emitter->setPosition(_male->getSprite()->getPosition());
                _world->DestroyBody(_male->getB2Body());
                _male->removeFromParentAndCleanup(true);
                _gsGamePlaying = false;
            }
            if(!_female->getIsAlive())
            {
                auto emitter = ParticleSystemQuad::create("particle_texture.plist");
                _platformsGroup->addChild(emitter, 999);
                emitter->setPosition(_female->getSprite()->getPosition());
                _world->DestroyBody(_female->getB2Body());
                _female->removeFromParentAndCleanup(true);
                _gsGamePlaying = false;
                
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
            cf = CallFunc::create([this]()
                                  {
                                      CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_TYPE_END);
                                      this->animateMapIn();
                                  });
            
            c->runAction(Sequence::create(dt,cf,fi,fadeOutMessage,NULL));
        }
        else
        {
            cf = CallFunc::create([this]()
                                  {
                                      auto toss = Util::toss();
                                      if(toss)
                                          CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_TYPE01);
                                      else
                                          CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_TYPE02);
                                  });
            
            c->runAction(Sequence::create(dt,cf,fi, NULL));
        }
        
        
    }
    
    
    this->addChild(labelTitle);
    
}

void GameScene::loadInstuctionsEnd()
{
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
    
    auto len = valuekey["end"].asString().length() - 1;
    
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
            cf = CallFunc::create([this]()
                                  {
                                      Director::getInstance()->replaceScene((Scene*)GameScene::create());
                                  });
            
            c->runAction(Sequence::create(dt,fi,cf,NULL));
        }
        else
        {
            cf = CallFunc::create([this]()
                                  {
                                      auto toss = Util::toss();
                                      if(toss)
                                          CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_TYPE01);
                                      else
                                          CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SFX_TYPE02);
                                  });
            
            c->runAction(Sequence::create(dt,cf,fi, NULL));
        }
        
        
    }
    
    
    this->addChild(labelTitle);
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
                    auto win = Win::createFixture(_world, layer, x, y, 3.0, 3.0); //bcoz image is 96*96
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
                    _platformsGroup->addChild(temple,1);
                    break;
                }
                case tmxCloud:
                {
                    auto cloud = Cloud::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(cloud,1);
                    break;
                }
                case tmxDoor:
                {
                    auto door = Door::createFixture(_world, layer, x, y, 1.0, 1.0);
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
                    _platformsGroup->addChild(tunnel,1);
                    break;
                }
                case tmxSociety:
                {
                    _society = Society::createFixture(_world, layer, x, y, 3.0, 2.0);
                    _platformsGroup->addChild(_society,0);
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
    _parent->runAction(Sequence::create(DelayTime::create(1.0f),EaseCubicActionOut::create(MoveTo::create(1, _screenInPosition)),NULL));
}

void GameScene::animateMapOut()
{
    _parent->runAction(MoveTo::create(1, _screenOutPosition));
}

void GameScene::BeginContact(b2Contact* contact)
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
            }
        }
        if(data1->a == tmxSociety || data2->a == tmxSociety)
        {
            _society->laugh();
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
                //TODO: Cloud deletion here
            }
            else
            {
                //TODO: Cloud deletion here
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
            Director::getInstance()->replaceScene((Scene*)GameScene::create());
            break;
        default:
            break;
    }
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
