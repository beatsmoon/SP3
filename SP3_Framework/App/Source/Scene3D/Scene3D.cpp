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
#include "Entity/Enemy3D.h"

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

bool CScene3D::Init(void)
{
	cSettings = CSettings::GetInstance();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Setup the shaders
	cShader = new Shader("Shader//Scene3D.vs", "Shader//Scene3D.fs");
	// Setup the shaders
	cSimpleShader = new Shader("Shader//SimpleShader.vs", "Shader//SimpleShader.fs");
	// Setup the shaders
	textShader = new Shader("Shader//text.vs", "Shader//text.fs");
	// Setup the shaders
	skyBoxShader = new Shader("Shader//SkyBox.vs", "Shader//SkyBox.fs");
	// Setup the shaders
	groundShader = new Shader("Shader//Ground.vs", "Shader//Ground.fs");
	// set up the shader
	cRenderToTextureShader = new Shader("Shader//RenderToTexture.vs", "Shader//RenderToTexture.fs");
	// set up the shader
	cGUIShader = new Shader("Shader//GUIShader.vs", "Shader//GUIShader.fs");
	// set up the shader
	cGUISimpleShader = new Shader("Shader//GUISimpleShader.vs", "Shader//GUISimpleShader.fs");
	
	bSceneChangeEnabled = true;
	bSceneChangeDelay = false;
	dSceneChangeDelay = 0.0;

	// Configure the camera
	cCamera = CCamera::GetInstance();
	//cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the joystick controller singleton instance here
	cJoystickController = CJoystickController::GetInstance();
	// init joystick controller
	cJoystickController->Init();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	Math::InitRNG();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\enemy_idle.ogg"), 4, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\enemy_hit.ogg"), 5, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\gunshot.ogg"), 6, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\gunreload.ogg"), 7, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\switchgun.ogg"), 8, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\click.ogg"), 9, true);

	for (int i = 0; i < 9; ++i)
	{
		cSoundController->MasterVolumeDecrease();
	}

	// Create and initialise the TextRenderer
	cTextRenderer = CTextRenderer::GetInstance();
	// Set a shader to this class
	cTextRenderer->SetShader(textShader);
	// Initialise the instance
	if (cTextRenderer->Init() == false)
	{
		cout << "Failed to load CTextRenderer" << endl;
		return false;
	}


	return true;
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
	, cCamera(NULL)
	, bSceneEnabled(false)
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

	// Destroy the cSoundController
	if (cSoundController)
	{
		cSoundController->Destroy();
		cSoundController = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	if (cKeyboardController)
	{
		cKeyboardController->Destroy();
		cKeyboardController = NULL;
	}

	if (cMouseController)
	{
		cMouseController->Destroy();
		cMouseController = NULL;
	}

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

void CScene3D::EnableScene()
{
	bSceneEnabled = true;
}

void CScene3D::DisableScene()
{
	bSceneEnabled = false;
}

bool CScene3D::GetSceneStatus() const
{
	return bSceneEnabled;
}

bool CScene3D::UpdateSceneDelay(const double dElapsedTime)
{
	// Update scene delay time
	dSceneChangeDelay += dElapsedTime;
	
	// Scene change animations, events can all be updated here

	// Check if the delay is more than a constant value
	if (dSceneChangeDelay > MAX_DELAY_TIME)
	{
		bSceneChangeDelay = false;
		bSceneChangeEnabled = true;
		dSceneChangeDelay = 0.0;
		return true;
	}

	return false;
}
