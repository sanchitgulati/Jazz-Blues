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
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _parent = Node::create();
    _parent->setAnchorPoint(Point(0.5,0.5));
    this->addChild(_parent);
    
    
    createPhysicalWorld();
    loadLevel(0);// for testing
    
    // schedule the update
    this->schedule(schedule_selector(GameScene::update), kUpdateInterval);
    
    
    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameScene::update(float dt)
{
    _world->Step(dt, 8, 1);
    if(_gsGamePlaying)
    {
        //Dynamic Scaling
        auto diffX = fabs(_male->getSprite()->getPosition().x - _female->getSprite()->getPosition().x);
        auto diffY = fabs( _male->getSprite()->getPosition().y - _female->getSprite()->getPosition().y);
        auto maxScale = MAX(diffX, diffY);
        _parent->setScale((((_parent->getContentSize().width-maxScale)/_parent->getContentSize().width)*0.5)+0.9);
        //End
        
        
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


void GameScene::createPhysicalWorld()
{
    _world = new b2World(b2Vec2(0.0f, kGravity)); //kGravity
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    auto _debugDraw = new GLESDebugDraw( kPixelsPerMeter );
//    _world->SetDebugDraw(_debugDraw);
    _world->SetContactListener(this);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
//    flags += b2Draw::e_jointBit;
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
    _parent->addChild(_bgGroup);
    
    
    auto _bg1 = Sprite::create(IMG_BG);
    _bg1->setColor(Color3B::WHITE);
    _bg1->setPosition(cocos2d::Point(_visibleSize.width/2.0f,_visibleSize.height/2.0f));
    _bgGroup->addChild(_bg1);
    
    _tm = TMXTiledMap::create("levels/2.tmx");
    _tm->setVisible(false);
    _platformsGroup = Node::create();
    _parent->addChild(_platformsGroup);
    
    //TODO: implement camera system
    _parent->setPosition(480,320);
    _parent->setContentSize(Size(_tm->getMapSize().width * _tm->getTileSize().width,
                                 _tm->getMapSize().height * _tm->getTileSize().height));
    _platformsGroup->setContentSize(_parent->getContentSize());
    
    
    
    _tm->retain();
    
    _playerGroup = Node::create();
    _parent->addChild(_playerGroup);

    prepareLayers();
    
    _bg1->setScaleX( ((_tm->getMapSize().width-1) * _tm->getTileSize().width)/_bg1->getBoundingBox().size.width );
    _bg1->setScaleY( ((_tm->getMapSize().height-1) * _tm->getTileSize().height)/_bg1->getBoundingBox().size.height  );
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
            this->createFixturesSecondPass(layer);
        }
    }
}

void GameScene::createFixturesSecondPass(TMXLayer* layer)
{
    // create all the rectangular fixtures for each tile
    Size layerSize = layer->getLayerSize();
    for (int x = 0; x < layerSize.width; x++)
    {
        
        int length = 1;
        int lx,ly = -1;
        for (int y = 0; y < layerSize.height; y++)
        {
            auto tileGID = layer->getTileGIDAt(Point(x,y));
            switch (tileGID) {
                case tmxEmpty:
                    break;
                case tmxPlatform: // platform
                {
                    uint32_t nextElementY = tmxEmpty;
                    if(y < _tm->getMapSize().height-1)
                        nextElementY = layer->getTileGIDAt(Point(x,y+1));
                    if(nextElementY == tmxPlatform)
                    {
                        if(lx == -1 || ly == -1)
                        {
                            lx = x;
                            ly = y;
                        }
                        length++;
                    }
                    else
                    {
                        if(lx == -1 || ly == -1)
                        {
                            lx = x;
                            ly = y;
                        }
                        if(length != 1)
                        {
                            auto platform = Platform::createFixture(_world,layer, lx, ly, 1.0f, 1.0f,length,true);
                            _platformsGroup->addChild(platform);
                        }
                        length = 1;
                        lx = ly = -1;
                    }
                    break;

                    
                }
            }
        }
    }
    
}
void GameScene::createFixturesFirstPass(TMXLayer* layer)
{
    // create all the rectangular fixtures for each tile
    Size layerSize = layer->getLayerSize();
    for (int y = 0; y < layerSize.height; y++)
    {
        
        int length = 1;
        int lx,ly = -1;
        for (int x = 0; x < layerSize.width; x++)
        {
            // create a fixture if this tile has a sprite
//            auto tileSprite = layer->getTileAt(Point(x, y));
            auto tileGID = layer->getTileGIDAt(Point(x,y));
            switch (tileGID) {
                case tmxEmpty:
                    break;
                case tmxPlatform: // platform
                {
                    uint32_t nextElementX;
                    if(x < _tm->getMapSize().width-1)
                        nextElementX = layer->getTileGIDAt(Point(x + 1,y));
                    else
                        nextElementX = tmxEmpty;
                    
                    
                    if(nextElementX == tmxPlatform)
                    {
                        if(lx == -1 || ly == -1)
                        {
                            lx = x;
                            ly = y;
                        }
                        layer->setTileGID(tmxEmpty,Vec2(x, y)); //if used, delete from map
                        length++;
                    }
                    else
                    {
                        if(lx == -1 || ly == -1)
                        {
                            lx = x;
                            ly = y;
                        }
                        if(length != 1)
                        {
                            layer->setTileGID(tmxEmpty,Vec2(x, y)); //if used, delete from map
                            auto platform = Platform::createFixture(_world,layer, lx, ly, 1.0f, 1.0f,length);
                            _platformsGroup->addChild(platform);

                        }
                        length = 1;
                        lx = ly = -1;
                    }
                    break;
                }
                case tmxFemale:
                    _female = Player::createPlayerFixture(_world,layer, x, y, 1.0f, 1.0f,pFemale);
                    _playerGroup->addChild(_female);
                    break;
                case tmxMale:
                    _male = Player::createPlayerFixture(_world,layer, x, y, 1.0f, 1.0f,pMale);
                    _playerGroup->addChild(_male);
                    break;
                case tmxFire:
                {
                    auto death = Death::createFixture(_world, layer, x, y, 1.0, 1.0);
                    _platformsGroup->addChild(death);
                    break;
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
        
        if(data1->a == tmxPlatform || data2->a == tmxPlatform)
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
            }
        }
        if(data1->a == tmxFire || data2->a == tmxFire)
        {
            die = true;
        }
        
        if(data1->b == pFemale)
            _female->setIsTouching(arraySensor,true,die);
        else if(data1->b == pMale)
            _male->setIsTouching(arraySensor,true,die);
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
        
        if(data1->a == tmxPlatform || data2->a == tmxPlatform)
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
            }
        }
        if(data1->b == pFemale)
            _female->setIsTouching(arraySensor,false);
        else if(data1->b == pMale)
            _male->setIsTouching(arraySensor,false);

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
//    _renderTex->beginWithClear(0, 0, 0, 0);
//    _parent->visit(renderer, transform, flags);
//    _renderTex->end();
    
    
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
