//
//  GameScene.cpp
//  jazz
//
//  Created by Sanchit Gulati on 11/08/14.
//
//

#include "GameScene.h"

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
    kCurrentLevel = 1;
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    _eventDispatcher = Director::getInstance()->getEventDispatcher();
    _eventDispatcher->removeAllEventListeners();
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _parent = Node::create();
    _parent->setAnchorPoint(Point(0.5,0.5));
    this->addChild(_parent,zGame);
    
    
    createPhysicalWorld();
    loadLevel(0);// for testing
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
    _platformsGroup->setVisible(false);
    _playerGroup->setVisible(false);
    _bgGroup->setVisible(false);
    /*end*/
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(SFX_BG_HAPPY,true);
    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    log("sender id %d",((Node*)pSender)->getTag());
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
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
        //Dynamic Scaling
        auto diffX = fabs(_male->getSprite()->getPosition().x - _female->getSprite()->getPosition().x);
        auto diffY = fabs( _male->getSprite()->getPosition().y - _female->getSprite()->getPosition().y);
        auto maxScale = MAX(diffX, diffY);
        _parent->setScale((((_parent->getContentSize().width-maxScale)/_parent->getContentSize().width)*0.5)+0.9);
        
        
        //End 
        
        if(_male->getAtFinish() && _female->getAtFinish())
        {
            kCurrentLevel ++; //increment level
            Director::getInstance()->replaceScene((Scene*)GameScene::create());
        }
        
        if(!_male->getIsAlive())
        {
            auto emitter = ParticleSystemQuad::create("particle_texture.plist");
            _playerGroup->addChild(emitter, 999);
            emitter->setPosition(_male->getSprite()->getPosition());
            _world->DestroyBody(_male->getB2Body());
            _male->removeFromParentAndCleanup(true);
            _gsGamePlaying = false;
        }
        if(!_female->getIsAlive())
        {
            auto emitter = ParticleSystemQuad::create("particle_texture.plist");
            _playerGroup->addChild(emitter, 999);
            emitter->setPosition(_female->getSprite()->getPosition());
            _world->DestroyBody(_female->getB2Body());
            _female->removeFromParentAndCleanup(true);
            _gsGamePlaying = false;
            
        }
        
    }
}


void GameScene::loadData()
{
    //todo: leaderboards
}

void GameScene::loadInstuctions()
{
    auto screenSize = Director::getInstance()->getWinSize();
    
    auto titleBackground = Sprite::create("images/bg.png");
    titleBackground->setColor(Color3B::BLACK);
    titleBackground->setPosition(screenSize.width/2, screenSize.height/2);
    titleBackground->setScaleX(Util::getScreenRatioWidth(titleBackground));
    titleBackground->setScaleY(Util::getScreenRatioHeight(titleBackground));
    this->addChild(titleBackground);
    
    auto valuekey = _tm->getProperties();
    auto labelTitle = Label::createWithTTF(valuekey["title"].asString().c_str(), FONT, 64);
    labelTitle->setWidth(screenSize.width/2);
    
    auto contentSize = titleBackground->getBoundingBox().size;
    labelTitle->setPosition(Point(contentSize.width/2,contentSize.height/2));
    labelTitle->setAnchorPoint(Point(0.5,0.5));
    titleBackground->addChild(labelTitle);
    
    
    auto pos = titleBackground->getPosition();
    titleBackground->setPositionY(pos.y + contentSize.height);
    auto awayPos = titleBackground->getPosition();
    auto easeIn = EaseIn::create(MoveTo::create(1.0, pos), 1.0);
    auto easesOut = EaseOut::create(MoveTo::create(1.0, awayPos), 1.0);
    auto delay  = DelayTime::create(1.0f);
    auto seq = Sequence::create(easeIn,delay,easesOut, NULL);
    titleBackground->runAction(seq);
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
    _parent->addChild(_bgGroup,zBackground);
    
    
    auto _bg1 = Sprite::create(IMG_BG);
    _bg1->setColor(RGB_WHITE);
    _bg1->setPosition(cocos2d::Point(_visibleSize.width/2.0f,_visibleSize.height/2.0f));
    _bg1->setScaleX( Util::getScreenRatioWidth(_bg1)*1.5);
    _bg1->setScaleY(Util::getScreenRatioHeight(_bg1)*1.5);
    _bgGroup->addChild(_bg1);
    
    auto loadLevelString = StringUtils::format("levels/%d.tmx",kCurrentLevel);
    _tm = TMXTiledMap::create(loadLevelString);
    _tm->setVisible(false);
    _platformsGroup = Node::create();
    _parent->addChild(_platformsGroup);
    
    _parent->setPosition(Point(_visibleSize.width/2,_visibleSize.height/2));
    _parent->setContentSize(Size(_tm->getMapSize().width * _tm->getTileSize().width,
                                 _tm->getMapSize().height * _tm->getTileSize().height));
    _platformsGroup->setContentSize(_parent->getContentSize());
    
    
    
    _tm->retain();
    
    _playerGroup = Node::create();
    _parent->addChild(_playerGroup);

    prepareLayers();
    
    _bg1->setAnchorPoint(Point(0.5,0.5));
    auto bgSize = _platformsGroup->getBoundingBox().size;
    _bg1->setPosition(bgSize.width/2,bgSize.height/2);
    
    
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
                    _platformsGroup->addChild(platform);
                    break;
                }
                case tmxFemale:
                    _female = Player::createPlayerFixture(_world,layer, x, y, 1.0f, 2.0f,pFemale);
                    _playerGroup->addChild(_female);
                    break;
                case tmxMale:
                    _male = Player::createPlayerFixture(_world,layer, x, y, 1.0f, 2.0f,pMale);
                    _playerGroup->addChild(_male);
                    break;
                case tmxFire:
                {
                    auto death = Death::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(death);
                    break;
                }
                case tmxWin:
                {
                    auto win = Win::createFixture(_world, layer, x, y, 3.0, 3.0); //bcoz image is 96*96
                    _platformsGroup->addChild(win);
                    break;
                }
                case tmxWall:
                {
                    auto wall = Wall::createFixture(_world,layer, x, y, 1.0f, 1.0f);
                    _platformsGroup->addChild(wall);
                    break;
                    
                }
                case tmxBlock:
                {
                    auto block = Block::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(block);
                }
                default:
                    break;
            }
        }
    }
}


void GameScene::BeginContact(b2Contact* contact)
{
    b2Fixture* f1 = contact->GetFixtureA();
    b2Fixture* f2 = contact->GetFixtureB();
    
    if(f1->IsSensor() || f2->IsSensor())
    {
        auto data1 = static_cast<userdataFormat *>(f1->GetUserData());
        auto data2 = static_cast<userdataFormat *>(f2->GetUserData());
        int arraySensor[4] = {sensorNone,sensorNone,sensorNone,sensorNone}; // 4 sensors
        int count = 0;
        bool die = false;
        bool isFinished = false;
        bool isBottomSet = false;
        
        if(data1->a == tmxPlatform || data2->a == tmxPlatform )
        {
            if( data1->a ==  sensorLeft || data2->a == sensorLeft)
            {
                arraySensor[count++] = sensorLeft;
            }
            else if( data1->a ==  sensorRight || data2->a == sensorRight)
            {
                arraySensor[count++] = sensorRight;
            }
            else if( data1->a ==  sensorBottom || data2->a == sensorBottom)
            {
                arraySensor[count++] = sensorBottom;
                isBottomSet = true;
            }
        }
        if(!isBottomSet)
        {
            if(data1->a == tmxBlock || data2->a == tmxBlock )
            {
                if( data1->a ==  sensorBottom || data2->a == sensorBottom)
                {
                    arraySensor[count++] = sensorBottom;
                }
            }
        }
        
        if(data1->a == tmxFire || data2->a == tmxFire)
        {
            die = true;
        }
        if(data1->a == tmxWin || data2->a == tmxWin)
        {
            isFinished = true;
        }
        
        if(data1->b == pFemale || data2->b == pFemale)
        {
            _female->setAtFinish(isFinished);
            _female->setIsTouching(arraySensor,true,die);
        }
        else if(data1->b == pMale || data2->b == pMale)
        {
            _male->setAtFinish(isFinished);
            _male->setIsTouching(arraySensor,true,die);
        }
    }
}

void GameScene::EndContact(b2Contact* contact)
{
    b2Fixture* f1 = contact->GetFixtureA();
    b2Fixture* f2 = contact->GetFixtureB();
    
    if(f1->IsSensor() || f2->IsSensor())
    {
        auto data1 = static_cast<userdataFormat *>(f1->GetUserData());
        auto data2 = static_cast<userdataFormat *>(f2->GetUserData());
        
        int arraySensor[4] = {sensorNone,sensorNone,sensorNone,sensorNone}; // 4 sensors
        int count = 0;
        bool isBottomSet = false;
        
        if(data1->a == tmxPlatform || data2->a == tmxPlatform )
        {
            if( data1->a ==  sensorLeft || data2->a == sensorLeft)
            {
                arraySensor[count++] = sensorLeft;
            }
            else if( data1->a ==  sensorRight || data2->a == sensorRight)
            {
                arraySensor[count++] = sensorRight;
            }
            else if( data1->a ==  sensorBottom || data2->a == sensorBottom)
            {
                arraySensor[count++] = sensorBottom;
                isBottomSet = true;
            }
        }
        
        if(!isBottomSet)
        {
            if(data1->a == tmxBlock || data2->a == tmxBlock )
            {
                if( data1->a ==  sensorBottom || data2->a == sensorBottom)
                {
                    arraySensor[count++] = sensorBottom;
                }
            }
        }
        
        if(data1->a == tmxWin || data2->a == tmxWin)
        {
            if(data1->b == pFemale || data2->b == pFemale)
                _female->setAtFinish(false);
            else if(data1->b == pMale || data2->b == pMale)
                _male->setAtFinish(false);
        }
        
        if(data1->b == pFemale || data2->b == pFemale)
        {
            _female->setIsTouching(arraySensor,false);
        }
        else if(data1->b == pMale || data2->b == pMale)
        {
            _male->setIsTouching(arraySensor,false);
        }

    }
}



void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _male->onKeyPressed(keyCode, event);
    _female->onKeyPressed(keyCode, event);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    _male->onKeyReleased(keyCode, event);
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
