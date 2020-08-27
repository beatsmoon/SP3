#include "Shop.h"

#include <iostream>

using namespace std;

CShop::CShop(void)
	: bActive(true)
	, cPlayer3D(NULL)
	, bShopActive(false)
	, cMouseController(NULL)
	, cScore(NULL)
	, sItemBought("")
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

	textShader = NULL;

	if (cMouseController)
	{
		cMouseController->Destroy();
		cMouseController = NULL;
	}

	if (cScore)
	{
		cScore->Destroy();
		cScore = NULL;
	}

	
	
	
}

bool CShop::Init(void)
{
	if (!cShader)
	{
		cout << "HUD::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Setup the shaders
	cSimpleShader = new Shader("Shader//SimpleShader.vs", "Shader//SimpleShader.fs");

	textShader = new Shader("Shader//text.vs", "Shader//text.fs");

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	cPlayer3D = CPlayer3D::GetInstance();

	cMouseController = CMouseController::GetInstance();

	cSettings = CSettings::GetInstance();

	cScore = CScore::GetInstance();


	

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
	if(bShopActive)
	{
		if (cMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::LMB) && bouncetime < GetTickCount64())
		{
		
			//Buy sniper
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.1 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.2625)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.125 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.375))
			{
				if (cScore->GetScore() < 1500)
				{
					sItemBought = "Not enough money";

					return;
				}

				else if (cScore->GetScore() * 0.15 > 1500)
				{
					cScore->SetScore(cScore->GetScore() * 0.85);
				}

				else if (cScore->GetScore() * 0.15 <= 1500)
				{
					cScore->SetScore(cScore->GetScore() - 1500);
				}

				// If player does not already have sniper
					if (cPlayer3D->GetInventoryWeapon(0)->GetWeaponName() != Weapon_Type::W_SNIPER)
					{
						sItemBought = "Bought Sniper Rifle";
						delete cPlayer3D->GetInventoryWeapon(0);

						//Player buys Sniper
						CGunBarrel* cGunBarrel = new CGunBarrel();
						cGunBarrel->SetShader(cShader);
						cGunBarrel->Init();
						cGunBarrel->SetTierLevel(CGunBarrel::TIERLEVEL::NOTIER);

						CGunExtMag* cGunExtMag = new CGunExtMag();
						cGunExtMag->SetShader(cShader);
						cGunExtMag->Init();
						cGunExtMag->SetTierLevel(CGunExtMag::TIERLEVEL::NOTIER);

						CGunScope* cGunScope = new CGunScope();
						cGunScope->SetShader(cShader);
						cGunScope->Init();
						cGunScope->SetTierLevel(CGunScope::TIERLEVEL::TIER3);

						CWeapon* rifle = new CWeapon(Weapon_Type::W_SNIPER);
						rifle->Init();
						rifle->SetShader(cSimpleShader);
						cPlayer3D->SetWeapon(0, rifle);
						cPlayer3D->SetCurrentWeapon(0);
						cPlayer3D->GetWeapon()->SetBarrel(cGunBarrel);
						cPlayer3D->GetWeapon()->SetExtMag(cGunExtMag);
						cPlayer3D->GetWeapon()->SetScope(cGunScope);
					}

					else
					{
						sItemBought = "You already have a Sniper Rifle";
					}
			}


			//Buy AK47
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.7375 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.9)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.125 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.375))
			{
				if (cPlayer3D->GetInventoryWeapon(0)->GetWeaponName() != Weapon_Type::W_AK47)
				{

					if (cScore->GetScore() < 2500)
					{
						sItemBought = "Not enough money";
						
						return;
					}

					else if (cScore->GetScore() * 0.2 > 2500)
					{
						cScore->SetScore(cScore->GetScore() * 0.8);
					}

					else if (cScore->GetScore() * 0.2 <= 2500)
					{
						cScore->SetScore(cScore->GetScore() - 2500);
					}

					
						sItemBought = "Bought AK47";
						//Player buys AK47
						delete cPlayer3D->GetInventoryWeapon(0);

						//Player buys AK47
						CGunBarrel* cGunBarrel = new CGunBarrel();
						cGunBarrel->SetShader(cShader);
						cGunBarrel->Init();
						cGunBarrel->SetTierLevel(CGunBarrel::TIERLEVEL::NOTIER);

						CGunExtMag* cGunExtMag = new CGunExtMag();
						cGunExtMag->SetShader(cShader);
						cGunExtMag->Init();
						cGunExtMag->SetTierLevel(CGunExtMag::TIERLEVEL::NOTIER);

						CGunScope* cGunScope = new CGunScope();
						cGunScope->SetShader(cShader);
						cGunScope->Init();
						cGunScope->SetTierLevel(CGunScope::TIERLEVEL::NOTIER);

						CWeapon* rifle = new CWeapon(Weapon_Type::W_AK47);
						rifle->Init();
						rifle->SetShader(cSimpleShader);
						cPlayer3D->SetWeapon(0, rifle);
						cPlayer3D->SetCurrentWeapon(0);
						cPlayer3D->GetWeapon()->SetBarrel(cGunBarrel);
						cPlayer3D->GetWeapon()->SetExtMag(cGunExtMag);
						cPlayer3D->GetWeapon()->SetScope(cGunScope);
					
				}

				else
				{
					sItemBought = "You already have an AK47";
				}
			}

			//Buy Shotgun
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.41875 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.58125)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.125 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.375))
			{
				if (cScore->GetScore() < 1000)
				{
					sItemBought = "Not enough money";

					return;
				}

				else if (cScore->GetScore() * 0.1 > 1000)
				{
					cScore->SetScore(cScore->GetScore() * 0.9);
				}

				else if (cScore->GetScore() * 0.1 <= 1500)
				{
					cScore->SetScore(cScore->GetScore() - 1000);
				}

				if (cPlayer3D->GetInventoryWeapon(0)->GetWeaponName() != Weapon_Type::W_SHOTGUN)
				{
					sItemBought = "Bought Shotgun";
					//Player buys shotgun
					delete cPlayer3D->GetInventoryWeapon(0);

					CGunBarrel* cGunBarrel = new CGunBarrel();
					cGunBarrel->SetShader(cShader);
					cGunBarrel->Init();
					cGunBarrel->SetTierLevel(CGunBarrel::TIERLEVEL::NOTIER);

					CGunExtMag* cGunExtMag = new CGunExtMag();
					cGunExtMag->SetShader(cShader);
					cGunExtMag->Init();
					cGunExtMag->SetTierLevel(CGunExtMag::TIERLEVEL::NOTIER);

					CGunScope* cGunScope = new CGunScope();
					cGunScope->SetShader(cShader);
					cGunScope->Init();
					cGunScope->SetTierLevel(CGunScope::TIERLEVEL::NOTIER);

					CWeapon* rifle = new CWeapon(Weapon_Type::W_SHOTGUN);
					rifle->Init();
					rifle->SetShader(cSimpleShader);
					cPlayer3D->SetWeapon(0, rifle);
					cPlayer3D->SetCurrentWeapon(0);
					cPlayer3D->GetWeapon()->SetBarrel(cGunBarrel);
					cPlayer3D->GetWeapon()->SetExtMag(cGunExtMag);
					cPlayer3D->GetWeapon()->SetScope(cGunScope);
				}

				else
				{
					sItemBought = "You already have a Shotgun";
				}
			}


			//Extended Magazine attachment
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.08984 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.22265)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.51315 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.67105))
			{
				if (cScore->GetScore() < 750)
				{
					sItemBought = "Not enough money";

					return;
				}

				else if (cScore->GetScore() >= 750)
				{
					cScore->SetScore(cScore->GetScore() - 750);
				}

				if (cPlayer3D->GetInventoryWeapon(0)->GetExtMag()->GetTierLevel() != cPlayer3D->GetInventoryWeapon(0)->GetExtMag()->TIER3)
				{

					cPlayer3D->GetInventoryWeapon(0)->GetExtMag()->UpgradeExtMag();

					cPlayer3D->GetInventoryWeapon(0)->SetMaxMagRound(cPlayer3D->GetInventoryWeapon(0)->GetMaxMagRound()
						+ cPlayer3D->GetInventoryWeapon(0)->GetExtMag()->GetAdditionMag());
					sItemBought = "Bought a tier " + std::to_string(static_cast<int>(cPlayer3D->GetInventoryWeapon(0)->GetExtMag()->GetTierLevel())) + "Extended Magazine";
				}

				else
				{
					sItemBought = "Extended Magazine is maxed";
				}
			}

			//Scope attachment
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.31641 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.44921)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.51315 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.67105))
			{
				if (cScore->GetScore() < 900)
				{
					sItemBought = "Not enough money";

					return;
				}

				else if (cScore->GetScore() >= 900)
				{
					cScore->SetScore(cScore->GetScore() - 900);
				}

				if (cPlayer3D->GetInventoryWeapon(0)->GetScope()->GetTierLevel() != cPlayer3D->GetInventoryWeapon(0)->GetScope()->TIER3)
				{

					cPlayer3D->GetInventoryWeapon(0)->GetScope()->UpgradeScope();
					sItemBought = "Bought a tier " + std::to_string(static_cast<int>(cPlayer3D->GetInventoryWeapon(0)->GetScope()->GetTierLevel())) + " Scope";
				}

				else
				{
					sItemBought = "Scope is maxed";
				}
			}

			//Barrel attachment
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.54297 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.67578)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.51315 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.67105))
			{
				if (cScore->GetScore() < 1200)
				{
					sItemBought = "Not enough money";

					return;
				}

				else if (cScore->GetScore() >= 1200)
				{
					cScore->SetScore(cScore->GetScore() - 1200);
				}

				//Player buys barrel attachment;
				CGunBarrel* cGunBarrel = new CGunBarrel();
				cGunBarrel->Init();

				if (cPlayer3D->GetInventoryWeapon(0)->GetBarrel()->GetTierLevel() != cPlayer3D->GetInventoryWeapon(0)->GetBarrel()->TIER3)
				{
					cPlayer3D->GetInventoryWeapon(0)->GetBarrel()->UpgradeBarrel();
					//cout << static_cast<int>(cPlayer3D->GetInventoryWeapon(0)->GetBarrel()->GetTierLevel()) << endl;
					sItemBought = "Bought a tier " + std::to_string(static_cast<int>(cPlayer3D->GetInventoryWeapon(0)->GetBarrel()->GetTierLevel())) + " Barrel";
				}

				else
				{
					sItemBought = "Barrel is maxed";
				}

			}

			//Bottom Left Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.076562 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.1625)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.76973 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.92105))
			{
				if (cScore->GetScore() < 500)
				{
					sItemBought = "Not enough money";
					return;
				}

				else if (cScore->GetScore() >= 500)
				{
					cScore->SetScore(cScore->GetScore() - 500);
				}

				if (cPlayer3D->GetWeapon()->GetTotalRound() != cPlayer3D->GetWeapon()->GetMaxTotalRound())
				{
					
					//Player buys Ammo
					cPlayer3D->GetInventoryWeapon(0)->AddRounds(cPlayer3D->GetInventoryWeapon(0)->GetMaxMagRound());
					sItemBought = "Ammo Bought" + std::to_string(cPlayer3D->GetInventoryWeapon(0)->GetMagRound()) + " / " +
						std::to_string(cPlayer3D->GetInventoryWeapon(0)->GetTotalRound());
				}
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

std::string CShop::GetItemBought(void)
{
	return sItemBought;
}

void CShop::SetItemBought(std::string sItemBought)
{
	this->sItemBought = sItemBought;
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
