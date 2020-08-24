#include "Shop.h"

#include <iostream>

using namespace std;

CShop::CShop(void)
	: bActive(true)
	, cPlayer3D(NULL)
	, bShopActive(false)
	, cMouseController(NULL)
{
}

CShop::~CShop(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// We set this to NULL, since it was created elsewhere so we don't delete it here
	cShader = NULL;

	cPlayer3D = NULL;

	if (cMouseController)
	{
		cMouseController->Destroy();
		cMouseController = NULL;
	}
}

bool CShop::Init(void)
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

	cMouseController = CMouseController::GetInstance();

	cSettings = CSettings::GetInstance();

	// Setup the shaders
	cSimpleShader = new Shader("Shader//SimpleShader.vs", "Shader//SimpleShader.fs");

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

	// load and create a texture
	iTextureID = LoadTexture("Images/GUI/ShopUIv4.tga");
	if (iTextureID == 0)
	{
		cout << "Unable to load Images/GUI/ShopUI.tga" << endl;
		return false;
	}


	return true;
}

void CShop::SetModel(glm::mat4 model)
{
	this->model = model;
}

void CShop::SetView(glm::mat4 view)
{
	this->view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
}

void CShop::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

//Check if Shop UI is active
bool CShop::GetStatus(void) const
{
    return bShopActive;
}

void CShop::Update(const double dElapsedTime)
{
	double dMouse_X, dMouse_Y;
	glfwGetCursorPos(cSettings->pWindow, &dMouse_X, &dMouse_Y);
	CMouseController::GetInstance()->UpdateMousePosition(dMouse_X, dMouse_Y);

	int m_windowWidth = cSettings->iWindowWidth;
	int m_windowHeight = cSettings->iWindowHeight;

	static float bouncetime = 0.0f;

	//Logic for buying in Shop UI
	if (bShopActive)
	{
		if (cMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::LMB) && bouncetime < GetTickCount64())
		{
			cout << "Clicked in shop" << endl;
			cout << "m_windowwidth: " << m_windowWidth << ", m_windowHeight: " << m_windowHeight << endl;
			//Top Left Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.1 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.2625)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.125 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.375))
			{
				cout << "Top Left Box" << endl;

				//Player buys Sniper
				delete cPlayer3D->GetInventoryWeapon(0);

				CWeapon* Sniper = new CWeapon(Weapon_Type::W_SNIPER);
				Sniper->Init();
				Sniper->SetShader(cSimpleShader);
				cPlayer3D->SetWeapon(0, Sniper);

			}


			//Top Right Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.7375 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.9)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.125 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.375))
			{
				cout << "Top Right Box" << endl;


				//Player buys AK47
				delete cPlayer3D->GetInventoryWeapon(0);

				CWeapon* AK47 = new CWeapon(Weapon_Type::W_AK47);
				AK47->Init();
				AK47->SetShader(cSimpleShader);
				cPlayer3D->SetWeapon(0, AK47);

			}

			//Top Middle Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.41875 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.58125)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.125 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.375))
			{
				cout << "Middle Box" << endl;

				//Player buys shotgun
				delete cPlayer3D->GetInventoryWeapon(0);

				CWeapon* Shotgun = new CWeapon(Weapon_Type::W_SHOTGUN);
				Shotgun->Init();
				Shotgun->SetShader(cSimpleShader);
				cPlayer3D->SetWeapon(0, Shotgun);
			}


			//Middle Left Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.08984 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.22265)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.51315 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.67105))
			{
				cout << "Middle Left Box" << endl;
				CGunExtMag* cExtendedMag = new CGunExtMag();
				cExtendedMag->Init();
				cExtendedMag->SetTierLevel(CGunExtMag::TIERLEVEL::TIER1);
				cPlayer3D->GetInventoryWeapon(0)->SetExtMag(cExtendedMag);
				cPlayer3D->GetInventoryWeapon(0)->SetMaxMagRound(cPlayer3D->GetInventoryWeapon(0)->GetMaxMagRound() + cPlayer3D->GetInventoryWeapon(0)->GetExtMag()->GetAdditionMag());
				cout << static_cast<int>(cExtendedMag->GetTierLevel());

				
			}

			//Middle Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.31641 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.44921)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.51315 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.67105))
			{
				cout << "Middle  Box" << endl;

				CGunScope* cScope = new CGunScope();
				cScope->Init();
				cScope->SetTierLevel(CGunScope::TIERLEVEL::TIER1);

				cPlayer3D->GetInventoryWeapon(0)->SetScope(cScope);

			}

			//Middle Right Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.54297 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.67578)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.51315 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.67105))
			{
				cout << "Middle Right Box" << endl;

				//Player buys barrel attachment;
				CGunBarrel* cGunBarrel = new CGunBarrel();
				cGunBarrel->Init();

				//cGunBarrel->SetTierLevel(static_cast<CGunBarrel::TIERLEVEL>(cPlayer3D->GetInventoryWeapon(0)->GetBarrel()->GetTierLevel()+ 1));
				//cGunBarrel->SetTierLevel(cPlayer3D->GetInventoryWeapon(0)->GetBarrel()->SetTier());


				cout << static_cast<int>(cGunBarrel->GetTierLevel()) << "Recoil Reduction is:" << cGunBarrel->GetRecoilReduction();

				cPlayer3D->GetInventoryWeapon(0)->SetBarrel(cGunBarrel);
			}


			

			//Bottom Left Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.076562 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.1625)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.76973 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.92105))
			{
				cout << "Ammo Bought" << endl;
				//Player buys Ammo
				cPlayer3D->GetInventoryWeapon(0)->AddRounds(30);
			}

			bouncetime = GetTickCount64() + 500;

		}
	}
}

//Activate Shop UI
void CShop::ActivateShop(void)
{
	bShopActive = true;
}

//Deactivate Shop UI
void CShop::DeactivateShop(void)
{
	bShopActive = false;
}

void CShop::PreRender(void)
{
	if (!bShopActive)
		return;

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CShop::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CCameraEffects::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	if (!bShopActive)
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

void CShop::PostRender(void)
{
	if (!bShopActive)
		return;

	// Disable blending
	glDisable(GL_BLEND);
}
