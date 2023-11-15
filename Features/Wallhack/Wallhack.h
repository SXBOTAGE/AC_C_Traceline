#ifndef WALLHACK_H_INCLUDED
#define WALLHACK_H_INCLUDED

#include "../../Game.h"

/* 
Call Gl_DepthFunc with a value of GL_ALWAYS  just before entity are drawn. Making all entity visible behind walls.

When glDepthFunc(GL_ALWAYS) is set, all fragments from the entity (player, for instance) will pass the depth test, regardless of their depth values. This means they will be
drawn in front of or on top of any existing geometry on the screen, regardless of their actual position in 3D space relative to the Z buffer.

I selected the emplacement where all entities are iterated in a loop : for internal cheat you can find the entity that will be draw in the ebx register.
*/

/* original 
ac_client.exe.text+1430E - 75 24                 - jne loc
ac_client.exe.text+14310 - 8B 0D F4F45000        - mov ecx,[ac_client.exe+10F4F4]
ac_client.exe.text+14316 - 8B 91 2C030000        - mov edx,[ecx+0000032C]
ac_client.exe.text+1431C - 3B 97 2C030000        - cmp edx,[edi+0000032C]

*/

/* Patching 

ac_client.exe.text+1430E - 90                    - nop 
ac_client.exe.text+1430F - 90                    - nop 
ac_client.exe.text+14310 - 68 07020000           - push 00000207
ac_client.exe.text+14315 - FF 15 14A24D00        - call dword ptr [ac_client.exe.rdata+214]
ac_client.exe.text+1431B - 90                    - nop 


*/



extern BYTE wallOriginB[20];
extern BYTE wallAllB[14];
extern void wallhackLogic();


#endif