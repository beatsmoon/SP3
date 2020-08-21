/**
 Application.cpp
 @brief This file contains the Application class for this project, which initialises and control the OpenGL environment
*/
#include "Application.h"

#include <Windows.h>

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif
// Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "GameControl\Settings.h"

// Inputs
#include "Inputs\KeyboardController.h"
#include "Inputs/MouseController.h"

#include <iostream>
using namespace std;

/**
 @brief Define an error callback
 @param error The error code
 @param description The details about the error
 */
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

/**
 @brief Define the key input callback
 @param window The window to receive the the instructions
 @param key The key code
 @param scancode The scan code
 @param action The action to take
 @param mods The modifications
 */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);*/

	CKeyboardController::GetInstance()->Update(key, action);
}

/**
 @brief Callback function when the OpenGL window is repositioned
 @param xpos integer value of the new x position of the window
 @param ypos integer value of the new y position of the window
*/
void repos_callback(GLFWwindow* window, int xpos, int ypos)
{
	// Store the window width position
	CSettings::GetInstance()->iWindowPosX = xpos;
	CSettings::GetInstance()->iWindowPosY = ypos;
	// Update the glViewPort
	glViewport(0, 0, CSettings::GetInstance()->iWindowWidth, CSettings::GetInstance()->iWindowHeight);
}

/**
 @brief Callback function when the OpenGL window is resized
 @param w integer value of the new width of the window
 @param h integer value of the new height of the window
 */
void resize_callback(GLFWwindow* window, int width, int height)
{
	// Store the window width and height
	CSettings::GetInstance()->iWindowWidth = width;
	CSettings::GetInstance()->iWindowHeight = height;
	// Update the glViewPort
	glViewport(0, 0, width, height);
}

/**
 @brief Callback function whenever the mouse has events
 @brief window the window to recieve the instructions
 @param button An int value of the mouse button causing the event
 @param action An int value of the action caused by the mouse button
 @param mods An int value storing the mods of the event
 */
void MouseButtonCallBacks(GLFWwindow* window, int button, int action, int mods)
{
	// send thr callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		CMouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		CMouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

/**
 @brief Callback function whenever the mouse has events
 @param window the window to recieve the instructions
 @param xoffset A double value of the mouse scroll offset in the x-axis
 @param yoffset A double value of the mouse scroll offset in the y-axis
 */
void MouseScrollCallBacks(GLFWwindow* window, double xoffset, double yoffset)
{
	CMouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

/**
@brief Constructor
*/
Application::Application(void)
	: cSettings(NULL)
	//, cScene2D(NULL)
	,cScene3D(NULL)
	, cFPSCounter(NULL)
	, isShopOpened(false)
{
}

/**
@brief Destructor
*/
Application::~Application(void)
{
}

/**
 @brief Initialise this class instance
 */
bool Application::Init(void)
{
	// glfw: initialize and configure
	// ------------------------------
	//Initialize GLFW
	if (!glfwInit())
	{
		// return false if the GLFW was not initialised successfully
		return false;
	}

	// Get the CSettings instance
	cSettings = CSettings::GetInstance();

	// Set the file location for the digital assets
	// This is backup, in case filesystem cannot find the current directory
	cSettings->logl_root = "D:/My Documents/2020_2021_SEM1/DM2231 Game Development Techniques/Teaching Materials/Week 01/Practical/NYP_Framework_Week01";

	//Set the GLFW window creation hints - these are optional
	if (cSettings->bUse4XAntiliasing == true)
		glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	//Create a window and create its OpenGL context
	cSettings->pWindow = glfwCreateWindow(	cSettings->iWindowWidth, cSettings->iWindowHeight,
											"NYP Framework", NULL, NULL);
	//If the window couldn't be created, then return false
	if (cSettings->pWindow == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return false;
	}

	// Set OpenGL window position
	glfwSetWindowPos(cSettings->pWindow, cSettings->iWindowPosX, cSettings->iWindowPosY);

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(cSettings->pWindow);

	//Sets the callback functions for GLFW
	//Set the window position callback function
	glfwSetWindowPosCallback(cSettings->pWindow, repos_callback);
	//Set the window resize resize callback function
	glfwSetWindowSizeCallback(cSettings->pWindow, resize_callback);
	//Set the frame buffer resize callback function
	glfwSetFramebufferSizeCallback(cSettings->pWindow, resize_callback);
	//Set the error callback function
	glfwSetErrorCallback(error_callback);
	//Set the mouse button callback function
	glfwSetMouseButtonCallback(cSettings->pWindow, MouseButtonCallBacks);
	//Set the mouse button callback function
	glfwSetScrollCallback(cSettings->pWindow, MouseScrollCallBacks);
	//Set the keyboard callback function
	glfwSetKeyCallback(cSettings->pWindow, key_callback);

	// Additional customisation for the GLFW environment
	
	//if (cSettings->bDisableMousePointer == true)
	//	glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//else
	//	// Hide the cursor
	//	if (cSettings->bShowMousePointer == false)
	//		glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// glewExperimental is a variable that is already defined by GLEW. You must set it to GL_TRUE before calling glewInit(). 
	glewExperimental = GL_TRUE;
	// Initialize GLEW
	GLenum glewInitErr = glewInit();
	//If GLEW hasn't initialized, then return false
	if (glewInitErr != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glewInitErr));
		glfwTerminate();
		return false;
	}

	////Initialise the CScene2D instance
	//cScene2D = CScene2D::GetInstance();
	//if (cScene2D->Init() == false)
	//{
	//	cout << "failed to load Scene2D" << endl;
	//	return false;
	//}

	/*cScene3D = CScene3D::GetInstance();
	if (cScene3D->Init() == false)
	{
		cout << "failed to load Scene3D" << endl;
		return false;
	}*/

	//Initialise the CPFSCounter instance
	cFPSCounter = CFPSCounter::GetInstance();
	cFPSCounter->Init();

	return true;
}

/**
@brief Run this class instance
*/ 
void Application::Run(void)
{
	// Start timer to calculate how long it takes to render this frame
	cStopWatch.StartTimer();

	double dElapsedTime = 0.0;

	// Get the handler to the CSceneManager instance
	CSceneManager* cSceneManager = CSceneManager::GetInstance();

	// Add all the necessary scenes to the manager
	CSceneMenu3D* cSceneMenu3D = CSceneMenu3D::GetInstance();
	cSceneManager->AddScene(cSceneMenu3D);

	CSceneGame3D* cSceneGame3D = CSceneGame3D::GetInstance();
	cSceneManager->AddScene(cSceneGame3D);

	CSceneShop3D* cSceneShop3D = CSceneShop3D::GetInstance();
	cSceneManager->AddScene(cSceneShop3D);

	// Initialise the cSceneManager to initialise all the scenes added in
	if (cSceneManager->Init() == false)
		return;

	/*if (cSceneMenu3D->Init() == false)
	{
		std::cout << "Failed to load second scene" << std::endl;
		return;
	}
	if (cSceneShop3D->Init() == false)
	{
		std::cout << "Failed to load scene" << std::endl;
		return;
	}
	if (cSceneGame3D->Init() == false)
	{
		std::cout << "Failed to load cScene3D" << std::endl;
		return;
	}*/
	
	// Enable the starting scene
	// TODO: Change to Scenes::MENU once development is done
	cSceneManager->EnableScene(SCENES::GAME);
	//cSceneManager->EnableScene(1);

	// Get a reference to the list of scenes used
	std::vector<CScene3D*> vActiveScenes = cSceneManager->GetSceneList();

	// Render loop
	while (!glfwWindowShouldClose(cSettings->pWindow)
		&& (!CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_ESCAPE))
		&& cSceneManager->CheckForApplicationEnd() == false)
	{
		// TODO: Add conditions for how scenes should be changed. E.g. Press A to change to second scene
		if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_P) && !isShopOpened)
		{
			static double InputDelay = 5.f;
			if (InputDelay < 5.f)
			{
				InputDelay += 1.f;

			}

			else
			{
				InputDelay = 0.f;
				cSettings->SetMousePointer(false, true);
				CSceneManager::GetInstance()->EnableScene(SCENES::SHOP);
				CSceneManager::GetInstance()->DisableScene(SCENES::GAME);

				isShopOpened = true;
			}
		}
		else if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_P) && isShopOpened)
		{
			static double InputDelay = 5.f;
			if (InputDelay < 5.f)
			{
				InputDelay += 1.f;

			}

			else
			{
				InputDelay = 0.f;
				cSettings->SetMousePointer(true, false);
				CSceneManager::GetInstance()->DisableScene(SCENES::SHOP);
				CSceneManager::GetInstance()->EnableScene(SCENES::GAME);

				isShopOpened = false;
			}
		}
		for (size_t i = 0; i < vActiveScenes.size(); ++i)
		{
			if (vActiveScenes.at(i)->GetSceneStatus())
			{
				// Call the cScene3D's Update method
				vActiveScenes.at(i)->Update(dElapsedTime);

				// We did not use PreRender here because we want more than one scene rendered at once
				// PreRender calls glClear and glClearColor that clears the whole screen

				// Call the cScene3D's Render method
				vActiveScenes.at(i)->Render();

				// Call the cScene3D's PostRender method
				vActiveScenes.at(i)->PostRender();
			}
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(cSettings->pWindow);

		// Poll events
		glfwPollEvents();

		// Update Input Devices
		UpdateInputDevices();
		

		dElapsedTime = cStopWatch.GetElapsedTime();
		cFPSCounter->Update(dElapsedTime);

		// Frame rate limiter. Limits each frame to a specified time in ms.   
		cStopWatch.WaitUntil(cSettings->frameTime);
	}

	cSceneManager->Exit();
	cSceneManager->Destroy();
}

/**
 @brief Destroy this class instance
 */
void Application::Destroy(void)
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(cSettings->pWindow);
	//Finalize and clean up GLFW
	glfwTerminate();

	// Destroy the keyboard instance
	CKeyboardController::GetInstance()->Destroy();

	// Destroy CFPSCounter instance
	if (cFPSCounter)
	{
		cFPSCounter->Destroy();
		cFPSCounter = NULL;
	}

	// Destroy the CSettings instance
	if (cSettings)
	{
		cSettings->Destroy();
		cSettings = NULL;
	}
}

/**
 @brief Get window height
 */
int Application::GetWindowHeight(void) const
{
	return cSettings->iWindowHeight;
}

/**
 @brief Get window width
 */
int Application::GetWindowWidth(void) const
{
	return cSettings->iWindowWidth;
}

/**
@brief Get updates from the input devices
*/
void Application::UpdateInputDevices(void)
{
	// Update Keyboard Input
	//CKeyboardController::GetInstance()->Update();

	if (cSettings->bDisableMousePointer == true)
		glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		// Hide the cursor
		if (cSettings->bShowMousePointer == false)
			glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// Show the cursor
	else
		glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Update Mouse Input
	double dMouse_X, dMouse_Y;
	glfwGetCursorPos(cSettings->pWindow, &dMouse_X, &dMouse_Y);
	CMouseController::GetInstance()->UpdateMousePosition(dMouse_X, dMouse_Y);
}

/**
@brief End updates from the input devices
*/
void Application::PostUpdateInputDevices(void)
{
	CKeyboardController::GetInstance()->PostUpdate();

	// If the mouse is centered, need to update the center pos for next frame
	if (CMouseController::GetInstance()->GetKeepMouseCentered())
	{
		double dMouse_X, dMouse_Y;
		dMouse_X = cSettings->iWindowWidth >> 1;
		dMouse_Y = cSettings->iWindowHeight >> 1;
		CMouseController::GetInstance()->UpdateMousePosition(dMouse_X, dMouse_Y);
		glfwSetCursorPos(cSettings->pWindow, dMouse_X, dMouse_Y);
	}

	// Call input system to update at the end of frame
	CMouseController::GetInstance()->PostUpdate();
}
