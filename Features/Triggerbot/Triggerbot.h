#ifndef TRIGGERBOT_H_INCLUDED
#define TRIGGERBOT_H_INCLUDED

#include "../../Game.h"

extern void triggerbot();
extern void clearName();
extern void getTrigName(char _buffer[15]);
extern void makePlayerShoot();
extern uint8_t getEntityByName(const char _name[15]);
extern bool trigValidEntity(uint8_t _i);



#endif