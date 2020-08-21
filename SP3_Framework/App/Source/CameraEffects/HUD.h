#pragma once


// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include "../App/Source/Scene3D/Player3D.h"


//enum HUD_Type
//{
//	H_HOLDING_GUN,
//	H_SELECTION_GUN
//};

class CHUD : public CEntity3D
{
public:

	// Constructor
	CHUD(void);
	CHUD(Weapon_Type etype);
	// Destructor
	virtual ~CHUD(void);

	// Initialise this class instance
	bool Init(void);

	// Set model
	void SetModel(glm::mat4 model);
	// Set view
	void SetView(glm::mat4 view);
	// Set projection
	void SetProjection(glm::mat4 projection);

	// Set crosshair on or off
	void SetStatus(const bool bStatus);
	// Get crosshair status
	bool GetStatus(void) const;

	// Update this class instance
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);
	// Render
	void Render(void);
	// PostRender
	void PostRender(void);

protected:
	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	CPlayer3D* cPlayer3D;

	bool bActive;

	Weapon_Type type;
	Weapon_Type prevtype;

	//float fHeight, fWidth;

	
};

