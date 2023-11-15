#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../include/LibMemory.h"
#include "../include/LibMath.h"
#include "Features/Aimbot/Aimbot.h"
#include "Features/Triggerbot/Triggerbot.h"
#include "Features/Radarehack/Radarehack.h"
#include "Features/Wallhack/Wallhack.h"
#include "Features/ESP/esp.h"
#include "./Settings.h"


// ASLR not supported inside the PE header of ac_client.exe 
#define AC_CLIENT_BASE_ADDRESS 0x400000
#define AC_CLIENT_OFFSET(offset)  (AC_CLIENT_BASE_ADDRESS + offset)
// Static offsets variables
#define LOCAL_PLAYER AC_CLIENT_OFFSET(0x10F4F4)
#define ENTITIES_VISIBILITY_CUSTOM AC_CLIENT_OFFSET(0x10F504)
#define TRACELINE_NEEDED_CUSTOM AC_CLIENT_OFFSET(0x10F508)
#define PLAYER_NBR AC_CLIENT_OFFSET(0x10F500)
#define ENTITY_LIST AC_CLIENT_OFFSET(0x10F4F8)
#define MODEL_VIEW_PROJECTION_MATRIX AC_CLIENT_OFFSET(0x101AE8)
#define WINDOW_WIDTH AC_CLIENT_OFFSET(0x110C94)
#define WINDOW_HEIGHT AC_CLIENT_OFFSET(0x110C98)
#define TRIGNAME	AC_CLIENT_OFFSET(0x101C38)
// Static offsets functions
#define TRACELINE AC_CLIENT_OFFSET(0x08a310)
// .text section
#define RADARE_COMP AC_CLIENT_OFFSET(0x9FB3)
#define WALLHACK_INJ AC_CLIENT_OFFSET(0x1530E)
// Other definition
#define BAD_INDEX -1
#define PLAYER_HEIGHT 4.50f
#define HEAD_POSTION(z) (z + 0.50f)
#define BODY_POSTION(z) (z - 1.00f)
#define LEGS_POSTION(z) (z - 3.00f)
#define FEET_POSTION(z) (z - 4.50f)
#define NECK_POSITION(z) (z)


// ---------------- Structures ----------------- //


typedef struct TraceResult                   TraceResult;
typedef struct PlayerEntity                  PlayerEntity;
typedef struct Weapon                 	 	 Weapon;
typedef struct EntityList					 EntityList;



struct __attribute__((__packed__)) TraceResult {
	Vector3 vec;
	bool collided;
};

struct __attribute__((__packed__)) PlayerEntity
{
	void* vptr; //0x0000
	Vector3 position; //0x0004
	Vector3 velocity; //0x0010
	char pad_001C[36]; //0x001C
	Vector3 viewAngles; //0x0040
	char pad_004C[4]; //0x004C
	float maxSpeed; //0x0050
	float timeInAir; //0x0054
	float radius; //0x0058
	float eyeHeight; //0x005C
	float maxEyeHeight; //0x0060
	float aboveeye; //0x0064
	bool inWater; //0x0068
	bool isOnFloor; //0x0069
	bool onLadder; //0x006A
	bool jumpnext; //0x006B
	bool isCroushing; //0x006C
	bool N0000046E; //0x006D
	bool N00000171; //0x006E
	bool N0000046F; //0x006F
	bool isStuck; //0x0070
	bool isScoping; //0x0071
	int32_t lastJump; //0x0072
	char pad_0076[10]; //0x0076
	int8_t forceUpOrDown; //0x0080
	int8_t forceLeftOrRight; //0x0081
	uint8_t playerState; //0x0082
	char pad_0083[117]; //0x0083
	int32_t health; //0x00F8
	uint32_t armor; //0x00FC
	uint32_t primary; //0x0100
	uint32_t nextPrimary; //0x0104
	uint32_t gunSelect; //0x0108
	bool unlockAkimbo; //0x010C
	char ammo[10][4]; //0x010D
	char mag[10][4]; //0x0135
	uint32_t WeaponsCoolddown[10]; //0x015D
	char WeaponsShotsStat[10][4]; //0x0185
	char WeaponsDamageStat[10][4]; //0x01AD
	char pad_01D5[79]; //0x01D5
	bool shoot; //0x0224
	char name[15]; //0x0225
	char pad_0234[248]; //0x0234
	bool team; //0x032C
	char pad_032D[27]; //0x032D
	char AllWeaponsPtr[10][4]; //0x0348
	Weapon *lastWeapon; //0x0370
	Weapon *currentWeapon; //0x0374
}localPlayer,*localPlayerPtr,*entities; //Size: 0x0378

struct __attribute__((__packed__)) Weapon
{
	void *vptr; //0x0000
	uint32_t index; //0x0004
	PlayerEntity *player; //0x0008
	char *Name; //0x000C
	uint32_t *ammoStock; //0x0010
	uint32_t *ammo; //0x0014
	bool *shoot; //0x0018
	uint32_t rifleCounting; //0x001C
	uint32_t isReloading; //0x0020
	char pad_0024[4]; //0x0024
	bool scoping; //0x0028
	char pad_0029[7]; //0x0029
}; //Size: 0x0030

struct __attribute__((__packed__)) EntityList
{
	char pad_0000[4]; //0x0000
	PlayerEntity *entity[31]; //0x0004
}*entityList; //Size: 0x0080

// -- ENUM --- // 
enum { GUN_KNIFE = 0, GUN_PISTOL, GUN_CARBINE, GUN_SHOTGUN, GUN_SUBGUN, GUN_SNIPER, GUN_ASSAULT, GUN_CPISTOL, GUN_GRENADE, GUN_AKIMBO, NUMGUNS };
enum { CLA = 0, RVSF = 1 };
enum { ALIVE, DEAD, DEBUG = 4, GHOST };

// -- Functions -- //

extern void resolveLocalPlayerPtr();
extern bool resolveEntityListPtr();
extern void updateLocalPlayerInformation();
extern void updateEntitiesInformation();
extern void writeTracelineWrapper();
extern void updateScreenResolution();
extern uint32_t getEntityWeaponIndex(uint8_t _entityIndex);
extern uint32_t getLocalPlayerWeaponIndex();
extern bool worldToScreen(Vector3 _pos, Vector2 *out);

// -- Variables -- //

extern const char* gunsName[NUMGUNS];
extern uintptr_t localPlayerAddr;
extern uintptr_t EntityListAddr;
extern uint32_t entitiesVisibilityValue;
extern uint8_t entityNbr;
extern uint8_t currentEntityNbrArray;
extern BYTE tracelineWrapper[164];
extern ScreenDimension screen;
LARGE_INTEGER frequency, start, end;
extern bool EntityListIni;


#endif