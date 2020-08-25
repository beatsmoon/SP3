#include "SceneGame3D.h"
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
#include "Entity/CBoss3D.h"
#include "Entity/RangeIndicator.h"

// Include MyMath.h
#include "../Library/Source/System/MyMath.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CSceneGame3D::CSceneGame3D(void)
	: cMinimap(NULL)
	, cCameraEffects(NULL)
	, cHealthBar(NULL)
	, cBulletsPerMagBar(NULL)
	, cTotalBulletsBar(NULL)
	, cCrossHair(NULL)
	, cHUD(NULL)
	, cWEAPONHUD(NULL)
	, cRenderHoldingGun(NULL)
	, cPlayer3D(NULL)
	, cEntityManager(NULL)
	, cSkybox(NULL)
	, cGroundMap(NULL)
	, cWave(NULL)
	, cScore(NULL)
{
}

/**
 @brief Destructor
 */
CSceneGame3D::~CSceneGame3D(void)
{
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
	if (cWEAPONHUD)
	{
		delete cWEAPONHUD;
		cWEAPONHUD = NULL;
	}

	if (cRenderHoldingGun)
	{
		delete cRenderHoldingGun;
		cRenderHoldingGun = NULL;
	}

	if (cHealthBar)
	{
		delete cHealthBar;
		cHealthBar = NULL;
	}

	if (cBulletsPerMagBar)
	{
		delete cBulletsPerMagBar;
		cBulletsPerMagBar = NULL;
	}

	if (cTotalBulletsBar)
	{
		delete cTotalBulletsBar;
		cTotalBulletsBar = NULL;
	}

	if (cCrossHair)
	{
		cCrossHair->Destroy();
		cCrossHair = NULL;
	}

	// Destroy the cGroundMap
	if (cGroundMap)
	{
		cGroundMap->Destroy();
		cGroundMap = NULL;
	}

	if (cCameraEffects)
	{
		cCameraEffects->Destroy();
		cCameraEffects = NULL;
	}

	if (cMinimap)
	{
		cMinimap->Destroy();
		cMinimap = NULL;
	}

	if (cWave)
	{
		cWave->Destroy();
		cWave = NULL;
	}

	if (cScore)
	{
		cScore->Destroy();
		cScore = NULL;
	}
}


/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CSceneGame3D::Init(void)
{
	if (!CScene3D::Init())
		return false;

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

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();

	//Creating Attachment
	CGunScope* cGunScope = new CGunScope();
	cGunScope->SetShader(cShader);
	cGunScope->Init();
	cGunScope->SetTierLevel(CGunScope::TIERLEVEL::TIER1);

	// add weapon to player
	CWeapon* cPistol = new CWeapon(Weapon_Type::W_PISTOL);
	cPistol->Init();
	cPistol->SetShader(cSimpleShader);
	cPlayer3D->SetWeapon(1, cPistol);
	cPlayer3D->SetCurrentWeapon(1);
	cPlayer3D->GetWeapon()->SetScope(cGunScope);

	//Creating Attachment
	CGunBarrel* cGunBarrel = new CGunBarrel();
	cGunBarrel->SetShader(cShader);
	cGunBarrel->Init();
	cGunBarrel->SetTierLevel(CGunBarrel::TIERLEVEL::TIER1);

	CGunExtMag* cGunExtMag = new CGunExtMag();
	cGunExtMag->SetShader(cShader);
	cGunExtMag->Init();
	cGunExtMag->SetTierLevel(CGunExtMag::TIERLEVEL::TIER3);

	cGunScope = new CGunScope();
	cGunScope->SetShader(cShader);
	cGunScope->Init();
	cGunScope->SetTierLevel(CGunScope::TIERLEVEL::TIER3);

	CWeapon* rifle = new CWeapon(Weapon_Type::W_SHOTGUN);
	rifle->Init();
	rifle->SetShader(cSimpleShader);
	cPlayer3D->SetWeapon(0, rifle);
	cPlayer3D->SetCurrentWeapon(0);


	cPlayer3D->GetWeapon()->SetBarrel(cGunBarrel);
	cPlayer3D->GetWeapon()->SetExtMag(cGunExtMag);
	cPlayer3D->GetWeapon()->SetScope(cGunScope);



	// attach camera
	cPlayer3D->AttachCamera(cCamera);

	// Intialise the CentityManager
	cEntityManager = CEntityManager::GetInstance();
	//cEntityManager->Init();

	//Initialise cWave
	cWave = CWave::GetInstance();
	cWave->Init();

	cScore = CScore::GetInstance();
	cScore->Init();

	//Load the Skybox
	// No need to initialise cSkybox as it is already initialised in SceneMenu3D
	cSkybox = CSkyBox::GetInstance();

	// Load the ground
	cGroundMap = CGroundMap::GetInstance();
	cGroundMap->SetShader(groundShader);
	cGroundMap->Init(glm::vec3(50.0f, 1.0f, 50.0f), glm::i32vec3(10, 1, 10));
	cGroundMap->ReCalculate();

	// Initialise Minimap
	cMinimap = CMinimap::GetInstance();
	cMinimap->SetShader(cRenderToTextureShader);
	cMinimap->Init();

	// Initialise cameraEffects
	cCameraEffects = CCameraEffects::GetInstance();
	cCameraEffects->SetShader(cGUIShader);
	cCameraEffects->Init();

	// Initialise progressBar
	cHealthBar = new CProgressBar(cPlayer3D->GetCurrHealth(), cPlayer3D->GetMaxHealth(), Progress_Bar_type::P_HEALTH);
	cHealthBar->SetShader(cGUISimpleShader);
	cHealthBar->Init();

	cBulletsPerMagBar = new CProgressBar(cPlayer3D->GetWeapon()->GetMagRound(), cPlayer3D->GetWeapon()->GetMaxMagRound(), Progress_Bar_type::P_BULLET_PER_MAG);
	cBulletsPerMagBar->SetShader(cGUISimpleShader);
	cBulletsPerMagBar->Init();

	cTotalBulletsBar = new CProgressBar(cPlayer3D->GetWeapon()->GetTotalRound(), cPlayer3D->GetWeapon()->GetMaxTotalRound(), Progress_Bar_type::P_BULLET_TOTAL);
	cTotalBulletsBar->SetShader(cGUISimpleShader);
	cTotalBulletsBar->Init();

	// Initialise cCrossHair
	//cCrossHair = CCrossHair::GetInstance();
	//cCrossHair->SetShader(cGUIShader);
	//cCrossHair->Init();

	// Initialise cScope
	cScope = CScope::GetInstance();
	cScope->SetShader(cGUIShader);
	cScope->Init();

	// Initialise HUD
	cHUD = new CHUD(Weapon_Type::W_PISTOL);
	cHUD->SetShader(cGUIShader);
	cHUD->Init();

	cWEAPONHUD = new CWEAPONHUD(Weapon_Type::W_PISTOL);
	cWEAPONHUD->SetShader(cGUIShader);
	cWEAPONHUD->Init();

	cRenderHoldingGun = new CHUD(Weapon_Type::W_PISTOL);
	cRenderHoldingGun->SetShader(cGUIShader);
	cRenderHoldingGun->Init();

	for (int i = 0; i < 3; i++)
	{
		/*int k = Math::RandIntMinMax(0, 2);

		CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f)), 2);
		cEnemy3D->SetShader(cShader);
		cEnemy3D->Init();
		cEnemy3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cEnemy3D);

		CRangeIndicator* cRangeIndicator = new CRangeIndicator(cEnemy3D->GetPosition(), 2, cEnemy3D);
		cRangeIndicator->SetShader(cShader);
		cRangeIndicator->Init();
		cRangeIndicator->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cRangeIndicator);*/

	/*	CStructure3D* rifleAmmo = new CStructure3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f))
			, CEntity3D::TYPE::RIFLE_AMMO);
		rifleAmmo->SetShader(cShader);
		rifleAmmo->Init();
		rifleAmmo->ActivateCollider(cSimpleShader);
		cEntityManager->Add(rifleAmmo);

		CStructure3D* pistolAmmo = new CStructure3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f))
			, CEntity3D::TYPE::PISTOL_AMMO);
		pistolAmmo->SetShader(cShader);
		pistolAmmo->Init();
		pistolAmmo->ActivateCollider(cSimpleShader);
		cEntityManager->Add(pistolAmmo);*/


	}

	/*CStructure3D* cExplosiveBarrel = new CStructure3D(glm::vec3(0.f, 0.5f, 0.f), CEntity3D::TYPE::EXPLOSIVE_BARREL);
	cExplosiveBarrel->SetShader(cShader);
	cExplosiveBarrel->Init();
	cExplosiveBarrel->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cExplosiveBarrel);*/


	/*CBoss3D* cEnemy3D = new CBoss3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f)), 2);
	cEnemy3D->SetShader(cShader);
	cEnemy3D->Init();
	cEnemy3D->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cEnemy3D);*/

	/*CStructure3D* cBarricade = new CStructure3D(glm::vec3(10.f, 0.5f, 10.f), CEntity3D::TYPE::BARRICADE);
	cBarricade->SetShader(cShader);
	cBarricade->Init();
	cBarricade->ActivateCollider(cSimpleShader);
	cEntityManager->Add(cBarricade);*/


	return true;
}

/**
@brief Update Update this instance
*/
void CSceneGame3D::Update(const double dElapsedTime)
{
	// respawn player
	if (cPlayer3D->GetCurrHealth() < 1)
	{
		cPlayer3D->SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
		cPlayer3D->SetCurrHealth(100);
	}

	// Weapon interaction
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_1))
	{
		static double InputDelay = 5.f;
		if (InputDelay < 5.f)
		{
			InputDelay += 1.f;

		}
		else
		{
			InputDelay = 0.f;
			cSoundController->PlaySoundByID(8);
			cPlayer3D->SetCurrentWeapon(0);
		}
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_2))
	{
		static double InputDelay = 5.f;
		if (InputDelay < 5.f)
		{
			InputDelay += 1.f;

		}
		else
		{
			InputDelay = 0.f;
			cSoundController->PlaySoundByID(8);
			cPlayer3D->SetCurrentWeapon(1);
		}
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_R) 
		&& cPlayer3D->GetWeapon()->GetMagRound() < cPlayer3D->GetWeapon()->GetMaxMagRound())
	{	
		static double InputDelay = 10.f;
		if (InputDelay < 10.f)
		{
			InputDelay += 1.f;

		}
		else
		{
			InputDelay = 0.f;
			if (cPlayer3D->GetWeapon() != NULL)
			{
				cPlayer3D->GetWeapon()->Reload();
				//cPlayer3D->GetWeapon()->SetCanFire(false);
				
			}
			else
			{
				cout << "there is no weapon to reload" << endl;
			}
		}

		if (cPlayer3D->GetWeapon()->GetWeaponName() == Weapon_Type::W_PISTOL)
		{
			cPlayer3D->GetWeapon()->SetTotalRound(cPlayer3D->GetWeapon()->GetMaxTotalRound());
		}

	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_B))
	{
		static double InputDelay = 10.f;
		if (InputDelay < 10.f)
		{
			InputDelay += 1.f;

		}
		else
		{
			InputDelay = 0.f;
			cSoundController->PlaySoundByID(9);
			if (cPlayer3D->GetShootingMode())
			{
				cPlayer3D->SetShootingMode(0);
			}
			else
			{
				cPlayer3D->SetShootingMode(1);
			}
		}
	}

	static double WaveTimer;
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_O) && cEntityManager->GetWaveStarted() == false)
	{
		cout << cWave->GetWaveNumber() << endl;
		cWave->SetWave(cWave->GetWaveNumber());
		WaveTimer = 0.0f;
	}

	// update the joystick
	cJoystickController->Update(cJoystickController->GetJoystickID());

	// Player's movement
	if (CKeyboardController::GetInstance()->IsKeyPressed(cSettings->keys[CSettings::FORWARD].key_value))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::FORWARD, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(cSettings->keys[CSettings::BACKWARD].key_value))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::BACKWARD, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(cSettings->keys[CSettings::LEFT].key_value))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::LEFT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(cSettings->keys[CSettings::RIGHT].key_value))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::RIGHT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_W) && CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::FORWARD_RUNNING, (float)dElapsedTime * 1.5f);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE) || (cJoystickController->IsButtonDown(0)))
	{
		cPlayer3D->SetToJump();
	}

	cPlayer3D->StorePositionForRollback();

	// Attach or Dettach camera
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		if (cPlayer3D->IsCameraAttached())
		{
			cPlayer3D->AttachCamera();
		}
		else
		{
			cPlayer3D->AttachCamera(cCamera);
		}
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
	}

	// camera movement
	if (!cPlayer3D->IsCameraAttached())
	{
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_I))
		{
			cCamera->ProcessKeyboard(CCamera::FORWARD, (float)dElapsedTime);
		}
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_K))
		{
			cCamera->ProcessKeyboard(CCamera::BACKWARD, (float)dElapsedTime);
		}
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_J))
		{
			cCamera->ProcessKeyboard(CCamera::LEFT, (float)dElapsedTime);
		}
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_L))
		{
			cCamera->ProcessKeyboard(CCamera::RIGHT, (float)dElapsedTime);
		}
	}

	

	if (cPlayer3D->IsCameraAttached())
	{
		// Get Mouse updates
		cPlayer3D->ProcessRotate(	(float)cMouseController->GetMouseDeltaX(),
									(float)cMouseController->GetMouseDeltaY());

		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));

		if (cPlayer3D->GetWeapon()->GetScope() != NULL)
		{
			if (CMouseController::GetInstance()->IsButtonReleased(CMouseController::BUTTON_TYPE::RMB))
			{
				cPlayer3D->SetScopeMode(false);
				cCamera->fZoom = 45.0f;
			}
			else if (CMouseController::GetInstance()->IsButtonPressed(CMouseController::BUTTON_TYPE::RMB))
			{

				cPlayer3D->SetScopeMode(true);

				if (cPlayer3D->GetWeapon()->GetScope()->GetTierLevel() == CGunScope::TIERLEVEL::TIER1)
				{
					cCamera->fZoom = 35.0f;
				}
				if (cPlayer3D->GetWeapon()->GetScope()->GetTierLevel() == CGunScope::TIERLEVEL::TIER2)
				{
					cCamera->fZoom = 15.0f;
				}
				if (cPlayer3D->GetWeapon()->GetScope()->GetTierLevel() == CGunScope::TIERLEVEL::TIER3)
				{
					cCamera->fZoom = 5.0f;
				}
			}
		}
	}
	else
	{
		// Get Mouse updates
		cCamera->ProcessMouseMovement(	(float)cMouseController->GetMouseDeltaX(),
										(float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll(	(float)cMouseController->GetMouseScrollStatus(
											CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && cPlayer3D->GetWeapon()->GetMagRound() > 0
		&& cPlayer3D->GetWeapon()->GetFiringType() == CWeaponInfo::FIRINGTYPE::AUTO && cPlayer3D->GetWeapon()->GetCanFire()) 
	{
		int BulletPerShot = 1;
		if (cPlayer3D->GetWeapon()->GetWeaponName() == Weapon_Type::W_SHOTGUN)
		{
			BulletPerShot = 8;
		}
		for (int i = 0; i < BulletPerShot; ++i)
		{
			CProjectile* cProjectile = cPlayer3D->DischargeWeapon();
			if (cProjectile)
			{

				cProjectile->SetGravityMultiplier(cPlayer3D->GetWeapon()->CalculateGravityMultiplier());
				
				cEntityManager->Add(cProjectile);
				cPlayer3D->TriggerRecoil();
			}
			if (i == BulletPerShot - 1)
			{
				cPlayer3D->GetWeapon()->SetMagRound((cPlayer3D->GetWeapon()->GetMagRound()) - 1);
				cPlayer3D->GetWeapon()->SetCanFire(false);
			}
		}
	}
	else if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::LMB) && cPlayer3D->GetWeapon()->GetMagRound() > 0
		&& cPlayer3D->GetWeapon()->GetFiringType() == CWeaponInfo::FIRINGTYPE::SINGLE)
	{
		//CProjectile* cProjectile = cPlayer3D->DischargeWeapon();
		//if (cProjectile)
		//{
		//	cEntityManager->Add(cProjectile);
		//}
		int BulletPerShot = 1;
		if (cPlayer3D->GetWeapon()->GetWeaponName() == Weapon_Type::W_SHOTGUN)
		{
			BulletPerShot = 8;
		}
		for (int i = 0; i < BulletPerShot; ++i)
		{
			CProjectile* cProjectile = cPlayer3D->DischargeWeapon();
			if (cProjectile)
			{
				cProjectile->SetGravityMultiplier(cPlayer3D->GetWeapon()->CalculateGravityMultiplier());
				cEntityManager->Add(cProjectile);
				cPlayer3D->TriggerRecoil();
			}
			if (i == BulletPerShot - 1)
			{
				cPlayer3D->GetWeapon()->SetMagRound((cPlayer3D->GetWeapon()->GetMagRound()) - 1);
				cPlayer3D->GetWeapon()->SetCanFire(false);
			}
		}
	}
	
	


	// Post Update Camera
	cMouseController->PostUpdate();


	// Update cEntityManager
	cEntityManager->Update(dElapsedTime);
	
	//Check if wave ended
	//If ended, update score and next wave
	if (cEntityManager->CheckWave() == true)
	{
		cout << "Wave over" << endl;
		cout << "Time Taken: " << WaveTimer << endl;
		//Calculate Score from completing wave
		cScore->AddScoreFromWave(cWave->GetWaveNumber(), WaveTimer);

		cout << "Total score is: " << cScore->GetScore() << endl;

		//Set the wave number for next wave
		cWave->SetWaveNumber(cWave->GetWaveNumber() + 1);

		cWave->UpdateHighScore();

		//Set the score player will get from killing each enemy
		cScore->SetScoreToAdd(cWave->GetWaveNumber() * 100);

		cout << "Next wave is wave: " << cWave->GetWaveNumber() << endl;
		cout << "Score per enemy is: " << cScore->GetScoreToAdd() << endl;
	
	}
	
	//While wave is ongoing
	else if(cEntityManager->CheckWave() == false)
	{
		//Update score while while is ongoing
		cEntityManager->UpdateScore();
		//Timer for wave
		WaveTimer += dElapsedTime;
	}
		
	

	// collision between player and entity
	if (cEntityManager->CollisionCheck(cPlayer3D))
	{
	}

	// clean up the deleted centity3d in the entity manager
	cEntityManager->CleanUp();

	// Update cPlayer3D
	cPlayer3D->Update(dElapsedTime);

	// update camera effects
	cCameraEffects->Update(dElapsedTime);

	// update progressBar
	cHealthBar->Update(dElapsedTime);
	cBulletsPerMagBar->Update(dElapsedTime);
	cTotalBulletsBar->Update(dElapsedTime);

	cRenderHoldingGun->Update(dElapsedTime);
	cHUD->Update(dElapsedTime);
	cWEAPONHUD->Update(dElapsedTime);

	if (cPlayer3D->GetWeapon()->GetScope() != NULL && cPlayer3D->GetScopeMode() == true)
	{
		cScope->UpdateTierListOfScope(cPlayer3D->GetWeapon()->GetScope()->GetTierLevel());
		//cScope->UpdateTierListOfScope(cPlayer3D->GetWeapon()->GetScope()->GetTierLevel());
		cScope->Update(dElapsedTime);
	}

	

	cScore->Update();
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CSceneGame3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Render Render this instance
 */
void CSceneGame3D::Render(void)
{
	if (cPlayer3D->GetScopeMode() == false)
	{
		// Part 1: Render for the minimap by binding to framebuffer and render to the color texture
	//         But the camera is move to the top-view of the scene

	// Backup some key settings for the camera and player
		glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
		float storeCameraYaw = cCamera->fYaw;
		float storeCameraPitch = cCamera->fPitch;
		glm::vec3 storeCameraPosition = cCamera->vec3Position;
		glm::vec3 storeCameraUp = cCamera->vec3Up;
		glm::vec3 storeCameraRight = cCamera->vec3Right;

		// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
		cCamera->fYaw += 180.f;
		cCamera->fPitch = -90.0f;

		// we store the player's position into the camera as we want the minimap to focus on the player
		cCamera->vec3Position = glm::vec3(storePlayerPosition.x, 10.0f, storePlayerPosition.z);

		// Recalculate all the camera vectors
		// we disable pitch constrains for this specific case as we want the camera to look straight down
		cCamera->ProcessMouseMovement(0, 0, false);

		// Generate the view and projection 
		glm::mat4 playerView = cCamera->GetViewMatrix();
		glm::mat4 playerProjection = glm::perspective(glm::radians(45.0f),
			(float)cSettings->iWindowWidth /
			(float)cSettings->iWindowHeight,
			0.1f, 1000.0f);

		// set the view and projection
		cCamera->fYaw = storeCameraYaw;
		cCamera->fPitch = storeCameraPitch;
		cCamera->vec3Position = storeCameraPosition;
		// Call this to make sure it updates its camera vectors, note that we disable pitch constraints
		// for this specific case (otherwise we can't reverse camera's pitch values)
		cCamera->ProcessMouseMovement(0, 0, false);
		cCamera->vec3Up = storeCameraUp;
		cCamera->vec3Right = storeCameraRight;

		// active
		cMinimap->Activate();
		// setup the rendering environment
		cMinimap->PreRender();

		glEnable(GL_DEPTH_TEST);

		// Render cGroundMap 
		cGroundMap->SetView(playerView);
		cGroundMap->SetProjection(playerProjection);
		cGroundMap->PreRender();
		cGroundMap->Render();
		cGroundMap->PostRender();

		// Render cPlayer3D 
		cPlayer3D->SetView(playerView);
		cPlayer3D->SetProjection(playerProjection);
		cPlayer3D->PreRender();
		cPlayer3D->Render();
		cPlayer3D->PostRender();

		cEntityManager->SetView(playerView);
		cEntityManager->SetProjection(playerProjection);
		cEntityManager->Render();

		// deactivate
		cMinimap->Deactivate();
	}
	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(	glm::radians(CCamera::GetInstance()->fZoom),
												(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
												0.1f, 1000.0f);

	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Render cSkybox
	cSkybox->SetView(view);
	cSkybox->SetProjection(projection);
	cSkybox->PreRender();
	cSkybox->Render();
	cSkybox->PostRender();

	// Render cGroundMap 
	cGroundMap->SetView(view);
	cGroundMap->SetProjection(projection);
	cGroundMap->PreRender();
	cGroundMap->Render();
	cGroundMap->PostRender();

	// Render cPlayer3D 
	cPlayer3D->SetView(view);
	cPlayer3D->SetProjection(projection);
	cPlayer3D->PreRender();
	cPlayer3D->Render();
	cPlayer3D->PostRender();

	// Render cEntityManager
	cEntityManager->SetView(view);
	cEntityManager->SetProjection(projection);
	cEntityManager->Render();

	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	cCameraEffects->PreRender();
	cCameraEffects->Render();
	cCameraEffects->PostRender();

	if (cPlayer3D->GetScopeMode() == false)
	{
		cHealthBar->PreRender();
		cHealthBar->Render();
		cHealthBar->PostRender();

		//cBulletsPerMagBar->PreRender();
		//cBulletsPerMagBar->Render();
		//cBulletsPerMagBar->PostRender();

		//cTotalBulletsBar->PreRender();
		//cTotalBulletsBar->Render();
		//cTotalBulletsBar->PostRender();

		cRenderHoldingGun->PreRender();
		cRenderHoldingGun->Render();
		cRenderHoldingGun->PostRender();

		cHUD->PreRender();
		cHUD->Render();
		cHUD->PostRender();

		cWEAPONHUD->PreRender();
		cWEAPONHUD->Render();
		cWEAPONHUD->PostRender();
	}
	

	//cCrossHair->PreRender();
	//cCrossHair->Render();
	//cCrossHair->PostRender();

	if (cPlayer3D->GetWeapon()->GetScope() != NULL && cPlayer3D->GetScopeMode() == true)
	{
		cScope->PreRender();
		cScope->Render();
		cScope->PostRender();
	}
	// Call the cTextRenderer's PreRender()
	cTextRenderer->PreRender();

	// Call the CTextRenderer's Render()
	textShader->use();
	//cTextRenderer->Render("DM2231 GDEV 2D", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render FPS info
	cTextRenderer->Render(cFPSCounter->GetFrameRateString(), 10.0f, 50.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render Camera Position
	//cTextRenderer->Render(glm::to_string(cPlayer3D->GetPosition()), 10.0f, 30.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	
	// Render Player Position
	cTextRenderer->Render(glm::to_string(cPlayer3D->GetPosition()),
						  10.0f, 30.0f, 0.5f,
					  	  glm::vec3(1.0f, 1.0f, 1.0f));
	   
	// Render Camera Position
	cTextRenderer->Render(glm::to_string(cCamera->vec3Position), 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	if (cPlayer3D->GetWeapon() != NULL)
	{
		cTextRenderer->Render(to_string(cPlayer3D->GetWeapon()->GetMagRound()), 10, 60.0f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		cTextRenderer->Render("/", 60, 60.0f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		cTextRenderer->Render(to_string(cPlayer3D->GetWeapon()->GetTotalRound()), 100, 60.0f, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		if (cPlayer3D->GetWeapon()->GetIsReloadStatus())
		{
			cTextRenderer->Render("Reloading :", 10, 150.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
			cTextRenderer->Render(to_string(cPlayer3D->GetWeapon()->GetReloadTime() - cPlayer3D->GetWeapon()->GetReloadElapsed()), 10, 110.0f, 1.f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}

	// Call the cTextRenderer's PostRender()
	cTextRenderer->PostRender();

	if (cPlayer3D->GetScopeMode() == false)
	{
		// call the cTextRenderer's Postrender()
		cMinimap->Render();
	}

	
	
	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CSceneGame3D::PostRender(void)
{
}


