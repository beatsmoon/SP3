/**
 CPlayer2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include Singleton template
#include "DesignPatterns\SingletonTemplate.h"
// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

// Include Settings
#include "GameControl\Settings.h"

// Include Physic
#include "Physics2D.h"

// include CEntity2D
#include "Entity2D.h"

#include "Bullets2D.h"

// Animation Sprites
#include "AnimatedSprites.h"

#include "Entity2DFactory.h"




class CPlayer2D : public CSingletonTemplate<CPlayer2D>, public CEntity2D
{
	friend CSingletonTemplate<CPlayer2D>;
private:
	bool p_haveGun;
	int p_health;
	int p_score;
	int p_bullets;
	bool p_isInvissible;
	int p_increaseLevel;
	//bool p_haveKey;
public:

	// Init
	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	bool IsPlayerAlive(void);

	int GetHealth();
	int GetBullets();
	int GetScore();
	bool GetIsInvissible();
	int GetIncreaseLevel();

	void SetBullets(int);
	void SetHealth(int);
	void SetIncreaseLevel(int);
	void SetScore(int);

protected:
	enum DIRECTION
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3,
		NUM_DIRECTIONS
	};

	
	CMap2D* cMap2D;

	glm::vec2 vec2UVCoordinate;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	// Settings
	CSettings* cSettings;

	// Physics
	CPhysics2D cPhysics2D;

	// AnimationSprites
	CAnimatedSprites cAnimatedSprites;

	// Constructor
	CPlayer2D(void);

	// Destructor
	virtual ~CPlayer2D(void);

	// Load a texture
	bool LoadTexture(const char* filename, GLuint& iTextureID);

	// Constraint the player's position within a boundary
	void Constraint(DIRECTION eDirection = LEFT);
	void FindFeasiblePosition(const int iOldIndex, DIRECTION eDirection);

	void InteractWithMap(void);

	// Update Jump or Fall
	void UpdateJumpFall(const double dElapsedTime);

	bool isMidAir(void);

};

