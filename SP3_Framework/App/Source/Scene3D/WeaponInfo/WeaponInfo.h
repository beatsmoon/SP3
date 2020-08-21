#pragma once

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include shader
#include <RenderControl\shader.h>

// Include Projectile
#include "Projectile.h"

#include "WeaponAttachments/AttachmentBarrel.h"
#include "WeaponAttachments/AttachmentExtMag.h"
//#include "WeaponAttachments/Scope.h"
#include "WeaponAttachments/AttachmentScope.h"

//#include "../Player3D.h"

class CWeaponInfo
{
public:
	// Default Constructor
	CWeaponInfo(void);
	// Destructor
	virtual ~CWeaponInfo(void);

	// Set a shader to this class instance
	virtual void SetShader(Shader* cShader);

	// Set the number of ammunition in the magazine for this player
	virtual void SetMagRound(const int iMagRounds);
	// Set the maximum number of ammunition in the magazine for this weapon
	virtual void SetMaxMagRound(const int iMagRounds);
	// The current total number of rounds currently carried by this player
	virtual void SetTotalRound(const int iTotalRounds);
	// The max total number of rounds currently carried by this player
	virtual void SetMaxTotalRound(const int iMaxTotalRounds);

	// Get the number of ammunition in the magazine for this player
	virtual int GetMagRound(void) const;
	// Get the maximum number of ammunition in the magazine for this weapon
	virtual int GetMaxMagRound(void) const;
	// Get the current total number of rounds currently carried by this player
	virtual int GetTotalRound(void) const;
	// Get the max total number of rounds currently carried by this player
	virtual int GetMaxTotalRound(void) const;

	virtual double GetReloadElapsed(void) const;
	virtual double GetReloadTime(void) const;

	// Set the time between shots
	virtual void SetTimeBetweenShots(const double dTimeBetweenShots);
	// Set the firing rate in rounds per min
	virtual void SetFiringRate(const int iFiringRate);
	// Set the firing flag
	virtual void SetCanFire(const bool bFire);

	// Get the time between shots
	virtual double GetTimeBetweenShots(void) const;
	// Get the firing rate
	virtual int GetFiringRate(void) const;
	// Get the firing flag
	virtual bool GetCanFire(void) const;
	virtual bool GetIsReloadStatus(void) const;

	// Initialise this instance to default values
	virtual void Init(void);
	// Update the elapsed time
	virtual void Update(const double dElapsedTime);
	// Discharge this weapon
	virtual CProjectile* Discharge(glm::vec3 vec3Position, glm::vec3 vec3Front, CEntity3D* pSource = NULL);
	// Reload this weapon
	virtual void Reload(void);
	// Add rounds
	virtual void AddRounds(const int iNewRounds);

	// Print Self
	virtual void PrintSelf(void);

	enum FIRINGTYPE
	{
		SINGLE = 0,
		BURST,
		AUTO,
		NUM_FIRINGTYPE
	};

	void SetFiringType(const FIRINGTYPE sStatus);
	FIRINGTYPE GetFiringType(void) const;

	void SetBarrel(CGunBarrel* cEntity3D);
	CGunBarrel* GetBarrel(void) const;

	void SetExtMag(CGunExtMag* cEntity3D);
	CGunExtMag* GetExtMag(void) const;

	void SetScope(CGunScope* cEntity3D);
	CGunScope* GetScope(void) const;

	void SetRecoilAngle(float fAngle);
	float GetRecoilAngle(void) const;

	virtual float GetDamageOutput(void) const;
	virtual void SetDamageOutput(const float fdamage);

protected:
	// Handler to the Shader Program instance
	Shader* cShader;

	// The number of ammunition in a magazine for this weapon
	int iMagRounds;
	// The maximum number of ammunition for this magazine for this weapon
	int iMaxMagRounds;
	// The current total number of rounds currently carried by this player
	int iTotalRounds;
	// The max total number of rounds currently carried by this player
	int iMaxTotalRounds;

	// The time between shots in milliseconds
	double dTimeBetweenShots;
	// The elapsed time (between shots) in milliseconds
	double dElapsedTime;
	// Boolean flag to indicate if weapon can fire now
	bool bFire;

	double dReloadTime;
	double dReloadElapsedTime;
	bool bIsReloading;

	float fRecoilDeltaAngle;
	float fDamage;
	float fSpeedOfBullet;
	
	FIRINGTYPE FiringTypeStatus;

	CGunBarrel* m_Barrel;
	CGunScope* m_Sight;
	CGunExtMag* m_ExtMag;
};
