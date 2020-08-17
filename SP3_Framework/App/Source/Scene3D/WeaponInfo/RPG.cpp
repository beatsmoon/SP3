#include "RPG.h"

/**
@brief Default Constructor
*/
CRPG::CRPG()
{
}

/**
@brief Default Destructor
*/
CRPG::~CRPG()
{
}

/**
@brief Initialise this instance to default values
*/
void CRPG::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	iMagRounds = 2;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 2;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 2;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 2;

	// The time between shots
	dTimeBetweenShots = 10.0;
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
