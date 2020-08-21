#include "Weapon.h"

CWeapon::CWeapon() : type(W_PISTOL)
{
}

CWeapon::CWeapon(Weapon_Type type)
{
	this->type = type;
}

Weapon_Type CWeapon::GetWeaponName()
{
	return type;
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
		iMaxTotalRounds = 40;

		// The time between shots
		dTimeBetweenShots = 0.3333;
		// The elapsed time (between shots)
		dElapsedTime = 0.0;
		// Boolean flag to indicate if weapon can fire now
		bFire = true;

		FiringTypeStatus = FIRINGTYPE::SINGLE;

		dReloadElapsedTime = 0.0;
		dReloadTime = 2.0;
		bIsReloading = false;

		type = Weapon_Type::W_PISTOL;
		fRecoilDeltaAngle = 0.5f;
		fDamage = 15;

		break;
	case W_M4:

		// The number of ammunition in a magazine for this weapon
		iMagRounds = 20;
		// The maximum number of ammunition for this magazine for this weapon
		iMaxMagRounds = 20;
		// The current total number of rounds currently carried by this player
		iTotalRounds = 40;
		// The max total number of rounds currently carried by this player
		iMaxTotalRounds = 80;

		// The time between shots
		dTimeBetweenShots = 0.111;
		// The elapsed time (between shots)
		dElapsedTime = 0.0;
		// Boolean flag to indicate if weapon can fire now
		bFire = true;

		FiringTypeStatus = FIRINGTYPE::AUTO;

		dReloadElapsedTime = 0.0;
		dReloadTime = 5.0;
		bIsReloading = false;

		type = Weapon_Type::W_M4;
		fRecoilDeltaAngle = 2.0f;
		fDamage = 15;

		break;
	case W_AK47:

		// The number of ammunition in a magazine for this weapon
		iMagRounds = 20;
		// The maximum number of ammunition for this magazine for this weapon
		iMaxMagRounds = 20;
		// The current total number of rounds currently carried by this player
		iTotalRounds = 40;
		// The max total number of rounds currently carried by this player
		iMaxTotalRounds = 80;

		// The time between shots
		dTimeBetweenShots = 0.111;
		// The elapsed time (between shots)
		dElapsedTime = 0.0;
		// Boolean flag to indicate if weapon can fire now
		bFire = true;

		FiringTypeStatus = FIRINGTYPE::AUTO;

		dReloadElapsedTime = 0.0;
		dReloadTime = 5.0;
		bIsReloading = false;

		type = Weapon_Type::W_AK47;
		fRecoilDeltaAngle = 2.0f;
		fDamage = 15;

		break;
	case W_SHOTGUN:

		// The number of ammunition in a magazine for this weapon
		iMagRounds = 8;
		// The maximum number of ammunition for this magazine for this weapon
		iMaxMagRounds = 8;
		// The current total number of rounds currently carried by this player
		iTotalRounds = 40;
		// The max total number of rounds currently carried by this player
		iMaxTotalRounds = 80;

		// The time between shots
		dTimeBetweenShots = 0.555;
		// The elapsed time (between shots)
		dElapsedTime = 0.0;
		// Boolean flag to indicate if weapon can fire now
		bFire = true;

		FiringTypeStatus = FIRINGTYPE::SINGLE;

		dReloadElapsedTime = 0.0;
		dReloadTime = 5.0;
		bIsReloading = false;

		type = Weapon_Type::W_SHOTGUN;
		fRecoilDeltaAngle = 2.0f;
		fDamage = 15;

		break;
	case W_SNIPER:

		// The number of ammunition in a magazine for this weapon
		iMagRounds = 3;
		// The maximum number of ammunition for this magazine for this weapon
		iMaxMagRounds = 3;
		// The current total number of rounds currently carried by this player
		iTotalRounds = 40;
		// The max total number of rounds currently carried by this player
		iMaxTotalRounds = 40;

		// The time between shots
		dTimeBetweenShots = 5.555;
		// The elapsed time (between shots)
		dElapsedTime = 0.0;
		// Boolean flag to indicate if weapon can fire now
		bFire = true;

		FiringTypeStatus = FIRINGTYPE::AUTO;

		dReloadElapsedTime = 0.0;
		dReloadTime = 7.0;
		bIsReloading = false;

		type = Weapon_Type::W_SNIPER;
		fRecoilDeltaAngle = 8.5f;
		fDamage = 50;

		break;
	case NUM_WEAPON:
		break;
	default:
		break;
	}
}
