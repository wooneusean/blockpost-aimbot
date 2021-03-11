#include "pch.h"
#include "includes.h"
#include <strsafe.h>

int Width = GetSystemMetrics(SM_CXSCREEN);
int Height = GetSystemMetrics(SM_CYSCREEN);

const MARGINS Margin = { 0, 0, Width, Height };

char lWindowName[256] = "BPH";
HWND hWnd;

char tWindowName[256] = "BLOCKPOST";
HWND tWnd;

RECT tSize;
MSG Message;

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

DWORD WINAPI HackThread(HMODULE hModule) {
	// Create console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	// get module base
	g::moduleBase = (uintptr_t)GetModuleHandle("GameAssembly.dll");
	g::unityPlayerBase = (uintptr_t)GetModuleHandle("UnityPlayer.dll");
	g::baseClass = *(BaseClass**)(g::moduleBase + 0x00B2D7B8);
	g::weapBase = *(BaseWeap**)(g::moduleBase + 0x00ACA8FC);
	g::viewMatrix = (float*)mem::FindDMAAddy(g::unityPlayerBase + 0x010F3ED8, { 0x40C,0x7EC,0x0,0x414,0xA8 });

	uintptr_t dynamicEntListAddr = g::moduleBase + 0x00B2D824;
	g::entListBase = mem::FindDMAAddy(dynamicEntListAddr, { 0x5C,0xC,0x0 }) + 0x10;

	hooks::init();

	std::cout << "BLOCKPOST INTERNAL" << std::endl;
	std::cout << "Num1 - Change Mode (FFA / TDM)" << std::endl;

	InitOverlay(hModule);

	CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)RepositionOverlay, 0, 0, nullptr));

	while (true) {
		if (GetAsyncKeyState(VK_END) & 1) {
			break;
		}

		if (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message);
			TranslateMessage(&Message);
		}
		Sleep(5);
	}

	// Cleanup and unload dll
	hooks::destroy();

	p_Device->Release();
	p_Object->Release();

	DestroyWindow(hWnd);
	UnregisterClass(lWindowName, hModule);

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		Render();
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		return 0;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void RepositionOverlay() {
	while (true) {
		tWnd = FindWindow(0, tWindowName);
		if (tWnd)
		{
			GetWindowRect(tWnd, &tSize);
			Width = tSize.right - tSize.left;
			Height = tSize.bottom - tSize.top;
			DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				tSize.top += 23;
				Height -= 23;
			}
			if (hWnd) {
				MoveWindow(hWnd, tSize.left, tSize.top, Width, Height, true);
			}
			else {
				ExitThread(0);
			}
		}
		else {
			ExitThread(0);
		}
		Sleep(1);
	}
}

void InitOverlay(HINSTANCE hInstance) {
	WNDCLASSEX wClass;
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = WndProc;
	wClass.lpszClassName = lWindowName;
	wClass.lpszMenuName = lWindowName;
	wClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wClass)) {
		ErrorExit(const_cast<LPSTR>("RegisterClassEx"));
		exit(1);
	}

	tWnd = FindWindow(0, tWindowName);
	if (tWnd)
	{
		GetWindowRect(tWnd, &tSize);
		Width = tSize.right - tSize.left;
		Height = tSize.bottom - tSize.top;
		hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, lWindowName, lWindowName, WS_POPUP, 1, 1, Width, Height, 0, 0, 0, 0);
		SetLayeredWindowAttributes(hWnd, 0, 1.0f, LWA_ALPHA);
		SetLayeredWindowAttributes(hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
		ShowWindow(hWnd, SW_SHOW);
	}

	DirectXInit(hWnd);
}