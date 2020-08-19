#include "ProgressBar.h"
#include "GL\glew.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CProgressBar::CProgressBar(void) : cPlayer3D(NULL)
{
}

CProgressBar::CProgressBar(const int iCurrentValue, const int iTotalValue, Progress_Bar_type type) : cPlayer3D(NULL)
{
	this->fCurrValue = iCurrentValue;
	this->fTotalValue = iTotalValue;
	this->type = type;
}

/**
 @brief Destructor
 */
CProgressBar::~CProgressBar(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cShader = NULL;

	cPlayer3D = NULL;
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CProgressBar::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CProgressBar::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	cPlayer3D = CPlayer3D::GetInstance();

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	switch (type)
	{
	case P_HEALTH:
		fHeight = 0.0333f * 1;
		fWidth = 0.0333f * 10;
		vec3Position = glm::vec3(-1.0f + 0.0333f, -1.0f + 0.0333f * 58, 0.0f);
		vec4Colour = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);
		vec3Scale.x = ((float)fCurrValue / fTotalValue);
		break;
	case P_BULLET_PER_MAG:
		fHeight = 0.0333f * 1;
		fWidth = 0.0333f * 10;
		vec3Position = glm::vec3(-1.0f + 0.0333f, -1.0f + 0.0333f * 10, 0.0f);
		vec4Colour = glm::vec4(1.0f, 0.5f, 0.0f, 0.5f);
		vec3Scale.x = ((float)fCurrValue / fTotalValue);
		break;
	case P_BULLET_TOTAL:
		fHeight = 0.0333f * 1;
		fWidth = 0.0333f * 10;
		vec3Position = glm::vec3(-1.0f + 0.0333f, -1.0f + 0.0333f * 5, 0.0f);
		vec4Colour = glm::vec4(1.0f, 0.0f, 0.5f, 0.5f);
		vec3Scale.x = ((float)fCurrValue / fTotalValue);
		break;
	default:
		break;
	}

	

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// positions		// texture coords
		-fWidth,  fHeight, 0.0f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-fWidth, -fHeight, 0.0f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		 fWidth, -fHeight, 0.0f, vec4Colour.x, vec4Colour.y, vec4Colour.z,

		 fWidth, -fHeight, 0.0f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		 fWidth,  fHeight, 0.0f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-fWidth,  fHeight, 0.0f, vec4Colour.x, vec4Colour.y, vec4Colour.z,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CProgressBar::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CProgressBar::SetView(glm::mat4 view)
{
	this->view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CProgressBar::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CProgressBar::Update(const double dElapsedTime)
{
	switch (type)
	{
	case P_HEALTH:
		fCurrValue = cPlayer3D->GetCurrHealth();
		fTotalValue = cPlayer3D->GetMaxHealth();
		break;
	case P_BULLET_PER_MAG:
		fCurrValue = cPlayer3D->GetWeapon()->GetMagRound();
		fTotalValue = cPlayer3D->GetWeapon()->GetMaxMagRound();
		break;
	case P_BULLET_TOTAL:
		fCurrValue = cPlayer3D->GetWeapon()->GetTotalRound();
		fTotalValue = cPlayer3D->GetWeapon()->GetMaxTotalRound();
		break;
	default:
		break;
	}
	vec3Scale.x = ((float)fCurrValue / fTotalValue);
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CProgressBar::PreRender(void)
{
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
void CProgressBar::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CProgressBar::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	unsigned int transformLoc;
	// get matrix's uniform location and set matrix
	transformLoc = glGetUniformLocation(cShader->ID, "transform");
	// Reset the transform
	transform = glm::mat4(1.0f);
	// Translate to the position to render. Note that the centrepoint of the progressbar will be at this position
	// So we need to offset to the right by the vec3Scale.x * fWidth
	transform = glm::translate(transform, glm::vec3(vec3Position.x + vec3Scale.x * fWidth, vec3Position.y, vec3Position.z));
	// Scale the size of the progressbar by vec3Scale. 
	// Note that .y and .z should be 1.0f if you are not scaling them at all
	transform = glm::scale(transform, vec3Scale);

	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Render the quad
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Reset to default
	glBindVertexArray(0);

	// get matrix's uniform location and set matrix
	transformLoc = glGetUniformLocation(cShader->ID, "transform");
	// Reset the transform
	transform = glm::mat4(1.0f);
	// Translate to the position to render. Note that the centrepoint of the progressbar will be at this position
	// So we need to offset to the right by the vec3Scale.x * fWidth
	transform = glm::translate(transform, glm::vec3(vec3Position.x + fWidth, vec3Position.y, vec3Position.z));

	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Render the quad
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_LOOP, 0, 6);

	// Reset to default
	glBindVertexArray(0);

}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CProgressBar::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

void CProgressBar::SetValue(const int iCurrentValue)
{
	this->fCurrValue = iCurrentValue;
	
}

void CProgressBar::SetTotalValue(const int iTotalValue)
{
	this->fTotalValue = iTotalValue;
}

int CProgressBar::GetValue(void) const
{
	return fCurrValue;
}

int CProgressBar::GetTotalValue(void) const
{
	return fTotalValue;
}
