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

#define FONT "fonts/Lato.ttf"


enum zButtonIndex {bLeft,bRight,bUp};
enum zIndex {zBackground,zGame,zForeground,zHUD,zControl};
enum tiledIndex {tmxEmpty,tmxPlatform,tmxFire,tmxFemale,tmxMale,tmxWin,tmxBlock};
enum playerIndex {pUnknown,pFemale,pMale};
enum sensorIndex {sensorNone = 100,sensorTop = 101,sensorRight = 102,sensorBottom = 103,sensorLeft = 104};
enum directionIndex {directionLeft,directionRight};
const float kPixelsPerMeter = 32.0f;
const float kGravity = -kPixelsPerMeter / 0.5; // adjust this to taste
const float kUpdateInterval = 1.0f / 60.0f;
const double kSecondsPerUpdate = 0.1;
struct userdataFormat { int a;int b;int c;cocos2d::Node* node;  userdataFormat(int a,int b = 0,int c = 0,cocos2d::Node* node = nullptr){this->a = a;this->b = b;this->c = c;this->node = node;}};

static int kCurrentLevel = 1;
#endif
