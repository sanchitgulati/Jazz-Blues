#include "AppDelegate.h"
#include "LogoSplash.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        //glview = GLView::createWithFullScreen("badazz");
        glview = GLView::create("badazz");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sfx/jump_0.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sfx/bg.mp3");
//    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sfx/bg.mp3",true);
    

    // create a scene. it's an autorelease object
//    auto scene = (Scene*)GameScene::create();
    auto scene = (Scene*)LogoSplash::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
