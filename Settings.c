#include "./Settings.h"



inline void enableTracing(){
    uint32_t enabled = {1};
    WriteProcessMemory(process.handle,(LPVOID)TRACELINE_NEEDED_CUSTOM,(LPVOID)&enabled,4,NULL);
    setting.isTracingActive = enabled;
}

inline void disableTracing(){
    uint32_t enabled = {0};
    WriteProcessMemory(process.handle,(LPVOID)TRACELINE_NEEDED_CUSTOM,(LPVOID)&enabled,4,NULL);
    setting.isTracingActive = enabled;
}