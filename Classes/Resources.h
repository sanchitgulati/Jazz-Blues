//
//  Resources.h
//  jazz
//
//  Created by Sanchit Gulati on 13/08/14.
//
//

#ifndef jazz_Resources_h
#define jazz_Resources_h

#define PI 3.14
#define DEGTORAD 180/PI

#define ARC4RANDOM_MAX      0x100000000
#define SG_RAND_0_1 ((double)arc4random() / ARC4RANDOM_MAX);


#define IMG_BG "images/bg.png"
#define IMG_FEMALE "images/girl.png"
#define IMG_MALE "images/boy.png"
#define IMG_PLATFORM "images/platform.png"
#define IMG_SPIKE "images/fire.png"
#define IMG_WIN "images/win.png"
#define IMG_BLOCK "images/block.png"

#define IMG_BUTTON_LEFT_0 "images/button_left_0.png"
#define IMG_BUTTON_LEFT_1 "images/button_left_1.png"
#define IMG_BUTTON_RIGHT_0 "images/button_right_0.png"
#define IMG_BUTTON_RIGHT_1 "images/button_right_1.png"
#define IMG_BUTTON_UP_0 "images/button_top_0.png"
#define IMG_BUTTON_UP_1 "images/button_top_1.png"

#define IMG_LEVEL_SELECTOR_0 "images/level_selector_0.png"
#define IMG_LEVEL_SELECTOR_1 "images/level_selector_1.png"

#define IMG_REFRESH_0 "images/refresh_0.png"
#define IMG_REFRESH_1 "images/refresh_1.png"

#define IMG_MUTE_0 "images/mute_0.png"
#define IMG_MUTE_1 "images/mute_1.png"

#define IMG_UNMUTE_0 "images/unmute_0.png"
#define IMG_UNMUTE_1 "images/unmute_1.png"

#define FONT "fonts/MavenPro-Medium.ttf"
#define FONT_BOLD "fonts/MavenPro-Black.ttf"

#define SFX_BG_INTRO "sfx/at_the_window_intro.mp3"
#define SFX_BG_HAPPY "sfx/at_the_window_happy.mp3"

#define IMG_JAZZ "images/jazz_01.png"
#define IMG_GAME_LOGO "images/game_logo.png"

#define RGB_BLACK Color3B(20,20,20)
#define RGB_WHITE Color3B(249,249,249)
#define RGB_ROSE Color3B(120,25,61)

#define LVL_A 9
#define LVL_B 10

#define VFX_CONSTANT 0.75

enum gameMenuTag {menuLevel,menuMute,menuUnmute,menuRefresh,menuToggle};
enum zButtonIndex {bLeft,bRight,bUp};
enum zIndex {zBackground,zGame,zForeground,zHUD,zControl};
enum tiledIndex {tmxEmpty,tmxPlatform,tmxFire,tmxFemale,tmxMale,tmxWin,tmxBlock,tmxWall,tmxPoison};
enum playerIndex {pUnknown,pFemale,pMale};
enum sensorIndex {sensorNone = 100,sensorTop = 101,sensorRight = 102,sensorBottom = 103,sensorLeft = 104};
enum directionIndex {directionLeft,directionRight};
const float kPixelsPerMeter = 32.0f;
const float kGravity = -kPixelsPerMeter / 0.5; // adjust this to taste
const float kUpdateInterval = 1.0f / 60.0f;
const double kSecondsPerUpdate = 0.1;
struct userdataFormat { int a;int b;int c;cocos2d::Node* node;  userdataFormat(int a,int b = 0,int c = 0,cocos2d::Node* node = nullptr){this->a = a;this->b = b;this->c = c;this->node = node;}};


const std::string sideA[LVL_A] = {
    "We were the world",
    "Almighty",
    "Superman",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text"
};
const std::string sideB[LVL_B] = {
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text",
    "Sample Text"
};

#endif
