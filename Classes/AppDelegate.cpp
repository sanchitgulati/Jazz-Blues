#include "AppDelegate.h"
#include "LogoSplash.h"
#include "Resources.h"
#include "Util.h"


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
        glview = GLView::createWithFullScreen("Jazz and Blues");
//        glview = GLView::create("Jazz and Blues");
        director->setOpenGLView(glview);
    }
    srand(time(NULL));
    
    // turn on display FPS
    director->setDisplayStats(false);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = Scene::create();
    auto layer = LogoSplash::create();
    scene->addChild(layer);
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    

    // if you use SimpleAudioEngine, it must be pause
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
}
