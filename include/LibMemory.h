#ifndef LIBMEMORY_H_INCLUDED
#define LIBMEMORY_H_INCLUDED

#include <Windows.h> // Win API
#include <stdint.h> // Library types
#include <tlhelp32.h> // Tool Help Library

// I tried to do it in C++ style just for curiosity but ...


typedef uint8_t                              byte_t;
typedef uint8_t                              bool;
typedef uint16_t                             word_t;
typedef uint32_t                             dword_t;
typedef uint64_t                             qword_t;

typedef DWORD                                pid_t;

typedef struct Process                       Process;

#define MEM_BAD                -1
#define MEM_GOOD               !MEM_BAD
#define MEM_NULL               0
#define MEM_FALSE              false
#define MEM_TRUE               true

// -------------- Process Utility ------------ //
void initProcess(Process* this);
struct Process {
    pid_t pid;
    HANDLE handle;
    const char* name;
    LPVOID allocAddr;    
}process;
bool getPIDByName(Process* this,const char * _processName);
bool getProcessHandle(Process* this);
bool getProcessName(Process *this,pid_t _pid);
LPVOID allocateMemory(Process *this);
void releaseProcess(Process *this);

bool writeShellcode(Process *this,size_t _len,BYTE _shellcodes[]);
void executeShellcode(Process *this);
// -------------- Patch Utility ------------ // 





#endif
