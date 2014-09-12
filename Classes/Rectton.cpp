//
//  Rectton.cpp
//  roundels
//
//  Created by Sanchit Gulati on 08/06/14.
//
//

#include "Rectton.h"

USING_NS_CC;

enum{kZoomActionTag,kSlideActionTag};

Rectton::Rectton(std::string text,Color3B color)
{
    this->setEnabled(true);
    _originalScale = 0;
    this->setAnchorPoint(Point(0.5f, 0.5f));
    
    auto localContent = LocalizedString::localizedString(text.c_str())->getCString();
    this->lblText = Label::create(localContent, Constants::fontNameBold, Constants::fontSize);
    
    this->backSprite = Sprite::create(IMG_BUTTON_WHITE);
    this->backSprite->setScale(Util::getScreenRatioWidth(backSprite)*0.5);
    
    this->backSprite->setColor(color);
    auto sizeMenuItem = this->backSprite->getBoundingBox().size;
    
    this->setContentSize(sizeMenuItem);
    this->getBoundingBox().setRect(0, 0, sizeMenuItem.width, sizeMenuItem.height);
    this->setScale(Util::getScreenRatio(backSprite)*0.5);
    
    _originalScale = this->getScale();
    _originalPosition = this->getPosition();
    
    this->lblText->setColor(RGB_COLOR6);
    
    this->backSprite->setAnchorPoint(Point(0.0f,0.0f));
    
    this->lblText->setPosition(Point(sizeMenuItem.width/2.0f,sizeMenuItem.height/2.0f ));
    this->lblText->setAnchorPoint(Point(0.5f, 0.5f));
    
    this->addChild(backSprite);
    this->addChild(lblText);
}

Rectton* Rectton::create(std::string text,Color3B color)
{
    Rectton *pRet = new Rectton(text,color);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void Rectton::selected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MenuItem::selected();
        
        Action *action = getActionByTag(kZoomActionTag);
        if (action)
        {
            this->stopAction(action);
        }
        else
        {
            _originalScale = this->getScale();
            _originalPosition = this->getPosition();
        }
        
        auto slideDistance = this->getBoundingBox().size.height/8.0f;
        
        Action *slideAction = MoveBy::create(0.1f, Point(0, -1*slideDistance));
        slideAction->setTag(kSlideActionTag);
        this->runAction(slideAction);
        
        Action *zoomAction = ScaleTo::create(0.1f, _originalScale * 0.95f);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
    }
}

void Rectton::unselected()
{
    // subclass to change the default action
    if(_enabled)
    {
        MenuItem::unselected();
        this->stopActionByTag(kZoomActionTag);
        Action *zoomAction = ScaleTo::create(0.1f, _originalScale);
        zoomAction->setTag(kZoomActionTag);
        this->runAction(zoomAction);
        
        Action *slideActionReverse = MoveTo::create(0.05f, _originalPosition);
        this->runAction(slideActionReverse);
    }
}


