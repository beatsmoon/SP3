/**
 Map2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Map2D.h"

#include <iostream>
using namespace std;

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

// Include Filesystem
#include "System\filesystem.h"
// Include ImageLoader
#include "System\ImageLoader.h"

// CSVReader
#include "../CSVReader/CSVReader.h"

// CSVWriter
#include "../CSVWriter/CSVWriter.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CMap2D::CMap2D(void)
	: cShader(NULL)
	, VAO(0)
	, VBO(0)
	, EBO(0)
	, cSettings(NULL)
	, cPlayer2D(NULL)
	//, cEnemy2D(NULL)
	, cBullets2D(NULL)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CMap2D::~CMap2D(void)
{
	// Set this to NULL since it was created elsewhere, so we let it be deleted there.
	cSettings = NULL;

	// We won't delete this since it was created elsewhere
	cShader = NULL;

	// Delete cPlayer2D
	if (cPlayer2D)
	{
		cPlayer2D->Destroy();
		cPlayer2D = NULL;
	}

	if (cEntity2DFactory)
	{
		cEntity2DFactory->Destroy();
		cEntity2DFactory = NULL;
	}

	if (cBullets2D)
	{
		cBullets2D = NULL;
	}
	
	/*if (cEnemy2D)
	{
		cEnemy2D = NULL;
	}*/

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

/**
 @brief Set a shader to this class instance
 @param cShader A Shader variable which stores the Shader instance
 */
void CMap2D::SetShader(Shader* cShader)
{
	this->cShader = cShader;
}

/**
@brief Init Initialise this instance
*/ 
bool CMap2D::Init(void)
{
	if (!cShader)
	{
		cout << "CMap2D::Init: Shader not found" << endl;
		return false;
	}

	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	cEntity2DFactory = CEntity2DFactory::GetInstance();


	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {	// positions          // texture coords
		(cSettings->TILE_WIDTH / 2.0f), (cSettings->TILE_HEIGHT / 2.0f), 0.0f, 1.0f, 1.0f, // top right
		(cSettings->TILE_WIDTH / 2.0f), -(cSettings->TILE_HEIGHT / 2.0f), 0.0f, 1.0f, 0.0f, // bottom right
		-(cSettings->TILE_WIDTH / 2.0f), -(cSettings->TILE_HEIGHT / 2.0f), 0.0f, 0.0f, 0.0f, // bottom left
		-(cSettings->TILE_WIDTH / 2.0f), (cSettings->TILE_HEIGHT / 2.0f), 0.0f, 0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = { 0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Load and create textures

	// Load the ground texture
	if (LoadTexture("Images/brick.tga", 1) == false)
	{
		std::cout << "Failed to load brick tile texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/portal.tga", 21) == false)
	{
		std::cout << "Failed to load portal texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/portal.tga", 22) == false)
	{
		std::cout << "Failed to load portal texture" << std::endl;
		return false;
	}

	// Load the tree texture
	if (LoadTexture("Images/bullet.tga", 11) == false)
	{
		std::cout << "Failed to load ammunation texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/potion.tga", 12) == false)
	{
		std::cout << "Failed to load potion texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/potion_i.tga", 13) == false)
	{
		std::cout << "Failed to load potion i texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/coin.tga", 14) == false)
	{
		std::cout << "Failed to load coin texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/save.tga", 31) == false)
	{
		std::cout << "Failed to load savepoint texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/chest.tga", 32) == false)
	{
		std::cout << "Failed to load chest tile texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/door.tga", 33) == false)
	{
		std::cout << "Failed to load door tile texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/key.tga", 15) == false)
	{
		std::cout << "Failed to load key texture" << std::endl;
		return false;
	}

	if (LoadTexture("Images/saved.tga", 34) == false)
	{
		std::cout << "Failed to load key texture" << std::endl;
		return false;
	}

	// Load the map into an array
	if (LoadMap("Maps/DM2231_Map_Level_01.csv") == false)
	{
		// The initialisation of this map has failed. Return false
		return false;
	}

	// Create and initialise cPlayer2D
	cPlayer2D = (CPlayer2D*)cEntity2DFactory->Create(PLAYER2D);

	// Pass shader to cPlayer2D
	cPlayer2D->SetShader(cShader);

	// initilaise the instance
	if (cPlayer2D->Init() == false)
	{
		cout << "Failed to load CPlayer2D" << endl;
		return false;
	}

	// Find indices for the player in vMapinfomation, and assign it 
	int iRow = -1;
	int iCol = -1;
	if (FindValue(PLAYER2D, iRow, iCol) == false)
	{
		return false; // unable to find the start pos of the player in the map
	}

	// Erase the value of the player in the vMapinfomation
	SetMapInfo(iRow, iCol, 0);

	cPlayer2D->Seti32vec2Index(iCol, iRow);
	cPlayer2D->Seti32vec2NumMicroSteps(0, 0);


	// Create and initialise cEnemy2D
	bool isEnemy = true;
	while (isEnemy)
	{
		iRow = -1;
		iCol = -1;
		if (FindValue(4, iRow, iCol))
		{
			CEnemy2D* cEnemy2D = (CEnemy2D*)CEntity2DFactory::GetInstance()->Create(ENEMY2D);

			// Pass shader to cEnemy2D
			cEnemy2D->SetShader(cShader);

			// Initialise the instance
			if (cEnemy2D->Init() == 0)
			{
				cout << "Failed to load cEnemy2D" << endl;
				return false;
			}



			// Erase the value of the enemy in the vMapinfomation
			SetMapInfo(iRow, iCol, 0);

			cEnemy2D->Seti32vec2Index(iCol, iRow);
			cEnemy2D->Seti32vec2NumMicroSteps(0, 0);

			cEnemy2D->SetPlayer2D(cPlayer2D);
		}
		else
		{
			isEnemy = false;
		}
		
	}

	//SaveMap("Maps/DM2231_Map_Level_01_SAVEGAME.csv");

	SetNumSteps(cSettings->x, 12);
	SetNumSteps(cSettings->y, 16);


	return true;
}

/**
@brief Update Update this instance
*/
void CMap2D::Update(const double dElapsedTime)
{
	// player updates
	cPlayer2D->Update(dElapsedTime);

	// enemy updates
	std::vector<CEntity2D*> it1 = CEntity2DFactory::GetInstance()->FindAllEntity(CEntity2D::TYPE::ENEMY);
	for (std::vector<CEntity2D*>::iterator itt = it1.begin(); itt != it1.end(); ++itt)
	{
		CEnemy2D* go = static_cast<CEnemy2D*>((CEntity2D*)*itt);

		if (go->GetIsActive() == false && go != NULL)
		{
			go = NULL;
			//delete go;
		}
		else
		{
			go->Update(dElapsedTime);

		}
	}

	std::vector<CEntity2D*> it = CEntity2DFactory::GetInstance()->FindAllEntity(CEntity2D::TYPE::BULLET);
	for (std::vector<CEntity2D*>::iterator itt = it.begin(); itt != it.end(); ++itt)
	{
		CBullets2D* go = static_cast<CBullets2D*>((CEntity2D*)*itt);

		
		if (go->isActive == false && go != NULL)
		{
			//delete go;
			go = NULL;
			delete go;
		}
		else
		{
			go->Update(dElapsedTime);
		}
	}

//	cEntity2DFactory->FindEntity(BULLET2D)->Update();

// reactable counts
// controls
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CMap2D::PreRender(void)
{
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate the shader
	cShader->use();
}

/**
 @brief Render Render this instance
 */
void CMap2D::Render(void)
{
	cShader->use();

	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(cShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Render

	// render tiles
	for (int iRow = 0; iRow < (int)cSettings->NUM_TILES_YAXIS; iRow++)
	{
		for (int iCol = 0; iCol < (int)cSettings->NUM_TILES_XAXIS; iCol++)
		{
			transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			transform = glm::translate(transform, glm::vec3(cSettings->ConvertIndexToUVSpace(cSettings->x, iCol, false, 0),
															cSettings->ConvertIndexToUVSpace(cSettings->y, iRow, true, 0),
															0.0f));
			//transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			// Update the shaders with the latest transform
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

			// Render a tile
			RenderTile(iRow, iCol);
		}
	}

	// render player
	cPlayer2D->Render();

	// render enemy
	std::vector<CEntity2D*> it1 = CEntity2DFactory::GetInstance()->FindAllEntity(CEntity2D::TYPE::ENEMY);
	for (std::vector<CEntity2D*>::iterator itt = it1.begin(); itt != it1.end(); ++itt)
	{
		CEnemy2D* go = static_cast<CEnemy2D*>((CEntity2D*)*itt);
		if (go != NULL && go->GetIsActive() == true)
		{
			go->Render();
		}
	}

	//i mean this
	// okok
	std::vector<CEntity2D*> it = CEntity2DFactory::GetInstance()->FindAllEntity(CEntity2D::TYPE::BULLET);
	for (std::vector<CEntity2D*>::iterator itt = it.begin(); itt != it.end(); ++itt)
	{
		CBullets2D* go = static_cast<CBullets2D*>((CEntity2D*)*itt);

		
		go->Render();
	}
	//cEntity2DFactory->FindEntity(BULLET2D)->Render();
		
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CMap2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);


}

// Set the specifications of the map
void CMap2D::SetNumTiles(const CSettings::AXIS sAxis, const unsigned int uiValue)
{
	// Check if the value is valid
	if (uiValue <= 0)
	{
		cout << "CMap2D::SetNumTiles() : value must be more than 0" << endl;
		return;
	}

	if (sAxis == CSettings::x)
	{
		cSettings->NUM_TILES_XAXIS = uiValue;
		cSettings->UpdateSpecifications();
	}
	else if (sAxis == CSettings::y)
	{
		cSettings->NUM_TILES_YAXIS = uiValue;
		cSettings->UpdateSpecifications();
	}
	else if (sAxis == CSettings::z)
	{
		// Not used in here
	}
	else
	{
		cout << "Unknown axis" << endl;
	}
}

// Set the specifications of the map
void CMap2D::SetNumSteps(const CSettings::AXIS sAxis, const unsigned int uiValue)
{
	// Check if the value is valid
	if (uiValue <= 0)
	{
		cout << "CMap2D::SetNumSteps() : value must be more than 0" << endl;
		return;
	}

	if (sAxis == CSettings::x)
	{
		cSettings->NUM_STEPS_PER_TILE_XAXIS = (float)uiValue;
		cSettings->UpdateSpecifications();
	}
	else if (sAxis == CSettings::y)
	{
		cSettings->NUM_STEPS_PER_TILE_YAXIS = (float)uiValue;
		cSettings->UpdateSpecifications();
	}
	else if (sAxis == CSettings::z)
	{
		// Not used in here
	}
	else
	{
		cout << "Unknown axis" << endl;
	}
}

/**
 @brief Set the value at certain indices in the vMapInformation
 @param iRow A const int variable containing the row index of the element to set to
 @param iCol A const int variable containing the column index of the element to set to
 @param iValue A const int variable containing the value to assign to this vMapInformation
 */
void CMap2D::SetMapInfo(const int iRow, const int iCol, const int iValue, const bool bInvert)
{
	if (bInvert)
		vMapInformation[cSettings->NUM_TILES_YAXIS - iRow - 1][iCol] = iValue;
	else
		vMapInformation[iRow][iCol] = iValue;
}

/**
 @brief Get the value at certain indices in the vMapInformation
 @param iRow A const int variable containing the row index of the element to get from
 @param iCol A const int variable containing the column index of the element to get from
 @param bInvert A const bool variable which indicates if the row information is inverted
 */
int CMap2D::GetMapInfo(const int iRow, const int iCol, const bool bInvert) const
{
	if (bInvert)
		return vMapInformation[cSettings->NUM_TILES_YAXIS - iRow - 1][iCol];
	else
		return vMapInformation[iRow][iCol];
}

/**
 @brief Load a map
 */ 
bool CMap2D::LoadMap(string filename)
{
	// Load the map into an array
	// CSV Reader
	//1
	CCSVReader cCSVReader;

	// Load a CSV file into vMapInformation
	//2
	vMapInformation = cCSVReader.read_csv(FileSystem::getPath(filename).c_str(), cSettings->NUM_TILES_XAXIS, cSettings->NUM_TILES_YAXIS);


	// If vMapInformation is empty, then the initialisation of this map has failed. Return false
	if (vMapInformation.size() == 0)
		return false;

	return true;
}

bool CMap2D::SaveMap(string filename)
{
	// make a copy of the vMapInfomation
	vector<vector<int>> vMapForSaving(vMapInformation);

	// update the vMapForSaving with positions of the player and the enemy
	glm::i32vec2 playerPos = cPlayer2D->Geti32vec2Index();
	vMapForSaving[(int)cSettings->NUM_TILES_YAXIS - playerPos.y - 1][playerPos.x] = 3;

	// save to a file
	CCSVWriter cCSVWriter;
	cCSVWriter.write_csv(filename, (int)cSettings->NUM_TILES_XAXIS, (int)cSettings->NUM_TILES_YAXIS, vMapForSaving);
	return true;
}

bool CMap2D::SaveObjinMap(string filename)
{
	vector<vector<int>> vMapForSaving(vMapInformation);

	int iRow = -1;
	int iCol = -1;
	if (FindValue(34, iRow, iCol) == true)
	{
		vMapForSaving[(int)cSettings->NUM_TILES_YAXIS - iRow - 1][iCol] = 3;
	}

	CCSVWriter cCSVWriter;
	cCSVWriter.write_csv(filename, (int)cSettings->NUM_TILES_XAXIS, (int)cSettings->NUM_TILES_YAXIS, vMapForSaving);
	return true;
}

bool CMap2D::ReloadMap()
{


	int iRow = -1;
	int iCol = -1;
	if (FindValue(PLAYER2D, iRow, iCol) == false)
	{
		cout << "fk u" << endl;
		return false; // unable to find the start pos of the player in the map
	}

	// Erase the value of the player in the vMapinfomation
	SetMapInfo(iRow, iCol, 34);

	cPlayer2D->Seti32vec2Index(iCol, iRow);
	cPlayer2D->Seti32vec2NumMicroSteps(0, 0);
	cPlayer2D->SetHealth(3);

	//std::vector<CEntity2D*> it1 = CEntity2DFactory::GetInstance()->FindAllEntity(CEntity2D::TYPE::ENEMY);
	//for (std::vector<CEntity2D*>::iterator itt = it1.begin(); itt != it1.end(); ++itt)
	//{
	//	CEnemy2D* go = static_cast<CEnemy2D*>((CEntity2D*)*itt);

	//	go->SetIsActive(0);
	//	
	//}

	bool isEnemy = true;
	while (isEnemy)
	{
		iRow = -1;
		iCol = -1;
		
		if (FindValue(4, iRow, iCol))
		{
			CEnemy2D* cEnemy2D = (CEnemy2D*)CEntity2DFactory::GetInstance()->Create(ENEMY2D);
			cEnemy2D->Init();
			cEnemy2D->SetShader(cShader);

			// Erase the value of the enemy in the vMapinfomation
			SetMapInfo(iRow, iCol, 0);

			cEnemy2D->Seti32vec2Index(iCol, iRow);
			cEnemy2D->Seti32vec2NumMicroSteps(0, 0);

			cEnemy2D->SetPlayer2D(cPlayer2D);
			
		}
		else
		{
			isEnemy = false;
		}

	}
}

/**
@brief Find the indices of a certain value in vMapInformation
@param iValue A const int variable containing the row index of the found element
@param iRow A const int variable containing the row index of the found element
@param iCol A const int variable containing the column index of the found element
@param bInvert A const bool variable which indicates if the row information is inverted
*/
bool CMap2D::FindValue(const int iValue, int& iRow, int& iCol, const bool bInvert) // can use for collision check
{
	for (int irow = 0; irow < (int)cSettings->NUM_TILES_YAXIS; irow++)
	{
		for (int icol = 0; icol < (int)cSettings->NUM_TILES_XAXIS; icol++)
		{
			if (vMapInformation[irow][icol] == iValue)
			{
				if (bInvert)
					iRow = cSettings->NUM_TILES_YAXIS - irow - 1;
				else
					iRow = irow;
				iCol = icol;
				return true;	// Return true immediately if the value has been found
			}
		}
	}return false;
}

/**
 @brief Load a texture, assign it a code and store it in MapOfTextureIDs.
 @param filename A const char* variable which contains the file name of the texture
 @param iTextureCode A const int variable which is the texture code.
 */
bool CMap2D::LoadTexture(const char* filename, const int iTextureCode)
{
	// Variables used in loading the texture
	int width, height, nrChannels;
	unsigned int textureID;

	// texture 1
	// ---------
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CImageLoader* cImageLoader = CImageLoader::GetInstance();
	unsigned char *data = cImageLoader->Load(filename, width, height, nrChannels, true);
	if (data)
	{
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Store the texture ID into MapOfTextureIDs
		MapOfTextureIDs.insert(pair<int, int>(iTextureCode, textureID));
	}
	else
	{
		return false;
	}
	// Free up the memory of the file data read in
	free(data);

	return true;
}

/**
 @brief Render a tile at a position based on its tile index
 @param iRow A const int variable containing the row index of the tile
 @param iCol A const int variable containing the column index of the tile
 */
void CMap2D::RenderTile(const int iRow, const int iCol) // render 1 quad (1 tile)
{
	if (vMapInformation[iRow][iCol] != 0)
	{
		glBindTexture(GL_TEXTURE_2D, MapOfTextureIDs.at(vMapInformation[iRow][iCol]));

		glBindVertexArray(VAO);
		// Render the tile
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
