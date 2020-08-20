#include "SceneShop3D.h"
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

#include "../SceneControl/SceneManager.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CSceneShop3D::CSceneShop3D(void)
{
}

/**
 @brief Destructor
 */
CSceneShop3D::~CSceneShop3D(void)
{
	
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CSceneShop3D::Init(void)
{
	if (!CScene3D::Init())
		return false;

	cShop = CShop::GetInstance();
	cShop->SetShader(cGUIShader);

	if (cShop->Init() == false)
		return false;

	cMouseController->PostUpdate();
	

	return true;
}

/**
@brief Update Update this instance
*/
void CSceneShop3D::Update(const double dElapsedTime)
{
	cShop->Update(dElapsedTime);

	//if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_P))
	//{
	//	static double InputDelay = 5.f;
	//	if (InputDelay < 5.f)
	//	{
	//		InputDelay += 1.f;

	//	}

	//	else
	//	{
	//		InputDelay = 0.f;
	//		if (cShop->GetStatus() == false)
	//		{
	//			cShop->ActivateShop();
	//			cSettings->SetMousePointer(false, true);
	//			/*cSettings->bDisableMousePointer = false;
	//			cSettings->bShowMousePointer = true;
	//			!cMouseController->GetKeepMouseCentered();*/
	//		}

	//		else
	//		{
	//			cShop->DeactivateShop();
	//			cSettings->SetMousePointer(true, false);
	//		}
	//	}
	//}

	
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CSceneShop3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	//glLoadIdentity();

	// Clear the screen and buffer
	/*glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
}

/**
 @brief Render Render this instance
 */
void CSceneShop3D::Render(void)
{
	cShop->PreRender();
	cShop->Render();
	cShop->PostRender();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CSceneShop3D::PostRender(void)
{
}


