#pragma once
#include "includes.h"

namespace hooks {
	void init();
	void destroy();
	bool unhook(BYTE* src, BYTE* gateway, uintptr_t len);

	extern uintptr_t someFunctionAddr;
	extern BYTE* someFunctionGateway;
	int __cdecl hkSomeFunction(int a1);
}