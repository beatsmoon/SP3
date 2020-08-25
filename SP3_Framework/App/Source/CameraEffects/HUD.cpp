#include "HUD.h"

#include <iostream>

using namespace std;

CHUD::CHUD(void)
    : bActive(true)
	, cPlayer3D(NULL)
	
{
}

CHUD::CHUD(Weapon_Type etype) 
	: bActive(true)
	, cPlayer3D(NULL)
{
	prevtype = type = etype;
}

CHUD::~CHUD(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cShader = NULL;

	cPlayer3D = NULL;
}

bool CHUD::Init(void)
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

	cPlayer3D = CPlayer3D::GetInstance();

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

	//switch (type)
	//{
	//case W_PISTOL:
	//	vec3Position = glm::vec3(0.5f, -0.5f, 0.0f);
	//	vec3Scale.x = 0.5;
	//	vec3Scale.y = 0.5;
	//	// load and create a texture 
	//	iTextureID = LoadTexture("Images/GUI/holding_rifle.tga");
	//	if (iTextureID == 0)
	//	{
	//		cout << "Unable to load ImageS/GUI/holding_rifle.tga" << endl;
	//		return false;
	//	}

	//	break;
	//case H_SELECTION_GUN:
	//	vec3Position = glm::vec3(0.8, -0.8, 0.0f);
	//	vec3Scale.x = 0.1;
	//	vec3Scale.y = 0.1;
	//	// load and create a texture 
	//	iTextureID = LoadTexture("Images/GUI/rifle_HUD.tga");
	//	if (iTextureID == 0)
	//	{
	//		cout << "Unable to load ImageS/GUI/rifle_HUD.tga" << endl;
	//		return false;
	//	}
	//	break;
	//default:
	//	break;
	//}
	vec3Position = glm::vec3(0.5f, -0.9f, 0.0f);
	vec3Scale.x = 0.5;
	vec3Scale.y = 0.8;
	type = prevtype = Weapon_Type::W_PISTOL;
	tPrevBarrel = tBarrel = CGunBarrel::TIERLEVEL::NOTIER;
	tPrevScope = tScope = CGunScope::TIERLEVEL::NOTIER;
	return true;
}

void CHUD::SetModel(glm::mat4 model)
{
	this->model = model;
}

void CHUD::SetView(glm::mat4 view)
{
	this->view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
}

void CHUD::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

void CHUD::SetStatus(const bool bStatus)
{
	this->bActive = bStatus;
}

bool CHUD::GetStatus(void) const
{
    return bActive;
}
void CHUD::UpdateScopeVisualStatus()
{
	tScope = cPlayer3D->GetWeapon()->GetScope()->GetTierLevel();
	if (tScope == tPrevScope)
	{
		return;
	}
	tPrevScope = tScope;

	type = cPlayer3D->GetWeapon()->GetWeaponName();
	if (type == prevtype)
	{
		return;
	}
	prevtype = type;
	switch (type)
	{
	case Weapon_Type::W_PISTOL:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_Pistol_Barreled.tga");
	}
	break;
	case Weapon_Type::W_SHOTGUN:
	{
		iTextureID = LoadTexture("Images/GUI/ScopeAttachment/Scene3D_Holding_Shotgun_Scoped.tga");
	}
	break;
	case Weapon_Type::W_AK47:
	{
		iTextureID = LoadTexture("Images/GUI/ScopeAttachment/Scene3D_Holding_AK47_Scoped.tga");
	}
	break;
	case Weapon_Type::W_M4:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_M4.tga");
	}
	break;
	case Weapon_Type::W_SNIPER:
	{
		iTextureID = LoadTexture("Images/GUI/ScopeAttachment/Scene3D_Holding_Sniper_Barreled.tga");
	}
	break;
	}
}
void CHUD::UpdateFullStatus()
{
	tScope = cPlayer3D->GetWeapon()->GetScope()->GetTierLevel();
	if (tScope == tPrevScope)
	{
		return;
	}
	tPrevScope = tScope;

	type = cPlayer3D->GetWeapon()->GetWeaponName();
	if (type == prevtype)
	{
		return;
	}
	prevtype = type;
	switch (type)
	{
	case Weapon_Type::W_PISTOL:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_Pistol_Barreled.tga");
	}
	break;
	case Weapon_Type::W_SHOTGUN:
	{
		iTextureID = LoadTexture("Images/GUI/FullAttachment/Scene3D_Holding_Shotgun_Full.tga");
	}
	break;
	case Weapon_Type::W_AK47:
	{
		iTextureID = LoadTexture("Images/GUI/FullAttachment/Scene3D_Holding_AK47_Full.tga");
	}
	break;
	case Weapon_Type::W_M4:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_M4.tga");
	}
	break;
	case Weapon_Type::W_SNIPER:
	{
		iTextureID = LoadTexture("Images/GUI/ScopeAttachment/Scene3D_Holding_Sniper_Barreled.tga");
	}
	break;
	}
}
void CHUD::UpdateBarrelVisualStatus()
{
	tBarrel = cPlayer3D->GetWeapon()->GetBarrel()->GetTierLevel();
	if (tBarrel == tPrevBarrel)
	{
		return;
	}
	tPrevBarrel = tBarrel;

	type = cPlayer3D->GetWeapon()->GetWeaponName();
	if (type == prevtype)
	{
		return;
	}
	prevtype = type;
	switch (type)
	{
	case Weapon_Type::W_PISTOL:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_Pistol_Barreled.tga");
	}
	break;
	case Weapon_Type::W_SHOTGUN:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_Shotgun_Barreled.tga");
	}
	break;
	case Weapon_Type::W_AK47:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_AK47_Barreled.tga");
	}
	break;
	case Weapon_Type::W_M4:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_M4.tga");
	}
	break;
	case Weapon_Type::W_SNIPER:
	{
		iTextureID = LoadTexture("Images/GUI/BarrelAttachment/Scene3D_Holding_Sniper_Barreled.tga");
	}
	break;
	}
}
void CHUD::Update(const double dElapsedTime)
{
	if (cPlayer3D->GetWeapon()->GetBarrel() != NULL && cPlayer3D->GetWeapon()->GetScope() == NULL)
	{
		//if gun only have barrel 
		UpdateBarrelVisualStatus();
	}
	else if (cPlayer3D->GetWeapon()->GetBarrel() == NULL && cPlayer3D->GetWeapon()->GetScope() != NULL)
	{
		//if gun only have scope
		UpdateScopeVisualStatus();
	}
	else if (cPlayer3D->GetWeapon()->GetBarrel() != NULL && cPlayer3D->GetWeapon()->GetScope() != NULL)
	{
		//if gun only have scope
		UpdateFullStatus();
	}
	else
	{
		type = cPlayer3D->GetWeapon()->GetWeaponName();
		if (type == prevtype)
		{
			return;
		}
		prevtype = type;
		switch (type)
		{
		case Weapon_Type::W_PISTOL:
		{
			iTextureID = LoadTexture("Images/GUI/NoAttachment/Scene3D_Holding_Pistol.tga");
		}
		break;
		case Weapon_Type::W_SHOTGUN:
		{
			iTextureID = LoadTexture("Images/GUI/NoAttachment/Scene3D_Holding_Shotgun.tga");
		}
		break;
		case Weapon_Type::W_AK47:
		{
			iTextureID = LoadTexture("Images/GUI/NoAttachment/Scene3D_Holding_AK47.tga");
		}
		break;
		case Weapon_Type::W_M4:
		{
			iTextureID = LoadTexture("Images/GUI/Scene3D_Holding_M4.tga");
		}
		break;
		case Weapon_Type::W_SNIPER:
		{
			iTextureID = LoadTexture("Images/GUI/NoAttachment/Scene3D_Holding_Sniper.tga");
		}
		break;
		}
	}
}

void CHUD::PreRender(void)
{
	if (!bActive)
		return;

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CHUD::Render(void)
{
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

void CHUD::PostRender(void)
{
	if (!bActive)
		return;

	// Disable blending
	glDisable(GL_BLEND);
}
