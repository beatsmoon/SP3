#pragma once
#include <includes/gtc/type_ptr.hpp>

//#include "../App/Source/Scene3D/Player3D.h"
#include "../Library/Source/Primitives/Entity3D.h"

enum Virus_Type
{
	T_HP_INCREASE = 0,
	T_SPEED_INCREASE,
	T_RELOAD_SPEED_INCREASE,
	T_SCORE_MULTIPLIER_INCREASE,
	T_WEAPON_DAMAGE_INCREASE,
	T_HP_DECREASE,
	T_SPEED_DECREASE,
	T_RELOAD_SPEED_DECREASE,
	T_SCORE_MULTIPLIER_DECREASE,
	T_WEAPON_DAMAGE_DECREASE,
};

class CVirus : public CEntity3D
{
public:

	// Constructor
	CVirus(void);
	CVirus(glm::uvec2 etype, int numofvirus);
	// Destructor
	virtual ~CVirus(void);

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

	//CPlayer3D* cPlayer3D;

	bool bActive;

	glm::uvec2 type;

	int numOfVirus;

};


