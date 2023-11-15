
#include "Radarehack.h"

BYTE radareOriginB[2] = {0x0F, 0x85};
BYTE radareShowAllB[2] = {0x0F, 0x8B};
BYTE radareShowEnnemyB[2] = {0x0F, 0x83};

inline void radareLogic()
{

    switch (setting.radareShowType)
    {
    case RADARE_ALL:
        WriteProcessMemory(process.handle, (LPVOID)RADARE_COMP, (LPCVOID)radareShowAllB, 2, NULL);
        break;
    case RADARE_ENNEMY:
        WriteProcessMemory(process.handle, (LPVOID)RADARE_COMP, (LPCVOID)radareShowEnnemyB, 2, NULL);
        break;
    case RADARE_ALLY:
        WriteProcessMemory(process.handle, (LPVOID)RADARE_COMP, (LPCVOID)radareOriginB, 2, NULL);
        break;
    }

    return;
}
