#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Camera
#include "../Camera.h"

// Include Player3D
#include "../Player3D.h"

// Include GroundMap
#include "../GroundMap.h"

#include "../Library/Source/System/LoadOBJ.h"

#include "../App/Source/SoundController/SoundController.h"

#include "../../Meshes/Mesh.h"

#include "../../Meshes/MatrixStack.h"

#include "Enemy3D.h"


class CRangeIndicator : public CEntity3D
{
public:
	// Default Constructor
	CRangeIndicator(void);

	// Constructor with vectors
	CRangeIndicator(const glm::vec3 vec3Position, int enemy_type, CEnemy3D* currenemy);

	// Destructor
	virtual ~CRangeIndicator(void);

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

	// Activate the CCollider for this class instance
	virtual void ActivateCollider(Shader* cLineShader = NULL);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	CEnemy3D* GetCurrEnemy(void);

protected:
	
	
	// curr enemy
	CEnemy3D* currEnemy;

	int type;

};

