#include "pch.h"
#include "includes.h"

IDirect3D9Ex* p_Object = 0;
IDirect3DDevice9Ex* p_Device = 0;
D3DPRESENT_PARAMETERS p_Params;
RECT rc;
ID3DXFont* pFont;
ID3DXFont* pFontSmall;

int DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(1);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		exit(1);

	LoadFont();

	return 0;
}

void LoadFont() {
	D3DXCreateFont(p_Device, 50, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFont);
	D3DXCreateFont(p_Device, 26, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFontSmall);
}

int Render()
{
	if (tWnd == GetForegroundWindow())
	{
		HWND newhwnd = FindWindow(NULL, tWindowName);
		if (newhwnd != NULL) {
			GetWindowRect(newhwnd, &rc);
		}
		else {
			ExitProcess(0);
		}

		p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
		p_Device->BeginScene();

		//Drawing Stuff
		visuals::render();
		/*DrawString("Simple Text", 10, 10, 255, 255, 0, 0, pFont);
		DrawFilledRectangle(10, 10, 100, 100, 255, 255, 255, 255);*/

		p_Device->EndScene();
		p_Device->PresentEx(0, 0, 0, 0, 0);
		return 0;
	}
}