//
//  Button.cpp
//  jazzblues
//
//  Created by Sanchit Gulati on 06/09/14.
//
//

#include "Button.h"
#include "GameObjects/Player.h"
using namespace cocos2d;


Button::Button(const char *normalSprite, const char *selectedSprite,cocos2d::Node* target1,cocos2d::Node* target2) {
	isPressed = false;
    _target1 = target1;
    _target2 = target2;
    setAnchorPoint(Vec2(0.5, 0.5));
	_normalSprite = Sprite::create(normalSprite);
	_selectedSprite = Sprite::create(selectedSprite);
	_selectedSprite->setVisible(false);
    _normalSprite->setAnchorPoint(Vec2(0.0, 0.0));
    _selectedSprite->setAnchorPoint(Vec2(0.0, 0.0));
    
    auto boundingBox = _normalSprite->getBoundingBox().size;
    this->setContentSize(boundingBox);
    this->getBoundingBox().setRect(0, 0, boundingBox.width, boundingBox.height);
    
    
    /* Testing */
//    auto drawNode = DrawNode::create();
//    drawNode->drawDot(getPosition(), 2, Color4F::BLACK);
//    drawNode->drawDot(Point(getPositionX() + boundingBox.width,getPositionY() + boundingBox.height), 2, Color4F::BLUE);
//    addChild(drawNode,9090);
    /* End Testing */
    
    // Adds touch event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    
    listener->onTouchBegan = CC_CALLBACK_2(Button::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
	addChild(_normalSprite);
	addChild(_selectedSprite);
}


Button* Button::create(const char *normalSprite, const char *selectedSprite,cocos2d::Node* target1,cocos2d::Node* target2) {
	Button *button = new Button(normalSprite, selectedSprite,target1,target2);
	button->autorelease();
	return button;
}

bool Button::onTouchBegan(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    if (this->getBoundingBox().containsPoint(touchLocation)) {
        isPressed = true;
        _selectedSprite->setVisible(true);
        _normalSprite->setVisible(false);
        static_cast<Player *>(_target1)->onButtonPressed(getUserData());
        static_cast<Player *>(_target2)->onButtonPressed(getUserData());
        return true;
    }
    return false;
}


void Button::onTouchEnded(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    isPressed = true;
    _selectedSprite->setVisible(false);
    _normalSprite->setVisible(true);
    static_cast<Player *>(_target1)->onButtonReleased(getUserData());
    static_cast<Player *>(_target2)->onButtonReleased(getUserData());
}

void Button::setTarget(cocos2d::Node *target1,cocos2d::Node *target2)
{
    _target1=target1;
    _target2=target2;
}

//void Button::setCallback(cocos2d::Ref* target)
//{
//    this->mTarget = target;
//}
