#include "Scene3D.h"
#include <iostream>
using namespace std;

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <math.h>

#include "System\filesystem.h"

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include Weapon
#include "WeaponInfo/Weapon.h"

// Include CEnemy3D
#include "Enemy3D.h"

// Include MyMath.h
#include "../Library/Source/System/MyMath.h"


void wait(DWORD interval)
{
	DWORD startTime = GetTickCount();

	while (GetTickCount() < (startTime + interval))
	{
		//DO NOTHING JUST WAIT
	}
}

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cShader(NULL)
	, cSimpleShader(NULL)
	, cTextRenderer(NULL)
	, cGUIShader(NULL)
	, cGUISimpleShader(NULL)
	, cRenderToTextureShader(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cJoystickController(NULL)
	, cFPSCounter(NULL)
	, cSoundController(NULL)
	, cPlayer3D(NULL)
	, cEntityManager(NULL)
	, cSkybox(NULL)
	, cGroundMap(NULL)
	, cCamera(NULL)
	, cMinimap(NULL)
	, cCameraEffects(NULL)
	, cHealthBar(NULL)
	, cBulletsPerMagBar(NULL)
	, cTotalBulletsBar(NULL)
	, cCrossHair(NULL)
	, cHUD(NULL)
	, cRenderHoldingGun(NULL)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}

	// Destroy the cGroundMap
	if (cGroundMap)
	{
		cGroundMap->Destroy();
		cGroundMap = NULL;
	}

	// Destroy the cSoundController
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

	// Destroy thr cPlayer3D 
	if (cPlayer3D)
	{
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}

	// Destroy the cEntityManger
	if (cEntityManager)
	{
		cEntityManager->Destroy();
		cEntityManager = NULL;
	}

	// Destroy the cSkybox
	if (cSkybox)
	{
		cSkybox->Destroy();
		cSkybox = NULL;
	}

	if (cHUD)
	{
		delete cHUD;
		cHUD = NULL;
	}

	if (cRenderHoldingGun)
	{
		delete cRenderHoldingGun;
		cRenderHoldingGun = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
	
	cMouseController = NULL;

	if (cJoystickController)
	{
		cJoystickController->Destroy();
		cJoystickController = NULL;
	}

	if (cTextRenderer)
	{
		cTextRenderer->Destroy();
		cTextRenderer = NULL;
	}

	if (cRenderToTextureShader)
	{
		delete cRenderToTextureShader;
		cRenderToTextureShader = NULL;
	}
	if (cGUISimpleShader)
	{
		delete cGUISimpleShader;
		cGUISimpleShader = NULL;
	}
	if (cGUIShader)
	{
		delete cGUIShader;
		cGUIShader = NULL;
	}
	if (groundShader)
	{
		delete groundShader;
		groundShader = NULL;
	}
	if (skyBoxShader)
	{
		delete skyBoxShader;
		skyBoxShader = NULL;
	}
	if (textShader)
	{
		delete textShader;
		textShader = NULL;
	}

	if (cSimpleShader)
	{
		delete cSimpleShader;
		cSimpleShader = NULL;
	}

	if (cShader)
	{
		delete cShader;
		cShader = NULL;
	}

	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	return true;
}

/**
@brief Update Update this instance
*/
void CScene3D::Update(const double dElapsedTime)
{

}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}


