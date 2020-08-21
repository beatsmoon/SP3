#include "SceneMenu3D.h"
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
#include "../Meshes/MeshBuilder.h"

#include "../SceneControl/SceneManager.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CSceneMenu3D::CSceneMenu3D(void)
	: cPlayer3D(NULL)
	, cEntityManager(NULL)
	, cSkybox(NULL)
	, cPistol(NULL)
{
}

/**
 @brief Destructor
 */
CSceneMenu3D::~CSceneMenu3D(void)
{
	if (cPistol)
	{
		delete cPistol;
		cPistol = NULL;
	}
	
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CSceneMenu3D::Init(void)
{
	if (!CScene3D::Init())
		return false;

	//cCamera = CCamera::GetInstance();
	//cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	cPlayer3D = CPlayer3D::GetInstance();

	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->Init();

	CStructure3D* cButton = NULL;
	float fYButtonOffset = cPlayer3D->GetPosition().y;
	float fRelativeScale = 0.5f;

	for (unsigned int i = 0; i < M_END; ++i)
	{
		cButton = new CStructure3D(glm::vec3(cPlayer3D->GetPosition().x, fYButtonOffset + fRelativeScale, cPlayer3D->GetPosition().z - 2.f), static_cast<CEntity3D::TYPE>(i + static_cast<int>(CEntity3D::TYPE::BUTTON_START) + 1), glm::vec3(0.f, 0.f, 1.f));
		cButton->SetShader(cShader);
		cButton->Init();
		cButton->ActivateCollider(cSimpleShader);
		vStructures.push_back(cButton);

		fYButtonOffset -= fRelativeScale;
	}

	// add weapon to player
	cPistol = new CWeapon(Weapon_Type::W_PISTOL);
	cPistol->Init();
	cPistol->SetShader(cSimpleShader);
	cPlayer3D->SetWeapon(0, cPistol);
	
	cSkybox = CSkyBox::GetInstance();
	cSkybox->SetShader(skyBoxShader);
	cSkybox->Init();

	return true;
}

/**
@brief Update Update this instance
*/
void CSceneMenu3D::Update(const double dElapsedTime)
{
	if (cPlayer3D->IsCameraAttached())
	{
		// Get Mouse updates
		cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(),
			(float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(
			CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB))
	{
		CProjectile* cProjectile = cPlayer3D->DischargeWeapon();
		if (cProjectile)
		{
			cEntityManager->Add(cProjectile);
		}
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_R))
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
			}
			else
			{
				cout << "there is no weapon to reload" << endl;
			}
		}

	}

	cMouseController->PostUpdate();

	// Update cEntityManager
	cEntityManager->Update(dElapsedTime);

	for (size_t i = 0; i < vStructures.size(); ++i)
	{
		if (cEntityManager->CollisionCheck(vStructures.at(i)))
		{
			switch (i)
			{
			case MENU_CHOICES::M_PLAY:
				CSceneManager::GetInstance()->EnableScene(SCENES::GAME);
				CSceneManager::GetInstance()->DisableScene(SCENES::MENU);
				break;
			case MENU_CHOICES::M_HIGHSCORE:
				// TODO: Render highscore screen
				break;
			case MENU_CHOICES::M_QUIT:
				// TODO: Quit game with smooth transition
				CSceneManager::GetInstance()->SetApplicationToEnd();
				break;
			}
		}
	}
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CSceneMenu3D::PreRender(void)
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
void CSceneMenu3D::Render(void)
{
	//if (cPlayer3D->GetScopeMode() == false)
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

		glEnable(GL_DEPTH_TEST);

		// Render cPlayer3D 
		cPlayer3D->SetView(playerView);
		cPlayer3D->SetProjection(playerProjection);
		cPlayer3D->PreRender();
		cPlayer3D->Render();
		cPlayer3D->PostRender();
	}
	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(glm::radians(CCamera::GetInstance()->fZoom),
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

	CStructure3D* tempStructure = NULL;
	for (size_t i = 0; i < M_END; ++i)
	{
		tempStructure = vStructures.at(i);
		tempStructure->SetView(view);
		tempStructure->SetProjection(projection);
		tempStructure->Render();
	}

	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	// Call the cTextRenderer's PreRender()
	cTextRenderer->PreRender();

	// Call the CTextRenderer's Render()
	textShader->use();
	//cTextRenderer->Render("DM2231 GDEV 2D", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render FPS info
	//	cTextRenderer->Render(cFPSCounter->GetFrameRateString(), 10.0f, 580.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	// Render Camera Position
	//cTextRenderer->Render(glm::to_string(cPlayer3D->GetPosition()), 10.0f, 30.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	// Render Camera Position
	cTextRenderer->Render("MENU", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	// Call the cTextRenderer's PostRender()
	cTextRenderer->PostRender();


	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CSceneMenu3D::PostRender(void)
{
}


