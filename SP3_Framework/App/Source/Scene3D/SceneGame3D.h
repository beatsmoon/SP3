/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include TextRenderer
#include "RenderControl\TextRenderer.h"

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

// Include Mouse controller
#include "Inputs\MouseController.h"

// Include joystick controller
#include "Inputs/JoystickController.h"

// FPS Counter
#include "..\App\Source\TimeControl\FPSCounter.h"

// Include SoundController
#include "..\SoundController\SoundController.h"

// Include Player3D
#include "Player3D.h"
// Include Camera
#include "Camera.h"
// Include MiniMap
#include "Minimap.h"
// Iniclude the CameraEffects
#include "../CameraEffects/CameraEffects.h"
// Include the Progression bar
#include "../CameraEffects/ProgressBar.h"
// Include the CrossHair
#include "CrossHair/CrossHair.h"

// Include SkyBox
#include "SkyBox.h"
// Include GroundMap
#include "GroundMap.h"

// Include CEnityManager
#include "EntityManager.h"

// Include CStructure3D
#include "Structure3D.h"

// Include HUD
#include "../App/Source/CameraEffects/HUD.h"
#include "../App/Source/Scene3D/Scene3D.h"

//Include Wave Control
#include "Wave.h"

class CSettings;

class CSceneGame3D : public CScene3D, public CSingletonTemplate<CSceneGame3D>
{
	friend class CSingletonTemplate<CSceneGame3D>;
public:
	// Init
	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	// Handler to the player class
	CPlayer3D* cPlayer3D;

	//Handler to the entityManager class
	CEntityManager* cEntityManager;

	// Handler to the skybox class
	CSkyBox* cSkybox;

	// Handler to the ground class
	CGroundMap* cGroundMap;

	// Handler to the minimap class
	CMinimap* cMinimap;

	// Handler to the cCameraEffects
	CCameraEffects* cCameraEffects;

	// Handler to the cProgressBar
	CProgressBar* cHealthBar; // health

	CProgressBar* cBulletsPerMagBar; // bullet per magazine

	CProgressBar* cTotalBulletsBar; // total bullet

	// Handler to the CCrossHair
	CCrossHair* cCrossHair;

	CHUD* cHUD;

	CHUD* cRenderHoldingGun;

	//Handler to the wave control class
	CWave* cWave;

	CScore* cScore;

protected:
	// Constructor
	CSceneGame3D(void);
	// Destructor
	virtual ~CSceneGame3D(void);
	
};

