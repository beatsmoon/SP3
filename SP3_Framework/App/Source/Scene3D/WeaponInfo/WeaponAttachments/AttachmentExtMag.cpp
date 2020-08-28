/**
 CStructure3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "AttachmentExtMag.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CGunExtMag::CGunExtMag(void)
	: cGroundMap(NULL)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CGunExtMag::CGunExtMag(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front,
					const float fYaw,
					const float fPitch)
	: cGroundMap(NULL)
{
	// Set the default position to the origin
	this->vec3Position = vec3Position;
}

/**
 @brief Destructor
 */
CGunExtMag::~CGunExtMag(void)
{
	if (cGroundMap)
	{
		// We set it to NULL only since it was declared somewhere else
		cGroundMap = NULL;
	}

	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CGunExtMag::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CStructure3D::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::GUNEXTMAG);

	//// Initialise the cPlayer3D
	//cPlayer3D = CPlayer3D::GetInstance();

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// Rear face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		// Front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		// Left face
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		// Right face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		// Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		// Top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load and create a texture 
	iTextureID = LoadTexture("Images/Scene3D_ExtMagTexture.jpg");
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_ExtMagTexture.jpg" << endl;
		return false;
	}
	vec3Scale = glm::vec3(0.1, 0.1, 0.1);
	// Store the handler to the CGroundMap
	cGroundMap = CGroundMap::GetInstance();

	return true;
}


/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CGunExtMag::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CGunExtMag::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CGunExtMag::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Update this class instance
*/
void CGunExtMag::Update(const double dElapsedTime)
{
}
void CGunExtMag::AddAmmo(void)
{
}
/**
@brief Activate the CCollider for this class instance
@param cLineShader A Shader* variable which stores a shader which renders lines
*/
void CGunExtMag::ActivateCollider(Shader* cLineShader)
{
	// Create a new CCollider
	cCollider = new CCollider();
	// Set the colour of the CCollider to Blue
	cCollider->vec4Colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	// Initialise the cCollider
	cCollider->Init();
	// Set a shader to it
	cCollider->SetLineShader(cLineShader);
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CGunExtMag::PreRender(void)
{
	// Draw this as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

/**
@brief Render Render this instance
@param cShader A Shader* variable which contains the Shader to use in this class instance
*/
void CGunExtMag::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CStructure3D::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);

	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	cShader->setMat4("projection", projection);
	cShader->setMat4("view", view);
	cShader->setMat4("model", model);

	// render boxes
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Render the CCollider if needed
	if ((cCollider) && (cCollider->bIsDisplayed))
	{
		cCollider->model = model;
		cCollider->view = view;
		cCollider->projection = projection;
		cCollider->Render();
	}
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CGunExtMag::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

void CGunExtMag::UpgradeExtMag()
{
	eTierLevel = static_cast<TIERLEVEL>(static_cast<int>(eTierLevel) + 1);
}


float CGunExtMag::GetAdditionMag(void)
{
	switch (eTierLevel)
	{
	case TIERLEVEL::TIER1:
	{
		return 5;
	}
	break;
	case TIERLEVEL::TIER2:
	{
		return 10;
	}
	break;
	case TIERLEVEL::TIER3:
	{
		return 15;
	}
	break;
	}
	return 0;
}
void CGunExtMag::SetTierLevel(const TIERLEVEL eStatus)
{
	eTierLevel = eStatus;
}
CGunExtMag::TIERLEVEL CGunExtMag::GetTierLevel(void) const
{
	return eTierLevel;
}
