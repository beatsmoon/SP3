/**
 CStructure3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "../App/Source/Scene3D/Camera.h"

// Include Player3D
//#include "Player3D.h"

// Include GroundMap
#include "../App/Source/Scene3D/GroundMap.h"

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class CGunScope : public CEntity3D
{
public:
	// Default Constructor
	CGunScope(void);

	// Constructor with vectors
	CGunScope(	const glm::vec3 vec3Position,
				const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f),
				const float fYaw = -90.0f,
				const float fPitch = 0.0f);

	// Destructor
	virtual ~CGunScope(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	virtual void AddAmmo(void);

	// Activate the CCollider for this class instance
	virtual void ActivateCollider(Shader* cLineShader = NULL);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	virtual float GetAdditionMag(void);

	enum TIERLEVEL
	{
		NOTIER = 0,
		TIER1,
		TIER2,
		TIER3,
		NUM_TIERLEVEL
	};
	// Set crosshair on or off
//	virtual void SetStatus(const bool bStatus);
	// Get crosshair status
//	virtual bool GetStatus(void) const;

	void SetTierLevel(const TIERLEVEL eStatus);
	TIERLEVEL GetTierLevel(void) const;

	void UpgradeScope();
protected:
	// The handle to the CGroundMap class instance
	CGroundMap* cGroundMap;

	//CPlayer3D* cPlayer3D;
	bool bActive;
	TIERLEVEL eTierLevel;
};
