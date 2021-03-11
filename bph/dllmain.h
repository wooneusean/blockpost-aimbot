#pragma once
#include "includes.h"

extern int Width;
extern int Height;
extern const MARGINS Margin;
extern char lWindowName[256];
extern HWND hWnd;
extern char tWindowName[256];
extern HWND tWnd;
extern RECT tSize;
extern MSG Message;
extern bool Debug_Border;
//void SetWindowToTarget();
void InitOverlay(HINSTANCE hInstance);
void ErrorExit(LPTSTR lpszFunction);
void RepositionOverlay();
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);