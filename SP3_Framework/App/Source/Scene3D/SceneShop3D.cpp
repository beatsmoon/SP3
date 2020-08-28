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

	cScore = CScore::GetInstance();
	

	return true;
}

/**
@brief Update Update this instance
*/
void CSceneShop3D::Update(const double dElapsedTime)
{
	
	cShop->Update(dElapsedTime);
	cShop->SetPrices();




	//if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_P))
	//{
	//	if (cShop->GetStatus() == false)
	//	{
	//		cShop->ActivateShop();

	//		/*cSettings->bDisableMousePointer = false;
	//		cSettings->bShowMousePointer = true;
	//		!cMouseController->GetKeepMouseCentered();*/
	//	}

	//	else
	//	{
	//		cShop->DeactivateShop();

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
		

	cTextRenderer->PreRender();
	cTextRenderer->Render(to_string(cMouseController->GetMousePositionY()), 10.0f, 50.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	cTextRenderer->Render(to_string(cMouseController->GetMousePositionX()), 10.0f, 70.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	//Shows player how much score they have
	cTextRenderer->Render(std::to_string(cScore->GetScore()), cSettings->iWindowWidth * 0.55, cSettings->iWindowHeight * 0.165, 2.f, glm::vec3(1.f, 1.f, 1.f));

	//Show what item the player is trying to buy
	cTextRenderer->Render(cShop->GetItemBought(), cSettings->iWindowWidth * 0.2, cSettings->iWindowHeight * 0.1, 1.f, glm::vec3(1.f, 1.f, 1.f));

	//Show player prices of items
	//First Row
	cTextRenderer->Render(std::to_string(cShop->GetPrice(SNIPER)), cSettings->iWindowWidth * 0.13156, cSettings->iWindowHeight * 0.89, 1.f, glm::vec3(1.f, 1.f, 1.f));

	cTextRenderer->Render(std::to_string(cShop->GetPrice(SHOTGUN)), cSettings->iWindowWidth * 0.45187, cSettings->iWindowHeight * 0.89, 1.f, glm::vec3(1.f, 1.f, 1.f));

	cTextRenderer->Render(std::to_string(cShop->GetPrice(AK47)), cSettings->iWindowWidth * 0.77218, cSettings->iWindowHeight * 0.89, 1.f, glm::vec3(1.f, 1.f, 1.f));

	//Second Row
	cTextRenderer->Render(std::to_string(cShop->GetPrice(EXTMAG)), cSettings->iWindowWidth * 0.14375, cSettings->iWindowHeight * 0.49, 0.6f, glm::vec3(1.f, 1.f, 1.f));

	cTextRenderer->Render(std::to_string(cShop->GetPrice(SCOPE)), cSettings->iWindowWidth * 0.36640, cSettings->iWindowHeight * 0.49, 0.6f, glm::vec3(1.f, 1.f, 1.f));

	cTextRenderer->Render(std::to_string(cShop->GetPrice(BARREL)), cSettings->iWindowWidth * 0.59296, cSettings->iWindowHeight * 0.49, 0.6f, glm::vec3(1.f, 1.f, 1.f));

	//Third Row

	cTextRenderer->Render(std::to_string(cShop->GetPrice(AMMOITEM)), cSettings->iWindowWidth * 0.09656, cSettings->iWindowHeight * 0.23342, 0.6f, glm::vec3(1.f, 1.f, 1.f));

	cTextRenderer->PostRender();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CSceneShop3D::PostRender(void)
{
}


