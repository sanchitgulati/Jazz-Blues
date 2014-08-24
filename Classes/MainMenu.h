#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::LayerColor
{
public:
	MainMenu();
	~MainMenu();
    virtual bool init();
    void onEnter();
    void onExit();
    
    void update(float delta);
    
    // a selector callback
    void menuCallback(cocos2d::Ref* pSender);
    
    cocos2d::Label* _gameLabel;
    
    CREATE_FUNC(MainMenu);
};

#endif