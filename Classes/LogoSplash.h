#ifndef __LOGOSPLASH_SCENE_H__
#define __LOGOSPLASH_SCENE_H__

#include "cocos2d.h"
#include "FmodHelper.h";

class LogoSplash : public cocos2d::LayerColor
{
public:
	LogoSplash();
	~LogoSplash();
    virtual bool init();
    void onEnter();
    void onExit();
    
    void nextScene(Node *);
    void update(float delta);
    
    
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
    
    cocos2d::Sprite *logoSprite;
    FmodHelper* _fmod;
    
    CREATE_FUNC(LogoSplash);
};

#endif