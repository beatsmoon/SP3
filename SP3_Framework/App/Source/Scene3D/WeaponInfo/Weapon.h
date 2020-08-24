#pragma once

#include "WeaponInfo.h"

enum Weapon_Type
{
	W_PISTOL,
	W_M4,
	W_AK47,
	W_SHOTGUN,
	W_SNIPER,
	NUM_WEAPON
};

class CWeapon : public CWeaponInfo
{
	

private:

	Weapon_Type type;

public:
	CWeapon();

	CWeapon(Weapon_Type type);

	Weapon_Type GetWeaponName();

	virtual ~CWeapon();

	void Init(void);

	float CalculateGravityMultiplier();
};

