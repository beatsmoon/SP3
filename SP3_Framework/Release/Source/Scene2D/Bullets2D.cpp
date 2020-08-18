#include "Bullets2D.h"

#include <iostream>
using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include ImageLoader
#include "System\ImageLoader.h"

// Include the Map2D as we will use it to check the player's movements and actions
#include "Map2D.h"

#include "Entity2DFactory.h"


CBullets2D::CBullets2D()
	: cMap2D(NULL)
	, cSettings(NULL)
	, isActive(false)

{
	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// Initialise vecIndex
	i32vec2Index = glm::i32vec2(0);

	// Initialise vecNumMicroSteps
	i32vec2NumMicroSteps = glm::i32vec2(0);

	// Initialise vec2UVCoordinate
	vec2UVCoordinate = glm::vec2(0.0f);
}

CBullets2D::~CBullets2D()
{
	// We won't delete this since it was created elsewhere
	cMap2D = NULL;

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

bool CBullets2D::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Get the handler to the CMap2D instance
	cMap2D = CMap2D::GetInstance();

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

	// Load the player texture

	if (LoadTexture("Images/shooting_bullets.tga", iTextureID) == false)
	{
		std::cout << "Failed to load bullets sprite texture" << std::endl;
		return false;
	}
	
	isActive = true;

}

void CBullets2D::Update(const double dElapsedTime)
{
	if (cAnimatedSprites.isFacingRight == true)
	{
		const int iOldIndex = i32vec2Index.x;

		// moving bullet right
		i32vec2NumMicroSteps.x += 3;

		if (i32vec2NumMicroSteps.x >= cSettings->NUM_STEPS_PER_TILE_XAXIS)
		{
			i32vec2NumMicroSteps.x = 0;
			i32vec2Index.x++;
		}

		// Constraint the Bullet's position within the screen boundary
		Constraint(RIGHT);

		// checking if bullet is near obsticle
		if (FindFeasiblePosition(iOldIndex, RIGHT) == true)
		{
			i32vec2Index.x = NULL;
			i32vec2Index.y = NULL;
			isActive = false;
			
		}
	}
	else
	{
		//cout << "VK_LEFT is called" << endl;
		const int iOldIndex = i32vec2Index.x;

		// moving player left
		i32vec2NumMicroSteps.x -= 3;

		if (i32vec2NumMicroSteps.x < 0)
		{
			i32vec2NumMicroSteps.x = (int)(cSettings->NUM_STEPS_PER_TILE_XAXIS) - 1;
			i32vec2Index.x--;
		}

		// Constraint the player's position within the screen boundary
		Constraint(LEFT);

		// FindFeasiblePosition
		if (FindFeasiblePosition(iOldIndex, LEFT) == true)
		{
			i32vec2Index.x = NULL;
			i32vec2Index.y = NULL;
			isActive = false;
			
		}
	}

	std::vector<CEntity2D*> it = CEntity2DFactory::GetInstance()->FindAllEntity(CEntity2D::TYPE::ENEMY);
	for (std::vector<CEntity2D*>::iterator itt = it.begin(); itt != it.end(); ++itt)
	{
		CEnemy2D* go = static_cast<CEnemy2D*>((CEntity2D*)*itt);

		if (i32vec2Index.x == go->Geti32vec2Index().x && i32vec2Index.y == go->Geti32vec2Index().y)
		{
			

			CPlayer2D* c = CPlayer2D::GetInstance();
			int s = c->GetScore();
			c->SetScore(s += 20);

			//isActive = 0;
			go->SetIsActive(0);
		}
		
	}



	vec2UVCoordinate.x = cSettings->ConvertIndexToUVSpace(cSettings->x, i32vec2Index.x, false, i32vec2NumMicroSteps.x * cSettings->MICRO_STEP_XAXIS);
	vec2UVCoordinate.y = cSettings->ConvertIndexToUVSpace(cSettings->y, i32vec2Index.y, false, i32vec2NumMicroSteps.y * cSettings->MICRO_STEP_YAXIS);
}

void CBullets2D::PreRender(void)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate the shader
	cShader->use();
}

void CBullets2D::Render(void)
{
	//cout << i32vec2Index.x << "   " << i32vec2Index.y << endl;

	//cout << vec2UVCoordinate.x << endl;
	// get matrix's uniform location and set matrix
	if (isActive != false)
	{
		unsigned int transformLoc = glGetUniformLocation(cShader->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(vec2UVCoordinate.x,
			vec2UVCoordinate.y,
			0.0f));
		// Update the shaders with the latest transform
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// Get the texture to be rendered
		glBindTexture(GL_TEXTURE_2D, iTextureID);

		glBindVertexArray(VAO);
		// Render the tile
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	//unsigned int transformLoc = glGetUniformLocation(cShader->ID, "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	//transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//transform = glm::translate(transform, glm::vec3(vec2UVCoordinate.x,
	//	vec2UVCoordinate.y,
	//	0.0f));
	//// Update the shaders with the latest transform
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	//// Get the texture to be rendered
	//glBindTexture(GL_TEXTURE_2D, iTextureID);

	//glBindVertexArray(VAO);
	//// Render the tile
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}



bool CBullets2D::LoadTexture(const char* filename, GLuint& iTextureID)
{
	// Variables used in loading the texture
	int width, height, nrChannels;

	// texture 1
	// ---------
	glGenTextures(1, &iTextureID);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CImageLoader* cImageLoader = CImageLoader::GetInstance();
	unsigned char* data = cImageLoader->Load(filename, width, height, nrChannels, true);
	if (data)
	{
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}
	// Free up the memory of the file data read in
	free(data);

	return true;
}

void CBullets2D::Constraint(BULLETDIRECTION eDirection)
{
	if (eDirection == LEFT)
	{
		if (i32vec2Index.x < 0)
		{
			i32vec2Index.x = 0;
			i32vec2NumMicroSteps.x = 0;
			i32vec2Index.x = NULL;
			i32vec2Index.y = NULL;
			isActive = false;
		}
	}
	else 
	{
		if (i32vec2Index.x >= ((int)(cSettings->NUM_TILES_XAXIS)) - 1)
		{
			i32vec2Index.x = ((int)(cSettings->NUM_TILES_XAXIS)) - 1;
			i32vec2NumMicroSteps.x = 0;
			i32vec2Index.x = NULL;
			i32vec2Index.y = NULL;
			isActive = false;
		}
	}
}

void CBullets2D::InteractWithMap(void)
{
}

bool CBullets2D::FindFeasiblePosition(const int iOldIndex, BULLETDIRECTION eDirection)
{
	bool state = false;
	if (eDirection == LEFT)
	{
		// If the player within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.x != 0) && (i32vec2Index.x >= 0))
		{
			
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x) == 1) // 1 is wall in map.csv if thrs are many diff kind of wall a range of number is more suitable
			{
				// Snap to the previous index
				i32vec2Index.x = iOldIndex;
				// Reset the i32vec2NumMicroSteps.x so that the player is aligned to the column
				i32vec2NumMicroSteps.x = 0;
				state = true;
			}
			// If the new position is between 2 rows, then check the other row as well
			if (i32vec2NumMicroSteps.y != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x) == 1)
				{
					// Snap to the previous index
					i32vec2Index.x = iOldIndex;
					// Reset the i32vec2NumMicroSteps.x so that the player is aligned to the column
					i32vec2NumMicroSteps.x = 0;
					state = true;
				}
			}
		}
	}
	else if (eDirection == RIGHT)
	{
		// If the player within the screen boundary, then check further
		if ((i32vec2NumMicroSteps.x != 0) && (i32vec2Index.x < ((int)cSettings->NUM_TILES_XAXIS) - 1))
		{
			// Check if this new position is not accessible, then go back to old position
			if (cMap2D->GetMapInfo(i32vec2Index.y, i32vec2Index.x + 1) == 1)
			{
				// Snap to the previous index
				i32vec2Index.x = iOldIndex;
				// Reset the i32vec2NumMicroSteps.x so that the player is aligned to the column
				i32vec2NumMicroSteps.x = 0;
				state = true;
			}
			// If the new position is between 2 rows, then check the other row as well
			if (i32vec2NumMicroSteps.y != 0)
			{
				// If the new position is not feasible, then go back to the old position
				if (cMap2D->GetMapInfo(i32vec2Index.y + 1, i32vec2Index.x + 1) == 1)
				{
					// Snap to the previous index
					i32vec2Index.x = iOldIndex;
					// Reset the i32vec2NumMicroSteps.x so that the player is aligned to the column
					i32vec2NumMicroSteps.x = 0;
					state = true;
				}
			}
		}
	}

	return state;
}

bool CBullets2D::ShootBullets(BULLETDIRECTION edirection)
{
	isActive = true;
	if (edirection == RIGHT)
	{
		const int iOldIndex = i32vec2Index.x;

		// moving bullet right
		i32vec2NumMicroSteps.x++;

		if (i32vec2NumMicroSteps.x >= cSettings->NUM_STEPS_PER_TILE_XAXIS)
		{
			i32vec2NumMicroSteps.x = 0;
			i32vec2Index.x++;
		}

		// Constraint the Bullet's position within the screen boundary
		Constraint(RIGHT);

		// checking if bullet is near obsticle
		if (FindFeasiblePosition(iOldIndex, RIGHT) == true)
		{
			isActive = false;
		}
	}
	else
	{
		//cout << "VK_LEFT is called" << endl;
		const int iOldIndex = i32vec2Index.x;

		// moving player left
		i32vec2NumMicroSteps.x--;

		if (i32vec2NumMicroSteps.x < 0)
		{
			i32vec2NumMicroSteps.x = (int)(cSettings->NUM_STEPS_PER_TILE_XAXIS) - 1;
			i32vec2Index.x--;
		}

		// Constraint the player's position within the screen boundary
		Constraint(LEFT);

		// FindFeasiblePosition
		if (FindFeasiblePosition(iOldIndex, RIGHT) == true)
		{
			isActive = false;
		}
	}
	return true;
}
