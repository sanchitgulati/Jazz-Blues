//
//  Button.cpp
//  jazzblues
//
//  Created by Sanchit Gulati on 06/09/14.
//
//

#include "Button.h"
using namespace cocos2d;

int Button::identifier = 0;

Button::Button(const char *normalSprite, const char *selectedSprite) {
	setAnchorPoint(Vec2(0.5, 0.5));
	isPressed = false;
    
	_normalSprite = Sprite::create(normalSprite);
	_selectedSprite = Sprite::create(selectedSprite);
	_selectedSprite->setVisible(false);
	addChild(_normalSprite);
	addChild(_selectedSprite);
    
    this->setContentSize(Size(_normalSprite->getContentSize().width,_normalSprite->getContentSize().height));
    
    // Adds touch event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(Button::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
}


Button* Button::create(const char *normalSprite, const char *selectedSprite) {
	Button *button = new Button(normalSprite, selectedSprite);
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
        log("%s",(char *)getUserData());
        return true;
    }
    return false;
}


void Button::onTouchEnded(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    if (this->getBoundingBox().containsPoint(touchLocation)) {
        isPressed = true;
        _selectedSprite->setVisible(false);
        _normalSprite->setVisible(true);
    }
}
