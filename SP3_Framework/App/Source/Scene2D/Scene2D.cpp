#include "Scene2D.h"
#include <iostream>
using namespace std;

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene2D::CScene2D(void)
	: cShader(NULL)
	, cTextRenderer(NULL)
	, cMap2D(NULL)
	, cKeyboardController(NULL)
	, cFPSCounter(NULL)
{
}

/**
 @brief Destructor
 */
CScene2D::~CScene2D(void)
{
	if (cTextRenderer)
	{
		cTextRenderer->Destroy();
		cTextRenderer = NULL;
	}

	if (cShader)
	{
		delete cShader;
		cShader = NULL;
	}

	if (cMap2D)
	{
		cMap2D->Destroy();
		cMap2D = NULL;
	}

	if (cKeyboardController)
	{
		cKeyboardController->Destroy();
		cKeyboardController = NULL;
	}

	cFPSCounter = NULL;

}

/**
@brief Init Initialise this instance
*/ 
bool CScene2D::Init(void)
{
	// Setup the shaders
	cShader = new Shader("Shader//Scene2D.vs", "Shader//Scene2D.fs");

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	cShader->use();
	cShader->setInt("texture1", 0);

	// Setup the shaders
	textShader = new Shader("Shader//text.vs", "Shader//text.fs");

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	textShader->use();

	// Create and initialise the Map 2D
	cTextRenderer = CTextRenderer::GetInstance();
	// Set a shader to this class
	cTextRenderer->SetShader(textShader);
	// Initialise the instance
	if (cTextRenderer->Init() == false)
	{
		cout << "Failed to load CTextRenderer" << endl;
		return false;
	}

	// Create and initialise the Map 2D
	cKeyboardController = CKeyboardController::GetInstance();
	

	// Initialise Map2D
	cMap2D = CMap2D::GetInstance();
	// set the shader to use in cMap2D
	cMap2D->SetShader(cShader);
	// Initialise the map
	if (cMap2D->Init() == false)
	{
		cout << "Failed to load CMap2D" << endl;
		return false;
	}



	cFPSCounter = CFPSCounter::GetInstance();

	cPlayer2D = CPlayer2D::GetInstance();
	


	return true;
}

/**
@brief Update Update this instance
*/
void CScene2D::Update(const double dElapsedTime)
{
	cMap2D->Update(dElapsedTime);
	
	if (!cPlayer2D->IsPlayerAlive())
	{
		cout << "yes" << endl;
		//cMap2D->LoadMap("Maps/DM2231_Map_Level_01_SAVEGAME.csv");

		if (cMap2D->LoadMap("Maps/DM2231_Map_Level_01_SAVEGAME.csv") == false)
		{
			throw runtime_error("unable to save current game to a save file");
		}
		else
		{
			cMap2D->ReloadMap();
		}
	}

	// get keyboard updates
	if (cKeyboardController->IsKeyDown(VK_F6))
	{
		// save the current game to a file
		try
		{
			if (cMap2D->SaveMap("Maps/DM2231_Map_Level_01_SAVEGAME.csv") == false)
			{
				throw runtime_error("unable to save current game to a save file");
			}
		}
		catch (runtime_error ex)
		{
			cout << "runtime error : " << ex.what();
		}
	}

	if (cKeyboardController->IsKeyDown(VK_F7))
	{
		// save the current game to a file
		try
		{
			if (cMap2D->LoadMap("Maps/DM2231_Map_Level_01_SAVEGAME.csv") == false)
			{
				throw runtime_error("unable to save current game to a save file");
			}
			else
			{
				cMap2D->ReloadMap();
			}
		}
		catch (runtime_error ex)
		{
			cout << "runtime error : " << ex.what();
		}
	}

	if (cPlayer2D->GetIncreaseLevel() == 1)
	{
		// save the current game to a file
		try
		{
			if (cMap2D->LoadMap("Maps/DM2231_Map_Level_02.csv") == false)
			{
				throw runtime_error("unable to save current game to a save file");
			}
			else
			{
				cMap2D->ReloadMap();
			}
			cPlayer2D->SetIncreaseLevel(2);
		}
		catch (runtime_error ex)
		{
			cout << "runtime error : " << ex.what();
		}
	}
	else if (cPlayer2D->GetIncreaseLevel() == 3)
	{
		// save the current game to a file
		try
		{
			if (cMap2D->LoadMap("Maps/DM2231_Map_Level_03.csv") == false)
			{
				throw runtime_error("unable to save current game to a save file");
			}
			else
			{
				cMap2D->ReloadMap();
			}
			cPlayer2D->SetIncreaseLevel(4);
		}
		catch (runtime_error ex)
		{
			cout << "runtime error : " << ex.what();
		}
	}
	//cout << cPlayer2D->GetIncreaseLevel() << endl;
	

	
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene2D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable 2D texture rendering
	glEnable(GL_TEXTURE_2D);

	// Call the cMap2D's PreRender()
	cMap2D->PreRender();

	// Call the Map2D's PreRender()
	cTextRenderer->PreRender();

	
}

/**
 @brief Render Render this instance
 */
void CScene2D::Render(void)
{

	cMap2D->Render();

	// Call the CTextRenderer's Render()
	cTextRenderer->Render("DM2231 GDEV 2D HY", 10.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	cTextRenderer->Render("FPS:" + cFPSCounter->GetFrameRateString(), 10.0f, 30.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	cTextRenderer->Render("Health : " + (to_string(cPlayer2D->GetHealth())), 10.0f, 580.0f, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

	if (cPlayer2D->GetIsInvissible())
	{
		cTextRenderer->Render("invissible : activated", 10.0f, 550.0f, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

	}

	cTextRenderer->Render("Bullets : " + (to_string(cPlayer2D->GetBullets())), 600.0f, 550.0f, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	cTextRenderer->Render("scores : " + (to_string(cPlayer2D->GetScore())), 600.0f, 580.0f, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene2D::PostRender(void)
{

	cMap2D->PostRender();
	// Call the Map2D's PostRender()
	cTextRenderer->PostRender();
	

}