#include "RangeIndicator.h"

#include "../../Meshes/MeshBuilder.h"
#include "../../Meshes/Mtx44.h"

#include <iostream>
using namespace std;

CRangeIndicator::CRangeIndicator(void) : indicatorMesh(NULL)/*, currEnemy(NULL)*/
{
	this->vec3Position = glm::vec3(1, 1, 1);
	this->type = Enemy_Type::E_ENEMY1;
}

CRangeIndicator::CRangeIndicator(const glm::vec3 vec3Position, int enemy_type, CEnemy3D* currenemy) : indicatorMesh(NULL)
{
	this->vec3Position = vec3Position;
	this->type = enemy_type;
	this->currEnemy = currenemy;
}

CRangeIndicator::~CRangeIndicator(void)
{
	if (indicatorMesh)
	{
		delete indicatorMesh;
	}
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

	switch (type)
	{
	case E_ENEMY1:
	{
		vec3Scale = glm::vec3(1.5, 1.5, 1.5);
		vec3ColliderScale = glm::vec3(2, 0.2, 2);

		indicatorMesh = MeshBuilder::GenerateOBJ("enemy", "OBJ/range.obj");
		// load and create a texture 
		iTextureID = LoadTexture("Images/chicken.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load Images/chicken.tga" << endl;
			return false;
		}

		break;
	}
	case E_ENEMY2:
	{
		vec3Scale = glm::vec3(1, 1, 1);
		vec3ColliderScale = glm::vec3(1.3, 0.2, 1.3);

		indicatorMesh = MeshBuilder::GenerateOBJ("enemy", "OBJ/range.obj");
		// load and create a texture 
		iTextureID = LoadTexture("Images/chicken.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load Images/chicken.tga" << endl;
			return false;
		}

		break;
	}
	case E_ENEMY3:
	{
		vec3Scale = glm::vec3(1, 1, 1);
		vec3ColliderScale = glm::vec3(1, 0.2, 1);

		indicatorMesh = MeshBuilder::GenerateOBJ("enemy", "OBJ/range.obj");
		// load and create a texture 
		iTextureID = LoadTexture("Images/chicken.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load Images/chicken.tga" << endl;
			return false;
		}

		break;
	}
	default:
	{
		vec3Scale = glm::vec3(1, 1, 1);
		vec3ColliderScale = glm::vec3(1, 0.2, 1);

		indicatorMesh = MeshBuilder::GenerateOBJ("enemy", "OBJ/chicken.obj");
		// load and create a texture 
		iTextureID = LoadTexture("Images/chicken.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load Images/chicken.tga" << endl;
			return false;
		}

		break;
	}

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
	if (currEnemy->GetHealth() <= 0)
	{
		SetToDelete(true);
		return;
	}
	else
	{
		vec3Position = currEnemy->GetPosition();
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

void CRangeIndicator::RenderMesh(Mesh* mesh)
{
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
	mesh->Render();
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
	modelStack.PushMatrix();
	RenderMesh(indicatorMesh);
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

void CRangeIndicator::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

CEnemy3D* CRangeIndicator::GetCurrEnemy(void)
{
	return currEnemy;
}
