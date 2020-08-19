#pragma once

// Include Singleton template
#include "DesignPatterns\SingletonTemplate.h"
// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include "Entity2D.h"

//
//#include "Player2D.h"

#include "AnimatedSprites.h"


// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;


#pragma once
class CBullets2D : public CEntity2D
{
private:
public:

	CBullets2D();

	
	virtual ~CBullets2D();

	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	bool isActive;
	CAnimatedSprites cAnimatedSprites;

protected:
	enum BULLETDIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		NUM_DIRECTIONS
	};

	CMap2D* cMap2D;

	glm::vec2 vec2UVCoordinate;

	// Settings
	CSettings* cSettings;


	// Load a texture
	bool LoadTexture(const char* filename, GLuint& iTextureID);

	// Constraint the player's position within a boundary
	void Constraint(BULLETDIRECTION eDirection = LEFT);

	void InteractWithMap(void);

	bool FindFeasiblePosition(const int iOldIndex, BULLETDIRECTION eDirection);

	bool ShootBullets(BULLETDIRECTION edirection);

};

