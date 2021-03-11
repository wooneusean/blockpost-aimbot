#pragma once
#include "includes.h"

namespace visuals {
	bool WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight);
	void render();
	void boxes();
}