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


enum tiledIndex {tmxEmpty,tmxPlatform,tmxFire,tmxFemale,tmxMale,tmxWin,tmxBlock};
enum playerIndex {pUnknown,pFemale,pMale};
enum sensorIndex {sensorNone = 100,sensorTop = 101,sensorRight = 102,sensorBottom = 103,sensorLeft = 104};
enum directionIndex {directionLeft,directionRight};
const float kPixelsPerMeter = 32.0f;
const float kGravity = -kPixelsPerMeter / 0.5; // adjust this to taste
const float kUpdateInterval = 1.0f / 60.0f;
const double kSecondsPerUpdate = 0.1;
struct userdataFormat { int a;int b;int c;cocos2d::Node* node;  userdataFormat(int a,int b = 0,int c = 0,cocos2d::Node* node = nullptr){this->a = a;this->b = b;this->c = c;this->node = node;}};
#endif
