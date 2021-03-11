#pragma once
#include "pch.h"
#include "includes.h"
class Vector4
{
public:
	float x, y, z, w;

	Vector4(float x = 0, float y = 0, float z = 0, float w = 0)
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}
};

class Vector3
{
public:
	float x, y, z;

	Vector3(float x = 0, float y = 0, float z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3 operator+(Vector3 other) {
		return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
	}

	Vector3 operator-(Vector3 other) {
		return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	float Magnitude() {
		return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	}

	float Dot(Vector3 other) {
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}

	Vector3 Normalize() {
		float hypo = this->Magnitude();
		return Vector3(this->x / hypo, this->y / hypo, this->z / hypo);
	}
};

class Vector2
{
public:
	//  yaw  pitch
	float x, y;

	Vector2(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}
};

// Created with ReClass.NET 1.2 by KN4CK3R

class BaseClass
{
public:
	char pad_0000[92]; //0x0000
	class ParentEnt* ParentEntPtr; //0x005C
}; //Size: 0x0060
static_assert(sizeof(BaseClass) == 0x60);

class ParentEnt
{
public:
	char pad_0000[60]; //0x0000
	Vector2 Angles; //0x003C
	char pad_0044[100]; //0x0044
	float Vertical; //0x00A8
	char pad_00AC[4]; //0x00AC
	float Horizontal; //0x00B0
	char pad_00B4[132]; //0x00B4
	class Ent* EntPtr; //0x0138
	char pad_013C[692]; //0x013C
	int32_t Health; //0x03F0
	char pad_03F4[32]; //0x03F4
	int32_t Ammo; //0x0414
	char pad_0418[120]; //0x0418
	int32_t hasBeenHit; //0x0490
}; //Size: 0x0494
static_assert(sizeof(ParentEnt) == 0x494);

class Ent
{
public:
	char pad_0000[20]; //0x0000
	int32_t Team; //0x0014
	class StringClass* NameStringPtr; //0x0018
	char pad_001C[12]; //0x001C
	int32_t Health; //0x0028
	char pad_002C[8]; //0x002C
	Vector3 UnkwnCoords1; //0x0034
	char pad_0040[4]; //0x0040
	Vector3 Coords; //0x0044
	Vector3 UnkwnCoords2; //0x0050
	char pad_005C[236]; //0x005C
	int32_t MovementType; //0x0148
	char pad_014C[76]; //0x014C
	bool SpawnProtect; //0x0198
	char pad_0199[3]; //0x0199
}; //Size: 0x019C
static_assert(sizeof(Ent) == 0x19C);

class StringClass
{
public:
	char pad_0000[12]; //0x0000
	wchar_t Name[16]; //0x000C
}; //Size: 0x002C
static_assert(sizeof(StringClass) == 0x2C);

class BaseWeap
{
public:
	char pad_0000[1180]; //0x0000
	class CurrWeap* CurrWeapPtr; //0x049C
}; //Size: 0x04A0
static_assert(sizeof(BaseWeap) == 0x4A0);

class CurrWeap
{
public:
	char pad_0000[468]; //0x0000
	class Weapon* WeaponPtr; //0x01D4
}; //Size: 0x01D8
static_assert(sizeof(CurrWeap) == 0x1D8);

class Weapon
{
public:
	char pad_0000[12]; //0x0000
	class WeaponInfo* WeaponInfoPtr; //0x000C
}; //Size: 0x0010
static_assert(sizeof(Weapon) == 0x10);

class WeaponInfo
{
public:
	char pad_0000[16]; //0x0000
	class Ammo* AmmoPtr[5]; //0x0010
}; //Size: 0x0024
static_assert(sizeof(WeaponInfo) == 0x24);

class Ammo
{
public:
	char pad_0000[40]; //0x0000
	int32_t AmmoInMag; //0x0028
	int32_t ReserveAmmo; //0x002C
}; //Size: 0x0030
static_assert(sizeof(Ammo) == 0x30);