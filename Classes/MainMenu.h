#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "FmodHelper.h"

class MainMenu : public cocos2d::LayerColor
{
    cocos2d::Node* _table;
    cocos2d::Node* _player;
    cocos2d::Node* _prop;
    cocos2d::Menu* _menu;
    cocos2d::Node* _gameLogo;
    FmodHelper* _fmod;
    cocos2d::Vector<cocos2d::Node*> _tracksTTF;
    
    void createLevelMenu();
    void hideButtons(bool guitar = false);
    void showButtons();
    void createSoundPlayer();
    void backToMenu();
public:
    static cocos2d::Scene* createScene();
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
    void soundCallback(cocos2d::Ref* pSender);
    
    
    CREATE_FUNC(MainMenu);
};

#endif