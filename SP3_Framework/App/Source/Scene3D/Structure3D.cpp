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

	// switch type of structure
	switch (eType)
	{
	case PISTOL_AMMO:
	{
		 // init structureMesh
		 vec3Scale = glm::vec3(0.2, 0.2, 0.2); // OBJ scale
		 vec3ColliderScale = glm::vec3(1.0, 1.0, 1.0); // collider scale
		 // load structureMesh OBJ
		 structureMesh = MeshBuilder::GenerateOBJ("enemy", "OBJ/pistol.obj");
		 // load and create a texture 
		 iTextureID = LoadTexture("Images/pistol.tga");
		 if (iTextureID == 0)
		 {
			 cout << "Unable to load Images/pistol.tga" << endl;
			 return false;
		 }
	}
		break;
	case RIFLE_AMMO:
	{
		 // init structureMesh
		 vec3Scale = glm::vec3(0.02, 0.02, 0.02); // OBJ scale
		 vec3ColliderScale = glm::vec3(1.0, 1.0, 1.0); // collider scale
		 // load structureMesh OBJ
		 structureMesh = MeshBuilder::GenerateOBJ("enemy", "OBJ/rifle.obj");
		 // load and create a texture 
		 iTextureID = LoadTexture("Images/rifle.tga");
		 if (iTextureID == 0)
		 {
			 cout << "Unable to load Images/rifle.tga" << endl;
			 return false;
		 }
	}
		break;
	default:
	{
		// init structureMesh
		vec3Scale = glm::vec3(0.02, 0.02, 0.02); // OBJ scale
		vec3ColliderScale = glm::vec3(1.0, 1.0, 1.0); // collider scale
		// load structureMesh OBJ
		structureMesh = MeshBuilder::GenerateOBJ("enemy", "OBJ/rifle.obj");
		// load and create a texture 
		iTextureID = LoadTexture("Images/rifle.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load Images/rifle.tga" << endl;
			return false;
		}
	}
		break;
	}

	// Store the handler to the CGroundMap
	cGroundMap = CGroundMap::GetInstance();

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

void CStructure3D::RenderMesh(Mesh* mesh)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// create transformations
	switch (eType)
	{
	case PISTOL_AMMO:
	{
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
		model = glm::rotate(model, (float)90.f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, vec3Scale);
		break;
	}

	case RIFLE_AMMO:
	{
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, vec3Position);
		model = glm::scale(model, vec3Scale);
		break;
	}

	default:
	{
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, vec3Position);
		model = glm::scale(model, vec3Scale);
		break;
	}

	}

	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	cShader->setMat4("projection", projection);
	cShader->setMat4("view", view);
	cShader->setMat4("model", model);

	// render OBJ
	mesh->Render();
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

	modelStack.PushMatrix();
	RenderMesh(structureMesh);
	modelStack.PopMatrix();
	
	// Render the CCollider if needed
	/*if ((cCollider) && (cCollider->bIsDisplayed))
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, vec3Position);
		model = glm::scale(model, vec3ColliderScale);

		cCollider->model = model;
		cCollider->view = view;
		cCollider->projection = projection;
		cCollider->Render();
	}*/
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CStructure3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}
