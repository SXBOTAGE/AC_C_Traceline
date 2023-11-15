#ifndef ESP_H_INCLUDED
#define ESP_H_INCLUDED

#include "../../Game.h"
#include <stdio.h>



extern const WCHAR overlayTitle[20];  // The title bar text
extern const LPCSTR targetTitle;
extern HDC hdc;
extern HFONT Font;
extern COLORREF TextCOLOR;
extern RECT m_Rect;
extern HWND overlayHWND;
extern HBRUSH hBrushEnemy;
extern HBRUSH hBrushTeam;
extern HBRUSH hBrushNeutral;
extern HBRUSH hBrushTeamNV;
extern COLORREF enemyColor;
extern COLORREF teamColor;
extern COLORREF neutralColor;
extern int width, height;


extern bool createOverlay(HINSTANCE _hinstance,int _nCmdShow, WNDPROC _wndproc);
extern void DrawString(Vector2 head2D, COLORREF color, const char* text, float _offset);
extern void draw2Dboxes(Vector2 _head2D, float _d, HBRUSH _color); // simple rectangle
extern void esp();
extern void initilize();





#endif