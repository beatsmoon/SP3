#pragma once

#include "WeaponInfo.h"

enum Weapon_Type
{
	W_PISTOL,
	W_RIFLE,
	NUM_WEAPON
};

class CWeapon : public CWeaponInfo
{
	

private:

	Weapon_Type type;

public:
	CWeapon();

	CWeapon(Weapon_Type type);

	virtual ~CWeapon();

	void Init(void);
};

