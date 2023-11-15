#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED
#include "./Game.h"

typedef struct Setting                   Setting;

enum {FROM_POSITION, FROM_CROSSHAIR};
enum {LEGS, BODY, NECK, HEAD};
enum {FIXED_AIMSPEED, PROGRESSIVE_AIMSPEED};
enum {RADARE_ALL, RADARE_ENNEMY, RADARE_ALLY};


struct Setting
{
    bool enableAimbot;
    bool  isTracingActive;
    bool  tracingEnabled;
    bool  focusMate;
    bool  aimbotFocusMode;
    bool  dimensionUpdateRequest;
    bool espVisibilityCheck;
    bool espShowAlly;
    bool espShowHealth;
    bool espShowArmor;
    bool espShowWeapon;
    bool enableTriggerbot;
    bool trigAlly;
    bool enableWallhack;

    float radius;
    float aimInPrecisionX;
    float aimInPrecisionY;
    float aimTime;
    float trigDelay;
    
    uint8_t radareShowType;

}setting;

extern void enableTracing();
extern void disableTracing();


#endif