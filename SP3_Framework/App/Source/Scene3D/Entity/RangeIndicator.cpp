#include "RangeIndicator.h"

#include "../../Meshes/MeshBuilder.h"
#include "../../Meshes/Mtx44.h"

#include <iostream>
using namespace std;

CRangeIndicator::CRangeIndicator(void)
{
	this->vec3Position = glm::vec3(1, 1, 1);
	this->type = Enemy_Type::E_ENEMY1;
}

CRangeIndicator::CRangeIndicator(const glm::vec3 vec3Position, int enemy_type, CEnemy3D* currenemy)
{
	this->vec3Position = vec3Position;
	this->type = enemy_type;
	this->currEnemy = currenemy;
}

CRangeIndicator::~CRangeIndicator(void)
{
	
}

bool CRangeIndicator::Init(void)
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
	SetType(CEntity3D::TYPE::NPC_ATTACK_RANGE);

	vec3Position = currEnemy->GetPosition();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	
	switch (type)
	{
	case E_ENEMY1:
	{
		vec3Scale = glm::vec3(2, 2, 2);
		vec3ColliderScale = glm::vec3(4, 0.1, 4);
		

		std::string file_path = "OBJ/range.obj";
		bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
		{
			return NULL;
		}


		break;
	}
	case E_ENEMY2:
	{
		vec3Scale = glm::vec3(1, 1, 1);
		vec3ColliderScale = glm::vec3(2, 0.2, 2);

		std::string file_path = "OBJ/range.obj";
		bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
		{
			return NULL;
		}

		break;
	}
	case E_ENEMY3:
	{
		vec3Scale = glm::vec3(0, 0, 0);
		vec3ColliderScale = glm::vec3(0, 0, 0);

		std::string file_path = "OBJ/range.obj";
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

	iTextureID = LoadTexture("Images/chicken.tga");
	if (iTextureID == 0)
	{
		cout << "Unable to load Images/chicken.tga" << endl;
		return false;
	}

	return true;
}

void CRangeIndicator::SetModel(glm::mat4 model)
{
	this->model = model;
}

void CRangeIndicator::SetView(glm::mat4 view)
{
	this->view = view;

}

void CRangeIndicator::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

void CRangeIndicator::Update(const double dElapsedTime)
{
	if (currEnemy->GetHealth() > 0)
	{
		vec3Position = currEnemy->GetPosition();
	}
	else
	{
		SetToDelete(true);
		return;
	}
	
}

void CRangeIndicator::ActivateCollider(Shader* cLineShader)
{
	// Since we are changing the colour, we don't call the parent's ActivateCollider() method.
	// Instead we create it here and insert our colour changing codes
	//CEntity3D::ActivateCollider(cLineShader);

	// Create a new CCollider
	cCollider = new CCollider();
	// Set the colour of the CCollider to Blue
	cCollider->vec4Colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	// Initialise the cCollider
	cCollider->Init();
	// Set a shader to it
	cCollider->SetLineShader(cLineShader);
}

void CRangeIndicator::PreRender(void)
{
	// Draw this as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

void CRangeIndicator::Render(void)
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

void CRangeIndicator::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

CEnemy3D* CRangeIndicator::GetCurrEnemy(void)
{
	return currEnemy;
}
