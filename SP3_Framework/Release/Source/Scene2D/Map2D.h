/**
 Map2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"
// Include shader
#include "RenderControl\shader.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CSVReader
#include "../CSVReader/CSVReader.h"
// Include map storage
#include <map>

// Include Settings
#include "GameControl\Settings.h"

// Include Player
#include "Player2D.h"

// Include Enemy
#include "Enemy2D.h"

#include "Bullets2D.h"

// Include CEntity2DFactory
#include "Entity2DFactory.h"

class CMap2D : public CSingletonTemplate<CMap2D>
{
	friend CSingletonTemplate<CMap2D>;
public:
	// Init
	bool Init(void);

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	// Set a shader to this class instance
	void SetShader(Shader* cShader);

	// Set the specifications of the map
	void SetNumTiles(const CSettings::AXIS sAxis, const unsigned int uiValue);
	void SetNumSteps(const CSettings::AXIS sAxis, const unsigned int uiValue);

	// Set the value at certain indices in the vMapInformation
	void SetMapInfo(const int iRow, const int iCol, const int iValue, const bool bInvert = true);

	// Get the value at certain indices in the vMapInformation
	int GetMapInfo(const int iRow, const int iCol, const bool bInvert = true) const;

	// Load a map
	bool LoadMap(string filename);

	// Save the map
	bool SaveMap(string filename);

	bool SaveObjinMap(string filename);

	bool ReloadMap();

protected:
	// Handler to the Shader Program instance
	Shader* cShader;

	// OpenGL objects
	unsigned int VBO, VAO, EBO;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// Vectors of Vectors containing the Map Information
	vector<vector<int>> vMapInformation;

	// Map containing texture IDs
	map<int, int> MapOfTextureIDs;

	// Settings
	CSettings* cSettings;

	// Player
	CPlayer2D* cPlayer2D;

	// Enemy
	//CEnemy2D* cEnemy2D;

	CBullets2D* cBullets2D;

	// Ther handler containing the instance of the CEnity2DFactory
	CEntity2DFactory* cEntity2DFactory;

	// Constructor
	CMap2D(void);

	// Destructor
	virtual ~CMap2D(void);

	// Find the indices of a certain value in vMapInformation
	bool FindValue(const int iValue, int& iRow, int& iCol, const bool bInvert = true);

	// Load a texture
	bool LoadTexture(const char* filename, const int iTextureCode);

	// Render a tile
	void RenderTile(const int iRow, const int iCol);
};

