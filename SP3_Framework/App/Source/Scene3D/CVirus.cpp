#include "CVirus.h"

#include "Player3D.h"
#include "Score.h"

#include <iostream>

using namespace std;

CVirus::CVirus(void)
	: bActive(true)
	//, cPlayer3D(NULL)

{
}

CVirus::CVirus(glm::uvec2 etype, int numofvirus)
	: bActive(true)
	//, cPlayer3D(NULL)
{
	type = etype;
	numOfVirus = numofvirus;
}

CVirus::~CVirus(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cShader = NULL;

	//cPlayer3D = NULL;
}

bool CVirus::Init(void)
{
	if (!cShader)
	{
		cout << "HUD::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	//cPlayer3D = CPlayer3D::GetInstance();

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// positions			// texture coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
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

	vec3Scale.x = 0.1f * 0.2;
	vec3Scale.y = 0.168f * 0.2;
	vec3Position = glm::vec3(-0.27f + ((numOfVirus - 1) * (2 * vec3Scale.x + 0.02)), 0.93f, 0.0f);
	vec3ColliderScale = glm::vec3(0, 0, 0);

	// load and create a texture 

	switch (type.x) // buff or debuff
	{
	case 0:
	{
		switch (type.y) // buff
		{
		case T_HP_INCREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/health_increase.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/health_increase.tga" << endl;
				return false;
			}
			break;
		}
		case T_SPEED_INCREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/movement_speed_increase.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/movement_speed_increase.tga" << endl;
				return false;
			}
			break;
		}
		case T_RELOAD_SPEED_INCREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/reload_speed_increase.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/reload_speed_increase.tga" << endl;
				return false;
			}
			break;
		}
		case T_SCORE_MULTIPLIER_INCREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/score_multiplier_increase.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/score_multiplier_increase.tga" << endl;
				return false;
			}
			break;
		}
		case T_WEAPON_DAMAGE_INCREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/damage_increase.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/damage_increase.tga" << endl;
				return false;
			}
			break;
		}
		break;
		}
	}
	case 1:
	{
		switch (type.y) // debuff
		{
		case T_HP_DECREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/health_decrease.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/health_decrease.tga" << endl;
				return false;
			}
			break;
		}
		case T_SPEED_DECREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/movement_speed_decrease.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/movement_speed_decrease.tga" << endl;
				return false;
			}
			break;
		}
		case T_RELOAD_SPEED_DECREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/reload_speed_decrease.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/reload_speed_decrease.tga" << endl;
				return false;
			}
			break;
		}
		case T_SCORE_MULTIPLIER_DECREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/score_multiplier_decrease.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/score_multiplier_decrease.tga" << endl;
				return false;
			}
			break;
		}
		case T_WEAPON_DAMAGE_DECREASE:
		{
			iTextureID = LoadTexture("Images/buff_hud/damage_decrease.tga");
			if (iTextureID == 0)
			{
				cout << "Unable to load ImageS/buff_hud/damage_decrease.tga" << endl;
				return false;
			}
			break;
		}
		break;
		}
	}
	}

	
	

	return true;
}

void CVirus::SetModel(glm::mat4 model)
{
	this->model = model;
}

void CVirus::SetView(glm::mat4 view)
{
	this->view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
}

void CVirus::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

void CVirus::SetStatus(const bool bStatus)
{
	this->bActive = bStatus;
}

bool CVirus::GetStatus(void) const
{
	return bActive;
}

void CVirus::Update(const double dElapsedTime)
{
	switch (type.x) // buff or debuff
	{
	case 0:
	{
		switch (type.y) // buff
		{
		case T_HP_INCREASE:
		{
			// increase max and curr health by 20
			CPlayer3D::GetInstance()->SetMaxHealth(CPlayer3D::GetInstance()->GetMaxHealth() + 20);
			CPlayer3D::GetInstance()->SetCurrHealth(CPlayer3D::GetInstance()->GetCurrHealth() + 20);
			break;
		}
		case T_SPEED_INCREASE:
		{
			// increase speed by 10% 
			CPlayer3D::GetInstance()->SetSpeed(CPlayer3D::GetInstance()->GetSpeed() * 1.10f); 
			break;
		}
		case T_RELOAD_SPEED_INCREASE:
		{
			// increase reload time by 10%
			CPlayer3D::GetInstance()->SetReloadSpeedMultiplier(CPlayer3D::GetInstance()->GetReloadSpeedMultiplier() * 0.90f);
			break;
		}
		case T_SCORE_MULTIPLIER_INCREASE:
		{
			// increase multiplier by 20%
			CScore::GetInstance()->SetMultiplier(CScore::GetInstance()->GetMultiplier() * 1.20f);
			break;
		}
		case T_WEAPON_DAMAGE_INCREASE:
		{
			// increase damage by 10%
			CPlayer3D::GetInstance()->SetDmageMultiplier(CPlayer3D::GetInstance()->GetDmageMultiplier() * 1.10f);
			break;
		}
		break;
		}
	}
	case 1:
	{
		switch (type.y) // debuff
		{
		case T_HP_DECREASE:
		{
			CPlayer3D::GetInstance()->SetMaxHealth(CPlayer3D::GetInstance()->GetMaxHealth() - 10);
			CPlayer3D::GetInstance()->SetCurrHealth(CPlayer3D::GetInstance()->GetCurrHealth() - 10);
			break;
		}
		case T_SPEED_DECREASE:
		{
			CPlayer3D::GetInstance()->SetSpeed(CPlayer3D::GetInstance()->GetSpeed() * 0.90f);
			break;
		}
		case T_RELOAD_SPEED_DECREASE:
		{
			// decrease reload time by 10%
			CPlayer3D::GetInstance()->SetReloadSpeedMultiplier(CPlayer3D::GetInstance()->GetReloadSpeedMultiplier() * 1.10f);
			break;
		}
		case T_SCORE_MULTIPLIER_DECREASE:
		{
			// decrease multiplier by 20%
			CScore::GetInstance()->SetMultiplier(CScore::GetInstance()->GetMultiplier() * 0.90f);
			break;
		}
		case T_WEAPON_DAMAGE_DECREASE:
		{
			// increase damage by 10%
			CPlayer3D::GetInstance()->SetDmageMultiplier(CPlayer3D::GetInstance()->GetDmageMultiplier() * 0.90f);
			break;
		}
		break;
		}
	}
	}
}

void CVirus::PreRender(void)
{
	if (!bActive)
		return;

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CVirus::Render(void)
{
	//cout << "fkfkfkfkfk" << endl;
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CCameraEffects::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	if (!bActive)
		return;

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

void CVirus::PostRender(void)
{
	if (!bActive)
		return;

	// Disable blending
	glDisable(GL_BLEND);
}
