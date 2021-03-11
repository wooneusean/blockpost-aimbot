#include "pch.h"
#include "includes.h"
#define M_PI 3.14159265358979323846

float GetDistance(Ent* ent) {
	float x = pow((g::baseClass->ParentEntPtr->EntPtr->Coords.x - ent->Coords.x), 2);
	float y = pow((g::baseClass->ParentEntPtr->EntPtr->Coords.y - ent->Coords.y), 2);
	float z = pow((g::baseClass->ParentEntPtr->EntPtr->Coords.z - ent->Coords.z), 2);
	return (sqrt(x + y + z));
}

float Rad2Deg(float rad) {
	return rad * (180 / M_PI);
}

float Deg2Rad(float deg) {
	return deg * (M_PI / 180);
}

Vector2 CalcAngles(Ent* ent) {
	if (ent == nullptr) { return g::baseClass->ParentEntPtr->Angles; }

	Vector3 vecDelta;
	vecDelta.x = ent->Coords.x - g::baseClass->ParentEntPtr->EntPtr->Coords.x;
	vecDelta.y = ent->Coords.y - g::baseClass->ParentEntPtr->EntPtr->Coords.y;
	vecDelta.z = ent->Coords.z - g::baseClass->ParentEntPtr->EntPtr->Coords.z;

	if (ent->MovementType == 2 || ent->MovementType == 3) {
		vecDelta.y -= 0.8f;
	}
	if (g::baseClass->ParentEntPtr->EntPtr->MovementType == 2 || g::baseClass->ParentEntPtr->EntPtr->MovementType == 3) {
		vecDelta.y += 0.8f;
	}

	Vector2 newAngles;
	float hypo = sqrt(pow(vecDelta.x, 2) + pow(vecDelta.z, 2));

	if (vecDelta.z > 0) {
		newAngles.x = Rad2Deg(atan(vecDelta.x / vecDelta.z));
	}
	else if (vecDelta.x > 0 && vecDelta.z < 0) {
		newAngles.x = 180.f + Rad2Deg(atan(vecDelta.x / vecDelta.z));
	}
	else if (vecDelta.x < 0 && vecDelta.z < 0) {
		newAngles.x = -180 + Rad2Deg(atan(vecDelta.x / vecDelta.z));
	}

	newAngles.y = -Rad2Deg(asin(vecDelta.y / GetDistance(ent)));

	return newAngles;
}

void aimbot::run() {
	if (g::baseClass->ParentEntPtr->Health <= 0) { return; }

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		float bestDistance = 9999;
		Ent* targetEnt = nullptr;
		for (unsigned int i = 0; i < 40; i++)
		{
			Ent* ent = *(Ent**)(g::entListBase + (i * 0x4));
			if (ent) {
				if (ent == g::baseClass->ParentEntPtr->EntPtr || ent->Health <= 10 || ent->SpawnProtect == true) { continue; }

				if (g::tdm) {
					if (ent->Team != g::baseClass->ParentEntPtr->EntPtr->Team) {
						float currDist = GetDistance(ent);
						if (currDist < bestDistance) {
							bestDistance = currDist;
							targetEnt = ent;
						}
					}
				}
				else {
					if (ent->Team == 0) {
						float currDist = GetDistance(ent);
						if (currDist < bestDistance) {
							bestDistance = currDist;
							targetEnt = ent;
						}
					}
				}
			}
		}
		if (targetEnt) {
			Vector2 angle = CalcAngles(targetEnt);
			g::baseClass->ParentEntPtr->Angles = angle;
		}
	}
}

