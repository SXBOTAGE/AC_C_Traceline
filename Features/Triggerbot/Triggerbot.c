#include "Triggerbot.h"

void triggerbot()
{

    if (!setting.enableTriggerbot)
        return;

    char entityName[15];

    getTrigName(entityName);

    int8_t i = getEntityByName(entityName);

    if (i == BAD_INDEX || !trigValidEntity(i) || isBadWeapon())
        return;

    if (setting.trigDelay > 0)
        Sleep(setting.trigDelay);

    makePlayerShoot();
    clearName();
}

inline uint8_t getEntityByName(const char _name[15])
{

    for (uint8_t i = 0; i < entityNbr - 1; i++)
    {
        if (!strcmp(entities[i].name, _name))
            return i;
    }

    return BAD_INDEX;
}

inline void clearName()
{
    const char c[15] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    WriteProcessMemory(process.handle, (LPVOID)TRIGNAME, (LPVOID)&c, 15, NULL);
    return;
}

inline void getTrigName(char _buffer[15])
{
    ReadProcessMemory(process.handle, (LPVOID)TRIGNAME, (LPVOID)_buffer, 15, NULL);
    return;
}

inline bool trigValidEntity(uint8_t _i)
{

    if (!setting.trigAlly && entities[_i].team == localPlayer.team)
        return false;

    return true;
}

inline void makePlayerShoot()
{
    bool shoot = true;
    WriteProcessMemory(process.handle, (LPVOID)&localPlayerPtr->shoot, (LPVOID)&shoot, 1, NULL);
}