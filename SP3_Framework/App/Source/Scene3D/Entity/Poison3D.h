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
#include "CBoss3D.h"


class CPoison3D : public CEntity3D
{
public:
	// Default Constructor
	CPoison3D(void);

	// Constructor with vectors
	CPoison3D(const glm::vec3 vec3Position, glm::uvec2 entity_type, CEntity3D* currenemy);

	// Destructor
	virtual ~CPoison3D(void);

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

	CEntity3D* GetCurrEnemy(void);

protected:
	
	
	// curr enemy
	CEntity3D* currEnemy;

	glm::uvec2 type;

	float colliderSize;

	//int type;

};

