#include "Poison3D.h"

#include "../../Meshes/MeshBuilder.h"
#include "../../Meshes/Mtx44.h"

#include <iostream>
using namespace std;

CPoison3D::CPoison3D(void)
{
	this->vec3Position = glm::vec3(1, 1, 1);
	this->type = glm::uvec2(0,2);
}

CPoison3D::CPoison3D(const glm::vec3 vec3Position, glm::uvec2 entity_type, CEntity3D* currenemy)
{
	this->vec3Position = vec3Position;
	this->type = entity_type;
	this->currEnemy = currenemy;
}

CPoison3D::~CPoison3D(void)
{
	
}

bool CPoison3D::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CEnemy3D::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::POISON);

	vec3Position = currEnemy->GetPosition();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;


	switch (type.x)
	{
	case 0: // enemy
	{
		switch (type.y)
		{
		case Enemy_Type::T_ENEMY1:
		{
			colliderSize = 2.0f;
			break;
		}
		case Enemy_Type::T_ENEMY2:
		{
			colliderSize = 1.0f;

			break;
		}
		case Enemy_Type::T_ENEMY3:
		{
			colliderSize = 0.0f;

			break;
		}
		}
		break;
	}
	case 1: // boss
	{
		switch (type.y)
		{
		case Boss_Type::T_BOSS1:
		{
			colliderSize = 4.0f;
			break;
		}
		}
	}
	}

	
	vec3Scale = glm::vec3(colliderSize, colliderSize, colliderSize);
	vec3ColliderScale = glm::vec3(1.f, 1.f, 1.f);

	std::string file_path = "OBJ/range.obj";
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
	{
		return NULL;
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

	iTextureID = LoadTexture("Images/poison.tga");
	if (iTextureID == 0)
	{
		cout << "Unable to load Images/chicken.tga" << endl;
		return false;
	}

	return true;
}

void CPoison3D::SetModel(glm::mat4 model)
{
	this->model = model;
}

void CPoison3D::SetView(glm::mat4 view)
{
	this->view = view;

}

void CPoison3D::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

void CPoison3D::Update(const double dElapsedTime)
{
	
	if(currEnemy->GetHealth() < 1 || currEnemy->GetHealth() > currEnemy->GetMaxHealth())
	{
		SetToDelete(true);
		return;
	}

	if (currEnemy->GetHealth() > 0)
	{
		vec3Position = currEnemy->GetPosition();
	}
	
}

void CPoison3D::ActivateCollider(Shader* cLineShader)
{
	// Since we are changing the colour, we don't call the parent's ActivateCollider() method.
	// Instead we create it here and insert our colour changing codes
	//CEntity3D::ActivateCollider(cLineShader);

	// Create a new CCollider
	cCollider = new CCollider();
	// Set the colour of the CCollider to Blue
	cCollider->vec4Colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	// scale the collider
	cCollider->vec3BottomLeft = glm::vec3(-colliderSize, -colliderSize, -colliderSize);
	cCollider->vec3TopRight = glm::vec3(colliderSize, colliderSize, colliderSize);

	// Initialise the cCollider
	cCollider->Init();

	// Set a shader to it
	cCollider->SetLineShader(cLineShader);
}

void CPoison3D::PreRender(void)
{
	// Draw this as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

void CPoison3D::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CEnemy3D::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	// render enemyMesh
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y - 0.49f, vec3Position.z));
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
		model = glm::scale(model, vec3ColliderScale);

		cCollider->model = model;
		cCollider->view = view;
		cCollider->projection = projection;
		cCollider->Render();
	}
}

void CPoison3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

CEntity3D* CPoison3D::GetCurrEnemy(void)
{
	return currEnemy;
}
