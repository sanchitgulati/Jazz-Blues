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
#define IMG_BODY "images/body.png"
#define IMG_MALE "images/boy.png"
#define IMG_PLATFORM "images/platform.png"
#define IMG_WALL "images/wall.png"
#define IMG_POLICE "images/police.png"
#define IMG_SOCIETY "images/society.png"
#define IMG_TUNNEL "images/platform.png"
#define IMG_SPIKE "images/fire.png"
#define IMG_WIN "images/win.png"
#define IMG_WIN_ALT "images/booth.png"
#define IMG_BLOCK "images/block.png"
#define IMG_TEDDY "images/teddy.png"
#define IMG_TEMPLE "images/temple.png"
#define IMG_TEMPLE_01 "images/temple01.png"
#define IMG_DOOR "images/door.png"
#define IMG_CLOUD "images/cloud.png"
#define IMG_CLOUD_0 "images/cloud01.png"
#define IMG_CLOUD_1 "images/cloud02.png"
#define IMG_CLOUD_2 "images/cloud03.png"

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

#define IMG_POISON "images/poison.png"
#define IMG_POISON_ALT "images/poisonalt.png"

#define IMG_LAUGH "images/laugh.png"

#define FONT "fonts/travel.ttf"
#define FONT_JANE "fonts/hand.ttf"
#define BMP_FONT "fonts/travel.fnt"

#define SFX_BG_INTRO "sfx/Danse_Morialta_Intro.mp3"
#define SFX_BG_HAPPY "sfx/Danse_Morialta_Happy.mp3"


#define SFX_JUMP "sfx/jump_0.mp3"
#define SFX_OUCH "sfx/ouch.wav"
#define SFX_WIN "sfx/kiss.wav"
#define SFX_KEY "sfx/collect_0.mp3"
#define SFX_TYPE01 "sfx/type01.wav"
#define SFX_TYPE02 "sfx/type02.wav"
#define SFX_TYPE_END "sfx/type03.wav"
#define SFX_HAHA "sfx/haha.wav"
#define SFX_THUNDER "sfx/lightning_bolt.wav"
#define SFX_POLICE "sfx/police.wav"
#define SFX_LOCK "sfx/lock.wav"
#define SFX_GRIND "sfx/grind.wav"
#define SFX_NO "sfx/no.wav"

#define IMG_JAZZ_0 "images/guitar_0.png"
#define IMG_JAZZ_1 "images/guitar_1.png"
#define IMG_JAZZ_2 "images/guitar_2.png"
#define IMG_GAME_LOGO_0 "images/game_logo_0.png"
#define IMG_GAME_LOGO_1 "images/game_logo_1.png"

#define BAND_01 "images/refuge.jpg"

#define IMG_RECORD_D "images/recordD.png"
#define IMG_RECORD "images/record.png"

//words
#define FNT_BRACKET "text/bracket.png"
#define FNT_ONCE "text/once.png"
#define FNT_NOTSO "text/notso.png"

#define RGB_BLACK Color3B(20,20,20)
#define RGB_WHITE Color3B(252,252,252)
#define RGB_ROSE Color3B(229,11,26)
#define RGB_BLUE Color3B(0,243,236)
#define RGB_PLATFORM Color3B(50,50,50)


#define RGB_BOY Color3B(0,86,185)
#define RGB_GIRL Color3B(171,71,203)

#define RGB_SKIN Color3B(250,215,129)

#define MAX_SFX 0.50
#define MAX_BG 0.90

#define LVLS 12
#define TRACKS 6
#define WIN_QOUTES 7
#define LOSE_QOUTES 7

#define VFX_CONSTANT 0.75

enum gameMenuTag {menuLevel,menuMute,menuUnmute,menuRefresh,menuToggle,menuSkip};
enum zButtonIndex {bLeft,bRight,bUp};
enum zIndex {zBackground,zGame,zNight,zRain,zForeground,zHUD,zControl};
enum tiledIndex {tmxEmpty,tmxPlatform,tmxFire,tmxFemale,tmxMale,tmxWin,tmxBlock,tmxWall,tmxCloud,tmxTemple,tmxDoor,tmxPoison,tmxTeddy,tmxTunnel,tmxSociety,tmxPolice};
enum playerIndex {pUnknown,pFemale,pMale};
enum sensorIndex {sensorNone = 100,sensorTop = 101,sensorRight = 102,sensorBottom = 103,sensorLeft = 104};
enum directionIndex {directionLeft,directionRight};
const float kPixelsPerMeter = 32.0f;
const float kGravity = -kPixelsPerMeter / 0.5; // adjust this to taste
const float kUpdateInterval = 1.0f / 60.0f;
const double kSecondsPerUpdate = 0.1;
struct userdataFormat { int a;int b;int c;cocos2d::Node* node;  userdataFormat(int a,int b = 0,int c = 0,cocos2d::Node* node = nullptr){this->a = a;this->b = b;this->c = c;this->node = node;}};



const std::string level[LVLS] = {
    "I. Together",
    "II. Superman",
    "III. Present",
    "IV. Poison",
    "V. Tunnels",
    "VI. Downside",
    "VII. Reason",
    "VIII. Night",
    "IX. Fire",
    "X. Police",
    "XI. Finale",
    "XII. End",
};

const std::string tracks[TRACKS] = {
    "Maiden Voyage",
    "Connections",
    "Not So Blue",
    "Movie Night",
    "In a turn of events",
    "Estoy"
};


const std::string win[WIN_QOUTES] = {
    "And in her smile I see something more beautiful than the stars",
    "And I’ll never love you any less than I do, right this second",
    "Sometimes I can’t see myself when I’m with you. I can only just see you",
    "I am catastrophically in love with you",
    "I love you. Remember. They cannot take it",
    "Love isn’t all we need — love is all there is",
    "Take my hand and we'll make it"
};

const std::string lose[LOSE_QOUTES] = {
    "I'd do it all over again, if I had the choice. No regrets",
    "I can't live, with or without you",
    "I know the sun will swallow the only earth we'll ever have, and I am in love with you",
    "I dont care how hard being together is, nothing is worse than being apart",
    "But you ve slipped under my skin, invaded my blood and seized my heart",
    "Remember to let her into your heart",
    "I'd trade all my tomorrows for one single yesterday"
};

#endif
