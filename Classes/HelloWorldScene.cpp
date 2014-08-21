#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto director = Director::getInstance();
    auto dispatcher = director->getEventDispatcher();
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    touchListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    _pressedLeft = false;
    _pressedRight = false;
    _pressedUp = false;
    

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Adam And Eve", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    sprite->setScale(5, 5);

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    auto _platformGroup = Node::create();
    _platformGroup->setAnchorPoint(Point(0.0,0.0));
    this->addChild(_platformGroup,1);
    
    auto map = TMXTiledMap::create("levels/1.tmx");
    map->setAnchorPoint(Point(0.0,0.0));
//    _platformGroup->setContentSize(map->getContentSize());
    _platformGroup->addChild(map,1);
    
    auto mapContentSize = map->getContentSize();
    _platformGroup->setPosition(Point((visibleSize.width - mapContentSize.width)/2,
                                      (visibleSize.height - mapContentSize.height)/2));
    
    for (const auto& child : map->getChildren())
    {
        static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();
    }
    
    auto layer = map->getLayer("level");
    Size s = layer->getLayerSize();
    for (int x = 0; x < s.width; ++x)
    {
        for (int y = 0; y < s.height; ++y)
        {
            auto sprite = layer->getTileAt(Vec2(x,y));
            auto GID = layer->getTileGIDAt(Vec2(x, y));
            if(GID != 0) //not empty
            {
                sprite->setAnchorPoint(Point(0.5,0.5));
                auto getpos = sprite->getPosition();
                sprite->setPosition(getpos.x + sprite->getBoundingBox().size.width/2,getpos.y +  sprite->getBoundingBox().size.height/2);
            }
            if(GID == 2) //fire
            {
                sprite->runAction(RepeatForever::create(RotateBy::create(1.0, 360)));
            }
            if(GID == 3 || GID == 4)
            {
                auto player = Male::create();
                Texture2D* tex = layer->getTileAt(Vec2(x, y))->getTexture();
                player->setSprite(tex,(GID-1)*32,0*32,32,32);
                player->setPosition(x*map->getTileSize().width, (map->getMapSize().height-y-1)*map->getTileSize().height);
                
                layer->setTileGID(0,Vec2(x, y));
                
                player->setMapRef(map);
                _platformGroup->addChild(player,2);
                if(GID == 4)
                {
                    _male = player;
                }
                else if(GID == 3)
                {
                    _female = player;
                    _female->setInverted(true);
                }
            }
        }
    }
    
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float dt)
{
    if(_pressedLeft)
    {
        _male->moveLeft();
        _female->moveLeft();
    }
    if(_pressedRight)
    {
        _male->moveRight();
        _female->moveRight();
    }
    if(_pressedUp)
    {
//        _male->jump(true);
//        _female->jump(true);
    }
    else
    {
//        _male->jump();
//        _female->jump();
    }
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    
    for (auto& touch : touches)
    {
        if(!touch)
            break;
        
        auto location = touch->getLocation();
    }

}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            _pressedUp = true;
            break;
        case  EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _pressedLeft = true;
            break;
        case  EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _pressedRight = true;
            break;
        default:
            break;
    }
    
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            _pressedUp = false;
            break;
        case  EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _pressedLeft = false;
            break;
        case  EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _pressedRight = false;
            break;
        default:
            break;
    }
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
