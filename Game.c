
#include "Game.h"
#include <stdio.h>

/* Traceline wrapper in complete assembly
----
00A00000 - F7 05 08F55000 01000000 - test [ac_client.exe+10F508],00000001 { (1),1 }
00A0000A - 0F84 84000000         - je 00A00094
00A00010 - 8B 3D 00F55000        - mov edi,[ac_client.exe+10F500] { (00000004) }
00A00016 - 83 FF 02              - cmp edi,02 { 2 }
00A00019 - 7C 79                 - jl 00A00094
00A0001B - 0F1F 40 00            - nop dword ptr [eax+00]
00A0001F - 8B 15 F4F45000        - mov edx,[ac_client.exe+10F4F4] { (00F0A668) }
00A00025 - 8B 1D F8F45000        - mov ebx,[ac_client.exe+10F4F8] { (0D0F6BB0) }
00A0002B - 4F                    - dec edi
00A0002C - 8B 34 BB              - mov esi,[ebx+edi*4]
00A0002F - 83 EC 10              - sub esp,10 { 16 }
00A00032 - 8D 04 24              - lea eax,[esp]
00A00035 - 6A 00                 - push 00 { 0 }
00A00037 - 6A 00                 - push 00 { 0 }
00A00039 - 52                    - push edx
00A0003A - FF 76 0C              - push [esi+0C]
00A0003D - FF 76 08              - push [esi+08]
00A00040 - FF 76 04              - push [esi+04]
00A00043 - FF 72 0C              - push [edx+0C]
00A00046 - FF 72 08              - push [edx+08]
00A00049 - FF 72 04              - push [edx+04]
00A0004C - B9 10A34800           - mov ecx,ac_client.exe.text+89310 { (136596619) }
00A00051 - FF D1                 - call ecx
00A00053 - 8B 44 24 30           - mov eax,[esp+30]
00A00057 - 83 E0 01              - and eax,01 { 1 }
00A0005A - 83 C4 34              - add esp,34 { 52 }
00A0005D - 8B CF                 - mov ecx,edi
00A0005F - 49                    - dec ecx
00A00060 - 8B 1D 04F55000        - mov ebx,[ac_client.exe+10F504] { (-5) }
00A00066 - D3 EB                 - shr ebx,cl
00A00068 - 38 D8                 - cmp al,bl
00A0006A - 74 1E                 - je 00A0008A
00A0006C - BB 01000000           - mov ebx,00000001 { 1 }
00A00071 - D3 E3                 - shl ebx,cl
00A00073 - 83 F8 01              - cmp eax,01 { 1 }
00A00076 - 75 0F                 - jne 00A00087
00A00078 - 0F1F 40 00            - nop dword ptr [eax+00]
00A0007C - 09 1D 04F55000        - or [ac_client.exe+10F504],ebx { (-5) }
00A00082 - EB 0B                 - jmp 00A0008F
00A00084 - 0F1F 00               - nop dword ptr [eax]
00A00087 - F7 D3                 - not ebx
00A00089 - 21 1D 04F55000        - and [ac_client.exe+10F504],ebx { (-5) }
00A0008F - 83 FF 01              - cmp edi,01 { 1 }
00A00092 - 7F 8B                 - jg 00A0001F
00A00094 - 68 50000000           - push 00000050 { 80 }
00A00099 - FF 15 5CA14D00        - call dword ptr [ac_client.exe.rdata+15C] { ->->KERNELBASE.Sleep }
00A0009F - E9 5CFFFFFF           - jmp 00A00000
*/

uintptr_t localPlayerAddr = MEM_BAD;
uintptr_t EntityListAddr = MEM_BAD;
bool EntityListIni = FALSE;
uint8_t entityNbr = 0;
uint8_t currentEntityNbrArray = 0;
uint32_t entitiesVisibilityValue = 0;
BYTE tracelineWrapper[164] = {0xF7, 0x05, 0x08, 0xF5, 0x50, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x84, 0x84, 0x00, 0x00, 0x00, 0x8B, 0x3D, 0x00, 0xF5, 0x50, 0x00, 0x83, 0xFF, 0x02, 0x7C, 0x79, 0x0F, 0x1F, 0x40, 0x00, 0x8B, 0x15, 0xF4, 0xF4, 0x50, 0x00, 0x8B, 0x1D, 0xF8, 0xF4, 0x50, 0x00, 0x4F, 0x8B, 0x34, 0xBB, 0x83, 0xEC, 0x10, 0x8D, 0x04, 0x24, 0x6A, 0x00, 0x6A, 0x00, 0x52, 0xFF, 0x76, 0x0C, 0xFF, 0x76, 0x08, 0xFF, 0x76, 0x04, 0xFF, 0x72, 0x0C, 0xFF, 0x72, 0x08, 0xFF, 0x72, 0x04, 0xB9, 0x10, 0xA3, 0x48, 0x00, 0xFF, 0xD1, 0x8B, 0x44, 0x24, 0x30, 0x83, 0xE0, 0x01, 0x83, 0xC4, 0x34, 0x8B, 0xCF, 0x49, 0x8B, 0x1D, 0x04, 0xF5, 0x50, 0x00, 0xD3, 0xEB, 0x38, 0xD8, 0x74, 0x1E, 0xBB, 0x01, 0x00, 0x00, 0x00, 0xD3, 0xE3, 0x83, 0xF8, 0x01, 0x75, 0x0F, 0x0F, 0x1F, 0x40, 0x00, 0x09, 0x1D, 0x04, 0xF5, 0x50, 0x00, 0xEB, 0x0B, 0x0F, 0x1F, 0x00, 0xF7, 0xD3, 0x21, 0x1D, 0x04, 0xF5, 0x50, 0x00, 0x83, 0xFF, 0x01, 0x7F, 0x8B, 0x68, 0x50, 0x00, 0x00, 0x00, 0xFF, 0x15, 0x5C, 0xA1, 0x4D, 0x00, 0xE9, 0x5C, 0xFF, 0xFF, 0xFF};
ScreenDimension screen = {0, 0};
const char *gunsName[NUMGUNS] = {"Knife", "Pistol", "Carbine", "Shotgun", "Subgun", "Sniper", "Assault", "Cpistol", "Greande", "AKIMBO"};

// The player address is static and will not change, resolve it one time at game initilization and it will work perfectly
void resolveLocalPlayerPtr()
{
    ReadProcessMemory(process.handle, (LPCVOID)LOCAL_PLAYER, &localPlayerAddr, 4, NULL);
    localPlayerPtr = (PlayerEntity *)localPlayerAddr;
}

// The entity list address will be initilized when the user start a game mode, it can't be resolved before. When resolved it's static.
bool resolveEntityListPtr()
{
    if (EntityListIni == TRUE)
        return FALSE;

    // Check nbr of players, should be at least 1
    ReadProcessMemory(process.handle, (LPCVOID)PLAYER_NBR, &entityNbr, 4, NULL);

    if (entityNbr > 0 && entityNbr < 33)
    {
        ReadProcessMemory(process.handle, (LPCVOID)ENTITY_LIST, &EntityListAddr, 4, NULL);
        EntityListIni = TRUE;
        entityList = (EntityList *)EntityListAddr; // set the entityList structure with the correct address
        return TRUE;
    }

    return FALSE;
}

// Update local player information with 1 readprocmemory() reducing context switching between kernel mode and user-mode
// The localplayer structure is probably created inside the .bss segment and values are updated directly with this call
void updateLocalPlayerInformation()
{
    ReadProcessMemory(process.handle, (LPCVOID)localPlayerAddr, &localPlayer, sizeof(PlayerEntity), NULL);
}

// Create needed structures on the process heap to host entities informations
void updateEntitiesInformation()
{

    if (!EntityListIni)
        if (!resolveEntityListPtr())
            return;

    // Check nbr of players, should be at least 2
    ReadProcessMemory(process.handle, (LPCVOID)PLAYER_NBR, &entityNbr, 4, NULL);

    if (entityNbr < 2)
    {
        if (entities != NULL)
            free(entities);
        return;
    }

    // We should Update / Create heap objects to store entity values
    // if we created more heap objects, its ok we will just not use them don't need to rezize, the game may use them later
    if (currentEntityNbrArray < entityNbr)
    {

        if (entities == NULL)
            entities = malloc((entityNbr - 1) * sizeof(PlayerEntity));
        else
        {
            free(entities); // free old obj heap ptr
            entities = malloc((entityNbr - 1) * sizeof(PlayerEntity));
        }
        currentEntityNbrArray = entityNbr;
    }

    uintptr_t entityAddr = 0;

    for (int i = 0; i < entityNbr - 1; i++)
    {
        ReadProcessMemory(process.handle, (LPCVOID)&entityList->entity[i], &entityAddr, 4, NULL);                 // resolve entity addr
        ReadProcessMemory(process.handle, (LPCVOID)entityAddr, (LPVOID)&entities[i], sizeof(PlayerEntity), NULL); // copy entity struct to heap object
    }

    return;
}

void writeTracelineWrapper()
{

    allocateMemory(&process);                                             // allocate new region of 4096 bytes
    writeShellcode(&process, sizeof(tracelineWrapper), tracelineWrapper); // write the shellcode wrapper
    executeShellcode(&process);                                           // execute it with CreateRemoteThread
}

inline void updateScreenResolution()
{

    if (setting.dimensionUpdateRequest)
        ReadProcessMemory(process.handle, (LPVOID)WINDOW_WIDTH, &screen, 8, NULL);
    setting.dimensionUpdateRequest = false;
}

inline bool worldToScreen(Vector3 _pos, Vector2 *out)
{

    float matrix[4][4];
    ReadProcessMemory(process.handle, (LPVOID)MODEL_VIEW_PROJECTION_MATRIX, matrix, 64, NULL);

    updateScreenResolution();

    return worldToScreenGL(_pos, screen, matrix, out);
}

inline uint32_t getEntityWeaponIndex(uint8_t _entityIndex)
{

    Weapon weapon;

    ReadProcessMemory(process.handle, (LPCVOID)entities[_entityIndex].currentWeapon, &weapon, sizeof(weapon), NULL);

    return weapon.index;
}

inline uint32_t getLocalPlayerWeaponIndex()
{

    Weapon weapon;

    ReadProcessMemory(process.handle, (LPCVOID)localPlayer.currentWeapon, &weapon, sizeof(weapon), NULL);

    return weapon.index;
}
