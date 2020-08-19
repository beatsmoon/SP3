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

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CSceneMenu3D::CSceneMenu3D(void)
	
{
}

/**
 @brief Destructor
 */
CSceneMenu3D::~CSceneMenu3D(void)
{
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CSceneMenu3D::Init(void)
{
	if (!CScene3D::Init())
		return false;

	return true;
}

/**
@brief Update Update this instance
*/
void CSceneMenu3D::Update(const double dElapsedTime)
{
	
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
	
	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(	glm::radians(CCamera::GetInstance()->fZoom),
												(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
												0.1f, 1000.0f);

	/*glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Render cSkybox
	/*cSkybox->SetView(view);
	cSkybox->SetProjection(projection);
	cSkybox->PreRender();
	cSkybox->Render();
	cSkybox->PostRender();*/

	
	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test
	

	//cCrossHair->PreRender();
	//cCrossHair->Render();
	//cCrossHair->PostRender();

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
	cTextRenderer->Render(glm::to_string(cCamera->vec3Position), 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	cTextRenderer->Render("MENU", 10.0f, 15.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

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


