/**
 Application
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include "TimeControl\StopWatch.h"

#include "TimeControl\FPSCounter.h"
//
//#include "Scene2D/Scene2D.h"
//
//#include "Scene3D/Scene3D.h"

#include "SceneControl/SceneManager.h"



struct GLFWwindow;

class CSettings;

class Application : public CSingletonTemplate<Application>
{
	friend CSingletonTemplate<Application>;
public:
	// Initialise this class instance
	bool Init(void);
	// Run this class instance
	void Run(void);
	// Destroy this class instance
	void Destroy(void);

	// Get window height
	int GetWindowHeight(void) const;
	// Get window width
	int GetWindowWidth(void) const;
	
protected:
	// Declare timer for calculating time per frame
	CStopWatch cStopWatch;

	// The handler to the CSettings instance
	CSettings* cSettings;

	// the handlder to the cscene2d instance
	//CScene2D* cScene2D;

	// the handler to the cScene3D instance
	CScene3D* cScene3D;

	CFPSCounter* cFPSCounter;

	// Constructor
	Application(void);

	// Destructor
	virtual ~Application(void);

	// Update input devices
	void UpdateInputDevices(void);
	void PostUpdateInputDevices(void);
};
