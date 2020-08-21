#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>
//#include "../App/Source/Scene3D/Player3D.h"
#include "../App/Source/Scene3D/WeaponInfo/WeaponAttachments/AttachmentScope.h"

class CScope : public CSingletonTemplate<CScope>, public CEntity3D
{
	friend class CSingletonTemplate<CScope>;

public:
	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Set crosshair on or off
	virtual void SetStatus(const bool bStatus);
	// Get crosshair status
	virtual bool GetStatus(void) const;

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	//enum TIERLEVEL
	//{
	//	NOTIER = 0,
	//	TIER1,
	//	TIER2,
	//	TIER3,
	//	NUM_TIERLEVEL
	void UpdateTierListOfScope(CGunScope::TIERLEVEL tierLevel);
	//};

	//void SetTierLevel(const TIERLEVEL eStatus);
	//TIERLEVEL GetTierLevel(void) const;

protected:
	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	bool bActive;

	//CPlayer3D* cPlayer3D;
	// Constructor
	CScope(void);
	// Destructor
	virtual ~CScope(void);

	CGunScope::TIERLEVEL eTierLevel;
	CGunScope::TIERLEVEL eStorePrevTierLevel;
};
