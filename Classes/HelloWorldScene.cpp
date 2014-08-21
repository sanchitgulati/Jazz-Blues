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
    
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    
    auto director = Director::getInstance();
    auto dispatcher = director->getEventDispatcher();
    touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    touchListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

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

    
    auto map = TMXTiledMap::create("levels/1.tmx");
    map->setAnchorPoint(Point(0.5,0.5));
    map->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    this->addChild(map,1);
    
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
            auto GID = layer->getTileGIDAt(Vec2(x, y));
            if(GID == 3 || GID == 4)
            {
                auto player = Male::create();
                Texture2D* femaleTex = layer->getTileAt(Vec2(x, y))->getTexture();
                player->setSprite(femaleTex);
                player->setPosition(x*map->getTileSize().width, y*map->getTileSize().height);
                layer->setTileGID(0,Vec2(x, y));
                this->addChild(player,2);
                if(GID == 3)
                {
                    player->setColor(Color3B::GREEN);
                }
                else if(GID == 4)
                {
                    player->setColor(Color3B::ORANGE);
                }
            }
        }
    }
    
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
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
        case EventKeyboard::KeyCode::KEY_K:
            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            break;
        case  EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            break;
        case  EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            break;
        default:
            break;
    }
    
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    
    switch (keyCode) {
        case  EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            break;
        case  EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
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
