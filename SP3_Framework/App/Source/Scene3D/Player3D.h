/**
 CPlayer3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "Camera.h"

// Include GroundMap
#include "GroundMap.h"

// Include WeaponInfo
#include "WeaponInfo/WeaponInfo.h"

// Include Physic3D
#include "Physics3D.h"

// Include CSoundController
#include "../SoundController/SoundController.h"

// Include LoadOBJs
#include "../Library/Source/System/LoadOBJ.h"

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CPlayer3D : public CSingletonTemplate<CPlayer3D>, public CEntity3D
{
	friend class CSingletonTemplate<CPlayer3D>;

public:
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum Player_Movement {
		FORWARD = 0,
		FORWARD_RUNNING,
		BACKWARD,
		LEFT,
		RIGHT,
		NUM_MOVEMENTS
	};

	// Player Attributes
	glm::vec3 vec3Up;
	glm::vec3 vec3Right;
	glm::vec3 vec3WorldUp;
	// Euler Angles
	float fYaw;
	float fPitch;
	// Player options
	float fMouseSensitivity;

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix(void) const;

	// Attach a camera to this class instance
	void AttachCamera(CCamera* cCamera = NULL);
	// Check if a camera ia attached to this class instance
	bool IsCameraAttached(void);

    // Processes input received from any keyboard-like input system as player movements. 
	// Accepts input parameter in the form of Player defined ENUM (to abstract it from windowing systems)
	void ProcessMovement(const Player_Movement direction, const float fDeltaTime);

    // Processes input received from a mouse input system as player rotation. 
	// Expects the offset value in both the x and y direction.
	void ProcessRotate(float fXOffset, float fYOffset, const bool bConstrainPitch = true);

	void SetPosition(glm::vec3 pos);

	// Set weapon to this class instance 
	void SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo = NULL);

	// Get weapon for this class instance
	CWeaponInfo* GetWeapon(void) const;

	CWeaponInfo* GetInventoryWeapon(const int iSlot) const;

	// Set current weapon
	void SetCurrentWeapon(const int iSlot);

	// Discharge current weapon
	CProjectile* DischargeWeapon(void) const;

	void DropWeapon(void);

	// Setter
	void SetScopeMode(bool scopeMode);
	void SetShootingMode(bool shootingMode);
	void SetCurrHealth(int currhealth);

	// Getter
	bool GetScopeMode();
	bool GetShootingMode();
	int GetMaxHealth();
	int GetCurrHealth();

	// jump 
	void SetToJump(void);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	// The handle to the CCamera class instance
	CCamera* cCamera;

	// camera sway
	float fCameraSwayAngle;
	float fCameraSwayDeltaAngle;
	// the limits for the left and right sway
	const float fCameraSwayAngle_LeftLimit = -2.0f, fCameraSwayAngle_RightLimit = 2.0f;
	bool bCameraSwayDirection;
	bool bCameraSwayActive;
	bool bUpdateCameraSway;

	// Handler to the cGroundMap
	CGroundMap* cGroundMap;

	CSoundController* cSoundController;

	// Hnadler to the WeaponInfo instance 
	CWeaponInfo* cPrimaryWeapon;
	CWeaponInfo* cSecondaryWeapon;
	int iCurrentWeapon;
	bool scopeMode;
	bool shootingMode;

	int maxHealth;
	int currHealth;

	// Handler to to physic3d
	CPhysics3D cPhysics3D;

	// update jump fall
	void UpdateJumpFall(const double dElapsedTime);

	// Default Constructor
	CPlayer3D(void);

	// Constructor with vectors
	CPlayer3D(	const glm::vec3 vec3Position,
				const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f),
				const float fYaw = -90.0f,
				const float fPitch = 0.0f);

	// Destructor
	virtual ~CPlayer3D(void);

    // Calculates the front vector from the Player's (updated) Euler Angles
	void UpdatePlayerVectors(void);

	// Constraint the player's position
	void Constraint(void);
};
