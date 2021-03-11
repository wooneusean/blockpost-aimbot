#pragma once
#include "includes.h"

#pragma warning( disable : 4244 )
#pragma warning( disable : 4996 )

void DrawString(const char* String, int x, int y, int a, int r, int g, int b, ID3DXFont* font);
void DrawFilledRectangle(float x, float y, float w, float h, int a, int r, int g, int b);
void DrawBorderBox(int x, int y, int w, int h, int thickness, int a, int r, int g, int b);