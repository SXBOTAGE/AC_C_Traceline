#include "Game.h"
#include "../include/LibMath.h"
#include <stdio.h> // intput / output

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        hdc = BeginPaint(overlayHWND, &ps);

        esp();

        EndPaint(overlayHWND, &ps);
    }
    break;
    case WM_DESTROY:
    {

        releaseProcess(&process);
        free(entities); // release all entities allocated on the process heap
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void CALLBACK TimerProc(HWND hWnd, UINT message, UINT_PTR timerId, DWORD time)
{

    InvalidateRect(hWnd, NULL, true);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    initProcess(&process);
    QueryPerformanceFrequency(&frequency);

    if (!getPIDByName(&process, "ac_client.exe") || !getProcessHandle(&process))
        return 1;

    setting.dimensionUpdateRequest = true;
    // --- AIMBOT --- //
    setting.enableAimbot = true;
    setting.focusMate = true;
    setting.aimbotFocusMode = FROM_CROSSHAIR;
    setting.radius = 100.00f;
    setting.aimTime = 0.50f;
    setting.aimInPrecisionX = 0.10f;
    setting.aimInPrecisionY = 0.50f;
    setting.tracingEnabled = true;
    // --- TRIGGERBOT --- //
    setting.enableTriggerbot = true;
    setting.trigAlly = true;
    setting.trigDelay = 0;
    // --- RadareHack --- //
    setting.radareShowType = RADARE_ENNEMY;
    // --- Wallhack --- //
    setting.enableWallhack = true;
    // --- ESP --- //
    setting.espShowAlly = true;
    setting.espVisibilityCheck = true;
    setting.espShowHealth = true;
    setting.espShowArmor = true;
    setting.espShowWeapon = true;

    resolveLocalPlayerPtr();
    enableTracing();
    writeTracelineWrapper();
    wallhackLogic();
    radareLogic();

    if (!createOverlay(hInstance, nCmdShow, WndProc))
        return false;

    MSG msg;

    initilize();

    SetTimer(overlayHWND, WM_TIMER, 16, (TIMERPROC)TimerProc); // 60 FPS

    while (GetMessage(&msg, NULL, 0, 0))
    {

        QueryPerformanceCounter(&start);

        if (msg.message == WM_TIMER) // Send WM_TIMER messages to the hwndTimer procedure.
            msg.hwnd = overlayHWND;

        updateLocalPlayerInformation();
        updateEntitiesInformation();

        aimbot();
        triggerbot();

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
