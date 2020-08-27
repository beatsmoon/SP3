#include "Scope.h"
#include "GL\glew.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CScope::CScope(void)
	: bActive(true)
{
}

/**
 @brief Destructor
 */
CScope::~CScope(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cShader = NULL;
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CScope::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CCrossHair::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	float fAspectRatio = (float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight;

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// positions			// texture coords
		-0.5f, 0.5f * fAspectRatio, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f * fAspectRatio, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f * fAspectRatio, 0.0f, 1.0f, 0.0f,

		0.5f, -0.5f * fAspectRatio, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f * fAspectRatio, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f * fAspectRatio, 0.0f, 0.0f, 1.0f
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

	if ((VAO == 0) || (VBO == 0))
	{
		std::cout << "Unable to initialise CameraEffect" << std::endl;
		return false;
	}

	// load and create a texture 
	iTextureID = LoadTexture("Images/GUI/Scene3D_ScopedMode_TIER4.tga");
	if (iTextureID == 0)
	{
		cout << "Unable to load Images/GUI/Scene3D_ScopedMode_TIER4.tga" << endl;
		return false;
	}

	bActive = true;

	eStorePrevTierLevel =eTierLevel = CGunScope::TIERLEVEL::NOTIER;
	//cPlayer3D = CPlayer3D::GetInstance();
	vec3Scale = glm::vec3(2, 1.2, 1);
	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CScope::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CScope::SetView(glm::mat4 view)
{
	this->view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CScope::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/** 
 @brief Toggle crosshair on or off
 */
void CScope::SetStatus(const bool bStatus)
{
	bActive = bStatus;
}
/**
@brief Get crosshair status
*/
bool CScope::GetStatus(void) const
{
	return bActive;
}
void CScope::UpdateTierListOfScope(CGunScope::TIERLEVEL tierLevel)
{
	eTierLevel = tierLevel;
}
/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScope::Update(const double dElapsedTime)
{
	//cPlayer3D->GetWeapon()->GetScope()->GetTierLevel();
	if (eTierLevel == eStorePrevTierLevel)
	{
		return;
	}
	eStorePrevTierLevel = eTierLevel;
	switch (eTierLevel)
	{
	case CGunScope::TIERLEVEL::NOTIER:
		{
			//iTextureID = LoadTexture("Images/GUI/Scene3D_Holding_Pistol.tga");
		}
		break;
	case CGunScope::TIERLEVEL::TIER1:
		{
			vec3Position = glm::vec3(0, -0.1, 1);
			vec3Scale = glm::vec3(2, 1.5, 1);
			iTextureID = LoadTexture("Images/GUI/Scene3D_ScopeMode_Tier1.tga");
		}
		break;
	case CGunScope::TIERLEVEL::TIER2:
		{
			vec3Position = glm::vec3(0, 0.0f, 1);
			vec3Scale = glm::vec3(2, 1.5, 1);
			iTextureID = LoadTexture("Images/GUI/Scene3D_ScopeMode_Tier2.tga");
		}
		break;
	case CGunScope::TIERLEVEL::TIER3:
		{
			vec3Scale = glm::vec3(2, 1.2, 1);
			vec3Position = glm::vec3(0, -0.01, 1);
			iTextureID = LoadTexture("Images/GUI/Scene3D_ScopedMode_Tier3.tga");
		}
		break;

		default:
			break;
	}
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScope::PreRender(void)
{
	if (!bActive)
		return;

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
 @brief Render Render this instance
 @param cShader A Shader* variable which contains the Shader to use in this class instance
 */
void CScope::Render(void)
{
	if (!bActive)
		return;

	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CCrossHair::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	unsigned int transformLoc;
	// get matrix's uniform location and set matrix
	transformLoc = glGetUniformLocation(cShader->ID, "transform");

	transform = glm::mat4(1.0f);
	// Translate to the position to render. Note that the centrepoint of the progressbar will be at this position
	// So we need to offset to the right by the vec3Scale.x * fWidth
	transform = glm::translate(transform, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	// Scale the size of the progressbar by vec3Scale. 
	// Note that .y and .z should be 1.0f if you are not scaling them at all
	transform = glm::scale(transform, vec3Scale);

	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Get the texture to be rendered
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// Render the quad
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Reset to default
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScope::PostRender(void)
{
	if (!bActive)
		return;

	// Disable blending
	glDisable(GL_BLEND);
}
