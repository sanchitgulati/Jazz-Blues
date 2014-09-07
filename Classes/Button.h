//
//  Button.h
//  jazzblues
//
//  Created by Sanchit Gulati on 06/09/14.
//
//

#ifndef __jazzblues__Button__
#define __jazzblues__Button__

#include "cocos2d.h"

class Button : public cocos2d::Node {
	cocos2d::Sprite *_normalSprite, *_selectedSprite;
	Button(const char *normalSprite, const char *selectedSprite);
public:
	bool isPressed;
	static Button* create(const char *normalSprite, const char *selectedSprite);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    static int identifier;
};

#endif /* defined(__jazzblues__Button__) */
