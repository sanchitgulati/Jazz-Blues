#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::LayerColor
{
    cocos2d::Node* _table;
    cocos2d::Node* _prop;
    cocos2d::Menu* _menu;
    cocos2d::Node* _gameLogo;
    
    void createLevelMenu();
public:
	MainMenu();
	~MainMenu();
    virtual bool init();
    void onEnter();
    void onExit();
    
    void update(float delta);
    void transitionToGameScene();
    void toGameScene();
    
    // a selector callback
    void menuCallback(cocos2d::Ref* pSender);
    void levelCallback(cocos2d::Ref* pSender);
    
    
    CREATE_FUNC(MainMenu);
};

#endif