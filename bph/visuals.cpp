#include "pch.h"
#include "includes.h"

void visuals::render() {
	DrawString("BLOCKPOST INTERNAL", 10, 10, 255, 255, 0, 0, pFontSmall	);

	if (g::baseClass->ParentEntPtr->Health <= 0) { return; }

	boxes();
	//BoxEnemy();
}

bool visuals::WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vector4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];
	//clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	//clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	//clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	//clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

void visuals::boxes() {
	for (unsigned int i = 0; i < 40; i++)
	{
		Ent* ent = *(Ent**)(g::entListBase + (i * 0x4));
		if (ent) {
			if (ent == g::baseClass->ParentEntPtr->EntPtr || ent->Health <= 10) { continue; }
			std::wstring ws(ent->NameStringPtr->Name);
			std::string name(ws.begin(), ws.end());
			if (g::tdm) {
				if (ent->Team != g::baseClass->ParentEntPtr->EntPtr->Team) {
					Vector2 drawPos;
					if (WorldToScreen(ent->Coords, drawPos, g::viewMatrix, Width, Height)) {
						DrawString(name.c_str(), drawPos.x, drawPos.y, 255, 255, 255, 255, pFont);
					}
				}
			}
			else {
				if (ent->Team == 0) {
					Vector2 drawPos;
					if (WorldToScreen(ent->Coords, drawPos, g::viewMatrix, Width, Height)) {
						DrawString(name.c_str(), drawPos.x, drawPos.y, 255, 255, 255, 255, pFont);
					}
				}
			}
		}
	}
}