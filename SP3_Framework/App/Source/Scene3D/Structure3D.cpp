/**
 CStructure3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Structure3D.h"

 // Include Meshbuilder and Mtxx44 for rendering
#include "../Meshes/MeshBuilder.h"
#include "../Meshes/Mtx44.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CStructure3D::CStructure3D(void)
	: cGroundMap(NULL)
	, iStructureHealth(100)
	, cPlayer3D(NULL)
	, fYaw(-90.0f)
	, fPitch(0.0f)
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
CStructure3D::CStructure3D(const glm::vec3 vec3Position, CEntity3D::TYPE type, const glm::vec3 vec3Front, const float fYaw, const float fPitch)
	: cGroundMap(NULL)
	, iStructureHealth(100)
	, cPlayer3D(NULL)
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, bRotateEnabled(false)
{
	// Set the default position to the origin
	this->vec3Position = vec3Position;
	SetType(type);
}


/**
 @brief Destructor
 */
CStructure3D::~CStructure3D(void)
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
bool CStructure3D::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CStructure3D::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();


	// Store the handler to the CGroundMap
	cGroundMap = CGroundMap::GetInstance();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	fYaw = -90.0f;
	fPitch = 0.0f;

	cPlayer3D = CPlayer3D::GetInstance();

	// switch type of structure
	switch (eType)
	{
	case EXPLOSIVE_BARREL:
	{
		vec3Scale = glm::vec3(0.02, 0.02, 0.02); // OBJ scale
		vec3ColliderScale = glm::vec3(1.0, 1.0, 1.0); // collider scale

		std::string file_path = "OBJ/TrainingBot.obj";
		bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
		{
			return NULL;
		}
		break;
	}
		
	case PLAY_BUTTON:
	case HIGHSCORE_BUTTON:
	case QUIT_BUTTON:
	{
		// init structureMesh
		vec3Scale = glm::vec3(0.1f, 0.1f, 0.1f); // OBJ scale
		vec3ColliderScale = glm::vec3(0.5, 0.5, 0.5); // collider scale
		// load structureMesh OBJ
		std::string file_path = "OBJ/button.obj";
		bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
		{
			return NULL;
		}

		bRotateEnabled = true;
		break;
	}
	default:
	{
		vec3Scale = glm::vec3(0.02, 0.02, 0.02); // OBJ scale
		vec3ColliderScale = glm::vec3(1.0, 1.0, 1.0); // collider scale

		std::string file_path = "OBJ/TrainingBot.obj";
		bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
		{
			return NULL;
		}
		break;
	}
	
	}

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	index_buffer_size = index_buffer_data.size();

	switch (eType)
	{
	case EXPLOSIVE_BARREL:
		iTextureID = LoadTexture("Images/TrainingBot.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load texture for structure " << endl;
			return false;
		}
		break;
	case PLAY_BUTTON:
		iTextureID = LoadTexture("Images/playbutton.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load texture for structure " << endl;
			return false;
		}
		break;
	case HIGHSCORE_BUTTON:
		iTextureID = LoadTexture("Images/highscorebutton.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load texture for structure " << endl;
			return false;
		}
		break;
	case QUIT_BUTTON:
		iTextureID = LoadTexture("Images/quitbutton.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load texture for structure " << endl;
			return false;
		}
		break;
	}

	

	return true;
}


/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CStructure3D::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CStructure3D::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CStructure3D::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Update this class instance
*/
void CStructure3D::Update(const double dElapsedTime)
{
}

/**
@brief Activate the CCollider for this class instance
@param cLineShader A Shader* variable which stores a shader which renders lines
*/
void CStructure3D::ActivateCollider(Shader* cLineShader)
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
void CStructure3D::PreRender(void)
{
	// Draw this as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

/**
@brief Render Render this instance
@param cShader A Shader* variable which contains the Shader to use in this class instance
*/
void CStructure3D::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CStructure3D::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	if (bRotateEnabled)
	{
		RotateToPlayer();
		model = glm::rotate(model, glm::radians(-fYaw + 90.f), glm::vec3(0.f, 1.f, 0.f));
	}
	model = glm::scale(model, vec3Scale);
	

	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	cShader->setMat4("projection", projection);
	cShader->setMat4("view", view);
	cShader->setMat4("model", model);

	// render OBJ
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, index_buffer_size, GL_UNSIGNED_INT, 0);


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// Render the CCollider if needed
	if ((cCollider) && (cCollider->bIsDisplayed))
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, vec3Position);
		if (bRotateEnabled)
		{
			model = glm::rotate(model, glm::radians(-fYaw + 90.f), glm::vec3(0.f, 1.f, 0.f));
		}
		model = glm::scale(model, vec3ColliderScale);

		cCollider->model = model;
		cCollider->view = view;
		cCollider->projection = projection;
		cCollider->Render();
	}
	
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CStructure3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

void CStructure3D::SetHealth(const int iStructureHealth)
{
	this->iStructureHealth = iStructureHealth;
}

int CStructure3D::GetHealth()
{
	return iStructureHealth;
}

void CStructure3D::RotateToPlayer()
{
	//if (bRotateEnabled)
	
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front = glm::normalize(front);

	// Check if we are too far from the player
	if (cPlayer3D)
	{
		// Update the direction of the enemy
		front = glm::normalize(glm::vec3(cPlayer3D->GetPosition() - vec3Position));

		// Update the yaw and pitch
		fYaw = glm::degrees(glm::atan(front.z, front.x));
		fPitch = glm::degrees(glm::asin(front.y));
	}

	vec3Front = front;
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));
	
}
