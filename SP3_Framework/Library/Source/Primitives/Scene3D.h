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


class CSettings;

class CScene3D
{
	//friend CSingletonTemplate<CScene3D>;
public:
	// Init
	virtual bool Init(void) = 0;

	// Update
	virtual void Update(const double dElapsedTime) = 0;

	// PreRender
	virtual void PreRender(void) = 0;

	// Render
	virtual void Render(void) = 0;

	// PostRender
	virtual void PostRender(void) = 0;

	// Handler to the Shader Program instance
	Shader* cShader;
	// Handler to the LineShader Program instance
	Shader* cSimpleShader;
	// Handler to the Text Shader Program instance
	Shader* textShader;
	// Handler to the SkyBox ShaderProgram instance
	Shader* skyBoxShader;
	// Handler to the groundShader Shader Program instance
	Shader* groundShader;
	// Handler to the GUIShader Shader Program instance
	Shader* cGUIShader;
	// Handler to the GUISimpleShader Shader Program instance
	Shader* cGUISimpleShader;
	// Handler to the RenderToTexture Shader Program instance
	Shader* cRenderToTextureShader;

protected:
	// The handler to the CSettings
	CSettings* cSettings;

	// The handler containing the instance of the CTextRenderer
	CTextRenderer* cTextRenderer;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	// Mouse Controller singleton instance
	CMouseController* cMouseController;

	// Joystick Controller singleton instance
	CJoystickController* cJoystickController;

	// the handler to CSoundController
	CSoundController* cSoundController;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// FPS Control
	CFPSCounter* cFPSCounter;

	// Handler to the CSoundController
	//CSoundController* cSoundController;

	// Handler to the camera class
	CCamera* cCamera;

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

public:
	// Constructor
	CScene3D(void);
	// Destructor
	virtual ~CScene3D(void);
};

