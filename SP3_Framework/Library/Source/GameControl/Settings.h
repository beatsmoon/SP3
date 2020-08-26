/**
 CSettings
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

#include <Windows.h>
#include "../Library/Source/System/ConfigFile.h"
#include "../Inputs/KeyboardController.h"

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif
// Include GLFW
#include <GLFW/glfw3.h>

class CSettings : public CSingletonTemplate<CSettings>
{
	friend CSingletonTemplate<CSettings>;
public:
	enum AXIS
	{
		x = 0,
		y = 1,
		z = 2,
		NUM_AXIS
	};

	struct Key
	{
		std::string key_name;
		int key_value;
	};

	enum KEY_TYPE
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		NUM_KEYS,
	};

	// File information
	char* logl_root;	// Root directory for loading digital assets

	// GLFW Information
	GLFWwindow* pWindow;
	bool bUse4XAntiliasing = true;

	// Windows Information
	// Should make these not hard-coded :P
	unsigned int iWindowWidth = 1280;
	unsigned int iWindowHeight = 760;
	unsigned int iWindowPosX = 50;
	unsigned int iWindowPosY = 50;
	bool bShowMousePointer = false;
	bool bDisableMousePointer = true;
	// Frame Rate Information
	const unsigned char FPS = 60; // FPS of this game
	const unsigned int frameTime = 1000 / FPS; // time for each frame

	// Input control
	Key keys[NUM_KEYS];
	int totalKeys;

	CKeyboardController* cKeyboardController;

	// 2D Settings
	// The variables which stores the specifications of the map
	unsigned int NUM_TILES_XAXIS;
	unsigned int NUM_TILES_YAXIS;

	float TILE_WIDTH;
	float TILE_HEIGHT;

	float NUM_STEPS_PER_TILE_XAXIS;
	float NUM_STEPS_PER_TILE_YAXIS;

	float MICRO_STEP_XAXIS;
	float MICRO_STEP_YAXIS;

	// Update the specifications of the map
	void UpdateSpecifications(void);

	// Convert an index number of a tile to a coordinate in UV Space
	float ConvertIndexToUVSpace(const AXIS sAxis, const int iIndex, const bool bInvert, const float fOffset = 0.0f);

	//Sets mouse pointer to be visible on screen
	void SetMousePointer(bool bDisableMousePointer, bool bShowMousePointer);

	// Set all key names
	void SetKeyInformation();

	// Load all key values from text file
	void LoadKeyInformation();
	// Parse all key values back to the text file
	void ParseKeyInformation();

	void ChangeKeyInformation(unsigned int keyToChange);

protected:
	// Constructor
	CSettings(void);

	// Destructor
	virtual ~CSettings(void);

	ConfigFile* cConfigFile;
};

