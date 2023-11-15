#ifndef RADAREHACK_H_INCLUDED
#define RADAREHACK_H_INCLUDED

#include "../../Game.h"

/* 
In function that display ally on the radare is only active on "TEAM" mode.
So to keep it simple this feature will show ennemy on the radare only in team mode
*/

extern BYTE radareOriginB[2];
extern BYTE radareShowAllB[2];
extern BYTE radareShowEnnemyB[2];


extern void radareLogic();
extern void disableRadare();

#endif