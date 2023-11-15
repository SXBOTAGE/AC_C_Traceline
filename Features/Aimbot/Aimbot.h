#ifndef AIMBOT_H_INCLUDED
#define AIMBOT_H_INCLUDED

#include "../../Game.h"


extern uint32_t getEntitiesVisibilityValue();
extern bool isEntityVisible(size_t _position);
extern bool isValidAimedEntity(uint8_t _index);
extern bool isBadWeapon();
extern int8_t getClosestTargetFromPosition();
extern int8_t getClosestTargetFromCrosshair();
extern void aimbot();
extern void updateViewAngles(Vector2 _angle);
extern void updateYaw(float _yaw);
extern void updatePitch(float _pitch);
extern float getAimspeedScaling();
extern Vector2 relativePrecision(float _distance);



#endif