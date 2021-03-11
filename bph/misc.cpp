#include "pch.h"
#include "includes.h"
#include <stdlib.h>
#include <time.h>

std::string ws2s(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

void misc::SetName(std::string name) {
	std::string trimmedName = name.substr(0, 16);
	char x{ };
	while (trimmedName.length() < 16) {
		trimmedName += " ";
	}

	std::copy(std::begin(trimmedName), std::end(trimmedName), g::baseClass->ParentEntPtr->EntPtr->NameStringPtr->Name);
}

void misc::NameChanger() {
	static int i = 0;
	i++;
	if (i % 240 == 0) {
		i = 0;
		srand(time(NULL));
		Ent* ent = nullptr;
		while (!ent) {
			int entIndex = rand() % 39 + 0;
			ent = *(Ent**)(g::entListBase + (entIndex * 0x4));
		}
		if (ent) {
			std::string entName = ws2s(ent->NameStringPtr->Name);
			SetName(entName);
		}
	}
}