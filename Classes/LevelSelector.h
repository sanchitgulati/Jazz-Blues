#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"

class LevelSelector : public cocos2d::LayerColor
{
public:
	LevelSelector();
	~LevelSelector();
    virtual bool init();
    void onEnter();
    void onExit();
    
    void update(float delta);
    
    // a selector callback
    void menuCallback(cocos2d::Ref* pSender);
    
    cocos2d::Label* _gameLabel;
    
    CREATE_FUNC(LevelSelector);
};

#endif