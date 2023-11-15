#include "esp.h"
#include <wingdi.h>

HDC hdc;

const WCHAR overlayTitle[20] = L"AssaultCube Overlay"; // The title bar text
const LPCSTR targetTitle = "AssaultCube";

HWND overlayHWND;
RECT m_Rect;
COLORREF TextCOLOR;

HFONT Font;

int width;
int height;

HBRUSH hBrushEnemy;
HBRUSH hBrushTeam;
HBRUSH hBrushTeamNV;
HBRUSH hBrushNeutral;
COLORREF enemyColor;
COLORREF teamColor;
COLORREF neutralColor;

bool createOverlay(HINSTANCE _hinstance, int _nCmdShow, WNDPROC _wndproc)
{

    HANDLE targetHWND = FindWindowA(NULL, targetTitle);

    GetWindowRect(targetHWND, &m_Rect);
    width = m_Rect.right - m_Rect.left;
    height = m_Rect.bottom - m_Rect.top;

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = _wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = _hinstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName = overlayTitle;
    wcex.lpszClassName = overlayTitle;
    wcex.hIconSm = 0;

    RegisterClassExW(&wcex);

    overlayHWND = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, overlayTitle, overlayTitle, WS_POPUP, 1, 1, width, height, NULL, NULL, _hinstance, NULL);

    if (!overlayHWND)
        return false;

    SetLayeredWindowAttributes(overlayHWND, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(overlayHWND, _nCmdShow);

    MoveWindow(overlayHWND, m_Rect.left, m_Rect.top, width, height, true);

    return true;
}

void inline DrawString(Vector2 head2D, COLORREF color, const char *text, float _offset)
{

    head2D.y -= (20.00f * _offset);

    SetTextAlign(hdc, TA_CENTER | TA_NOUPDATECP);
    SetBkColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
    SelectObject(hdc, Font);
    TextOutA(hdc, head2D.x, head2D.y, text, strlen(text));
    DeleteObject(Font);
}

void inline draw2Dboxes(Vector2 _head2D, float _d, HBRUSH _color)
{

    float w = scaleWidthFromDistance(_d);
    float h = scaleHeightFromDistance(_d);

    RECT rec = {_head2D.x - w, _head2D.y, _head2D.x + w, _head2D.y + h};

    FrameRect(hdc, &rec, _color);

    return;
}

void esp()
{

    float distance;
    Vector2 head2D;
    char health[10];
    char armor[10];
    char weaponInfo[15];

    for (uint8_t i = 0; i < entityNbr - 1; i++)
    {

        if (entities[i].playerState != ALIVE)
            continue; // skip dead players

        if (!setting.espShowAlly && entities[i].team == localPlayer.team)
            continue; // skip ally

        if (!worldToScreen(entities[i].position, &head2D))
            continue; // not a valid 3D point

        distance = getDistanceFrom(localPlayer.position, entities[i].position);

        if (entities[i].team == localPlayer.team)
        { // ally

            if (setting.espVisibilityCheck)
            {
                if (isEntityVisible(i))
                    draw2Dboxes(head2D, distance, hBrushTeam); // ally visible
                else
                    draw2Dboxes(head2D, distance, hBrushTeamNV); // ally not visible
            }
            else
            {

                draw2Dboxes(head2D, distance, hBrushTeam);
            }
        }
        else
        { // ennemy

            if (setting.espVisibilityCheck)
            {
                if (isEntityVisible(i))
                    draw2Dboxes(head2D, distance, hBrushEnemy);
                else
                    draw2Dboxes(head2D, distance, hBrushNeutral);
            }
            else
            {
                draw2Dboxes(head2D, distance, hBrushEnemy);
            }
        }

        if (setting.espShowHealth)
        {

            sprintf(health, "HP : %d", entities[i].health);
            DrawString(head2D, neutralColor, health, 1);
        }

        if (setting.espShowArmor)
        {

            sprintf(armor, "ARMOR : %d", entities[i].armor);
            DrawString(head2D, neutralColor, armor, 2);
        }

        if (setting.espShowWeapon)
        {

            sprintf(weaponInfo, "%s", gunsName[getEntityWeaponIndex(i)]);
            DrawString(head2D, neutralColor, weaponInfo, 3);
        }
    }
}

void initilize()
{
    hBrushEnemy = CreateSolidBrush(RGB(255, 0, 0));
    hBrushTeam = CreateSolidBrush(RGB(0, 0, 255));
    hBrushNeutral = CreateSolidBrush(RGB(255, 255, 255));
    hBrushTeamNV = CreateSolidBrush(RGB(0, 102, 0));
    enemyColor = RGB(255, 0, 0);
    teamColor = RGB(0, 0, 255);
    neutralColor = RGB(255, 255, 255);
    TextCOLOR = RGB(0, 255, 0);
}