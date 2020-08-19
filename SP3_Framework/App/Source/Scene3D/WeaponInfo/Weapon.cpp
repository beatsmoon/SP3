#include "Weapon.h"

CWeapon::CWeapon() : type(W_PISTOL)
{
}

CWeapon::CWeapon(Weapon_Type type)
{
	this->type = type;
}

CWeapon::~CWeapon()
{
}

void CWeapon::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	switch (type)
	{
	case W_PISTOL:

		// The number of ammunition in a magazine for this weapon
		iMagRounds = 8;
		// The maximum number of ammunition for this magazine for this weapon
		iMaxMagRounds = 8;
		// The current total number of rounds currently carried by this player
		iTotalRounds = 40;
		// The max total number of rounds currently carried by this player
		iMaxTotalRounds = 80;

		// The time between shots
		dTimeBetweenShots = 0.3333;
		// The elapsed time (between shots)
		dElapsedTime = 0.0;
		// Boolean flag to indicate if weapon can fire now
		bFire = true;

		scopeZoom = 2.f;

		break;
	case W_RIFLE:

		// The number of ammunition in a magazine for this weapon
		iMagRounds = 30;
		// The maximum number of ammunition for this magazine for this weapon
		iMaxMagRounds = 30;
		// The current total number of rounds currently carried by this player
		iTotalRounds = 90;
		// The max total number of rounds currently carried by this player
		iMaxTotalRounds = 120;

		// The time between shots
		dTimeBetweenShots = 0.1;
		// The elapsed time (between shots)
		dElapsedTime = 0.0;
		// Boolean flag to indicate if weapon can fire now
		bFire = true;

		scopeZoom = 15.f;

		break;
	case NUM_WEAPON:
		break;
	default:
		break;
	}
}
