#include "pch.h"
#include "includes.h"

typedef int(__cdecl* tSomeFunction) (int a1);
tSomeFunction oSomeFunction;
uintptr_t hooks::someFunctionAddr = NULL;
BYTE* hooks::someFunctionGateway = nullptr;

int __cdecl hooks::hkSomeFunction(int a1) {
	aimbot::run();

	if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
		g::tdm = !g::tdm;
		if (g::tdm) {
			std::cout << "Team Deathmatch Mode" << std::endl;
		}
		else {
			std::cout << "Free-For-All Mode" << std::endl;
		}
	}

	// scuffed
	/*if (g::baseClass->ParentEntPtr->Health > 0) {
		misc::NameChanger();
	}*/

	return oSomeFunction(a1);
}

void hooks::init() {
	someFunctionAddr = g::moduleBase + 0x6A87E0; // base addr
	oSomeFunction = (tSomeFunction)(someFunctionAddr);
	someFunctionGateway = mem::TrampHook32((BYTE*)(oSomeFunction), (BYTE*)hkSomeFunction, 6);

	// ori func calls the gateway, the gateway calls stolen bytes and jmp back to the original loc
	oSomeFunction = (tSomeFunction)someFunctionGateway;

	if (someFunctionGateway) {
		std::cout << "Hook success" << std::endl;
	}
	else {
		std::cout << "Hook failure" << std::endl;
	}

}

void hooks::destroy() {
	unhook((BYTE*)someFunctionAddr, someFunctionGateway, 6);
}

bool hooks::unhook(BYTE* src, BYTE* gateway, uintptr_t len) {
	DWORD oldProt;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProt);
	memcpy_s(src, len, gateway, len);
	VirtualProtect(src, len, oldProt, &oldProt);
	return VirtualFree(gateway, 0, MEM_RELEASE);
}