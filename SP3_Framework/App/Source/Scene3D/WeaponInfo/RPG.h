#pragma once

#include "WeaponInfo.h"

class CRPG :	public CWeaponInfo
{
public:
	// Default Constructor
	CRPG(void);
	// Destructor
	virtual ~CRPG(void);

	// Initialise this instance to default values
	void Init(void);
};

