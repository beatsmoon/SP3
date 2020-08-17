/**
 CScene2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include singleton
#include "DesignPatterns/SingletonTemplate.h"

// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include TextRenderer
#include "RenderControl\TextRenderer.h"

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

#include "../App/Source/TimeControl/FPSCounter.h"

// Include Map2D
#include "Map2D.h"

// Include CPlayer2D	
#include "Player2D.h"

class CScene2D : public CSingletonTemplate<CScene2D>
{
	friend CSingletonTemplate<CScene2D>;
public:
	

	// Init
	bool Init(void);

	// Update
	void Update(const double delapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	// Handler to the Shader Program instance
	Shader* cShader;
	// Handler to the Text Shader Program instance
	Shader* textShader;

protected:
	// The handler containing the instance of the CTextRenderer
	CTextRenderer* cTextRenderer;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// handler containing the instance of Map2D
	CMap2D* cMap2D;

	// handler containing the instance of Player2D
	CPlayer2D* cPlayer2D;

	CFPSCounter* cFPSCounter;

	CKeyboardController* cKeyboardController;

	// Constructor
	CScene2D(void);
	// Destructor
	virtual ~CScene2D(void);

	
};