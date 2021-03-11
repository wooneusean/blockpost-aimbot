#pragma once

extern IDirect3D9Ex* p_Object;
extern IDirect3DDevice9Ex* p_Device;
extern D3DPRESENT_PARAMETERS p_Params;
extern ID3DXLine* p_Line;
extern ID3DXFont* pFont;
extern ID3DXFont* pFontSmall;

int DirectXInit(HWND hWnd);
void LoadFont();
int Render();