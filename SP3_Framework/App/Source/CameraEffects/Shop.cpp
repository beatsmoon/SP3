#include "Shop.h"

#include <iostream>

using namespace std;

CShop::CShop(void)
	: cPlayer3D(NULL)
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
	iTextureID = LoadTexture("Images/GUI/ShopUIv2.tga");
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

	//Logic for buying in Shop UI
	if (bShopActive)
	{
		if (cMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::LMB))
		{
			cout << "Clicked in shop" << endl;
			cout << "m_windowwidth: " << m_windowWidth << ", m_windowHeight: " << m_windowHeight << endl;
			//Top Left Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.1 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.2625)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.125 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.375))
			{
				cout << "Top Left Box" << endl;
			}


			//Top Right Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.7375 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.9)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.125 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.375))
			{
				cout << "Top Right Box" << endl;
			}

			//Middle Left Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.16875 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.3)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.51667 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.66667))
			{
				cPlayer3D->GetInventoryWeapon(0)->AddRounds(30);
				cout << "Ammo Bought" << endl;
			}
			
			//Middle Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.41875 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.58125)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.38333 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.63333))
			{
				cout << "Middle Box" << endl;
			}

			//Middle Right Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.7 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.83125)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.51667 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.66667))
			{
				cout << "Middle Right Box" << endl;
			}

			//Bottom Left Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.09375 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.225)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.775 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.91667))
			{
				cout << "Bottom Left Box" << endl;
			}

			//Bottom Middle Left Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.31875 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.45)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.775 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.91667))
			{
				cout << "Bottom Middle Left Box" << endl;
			}

			//Bottom Middle Right Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.54375 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.675)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.775 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.91667))
			{
				cout << "Bottom Middle Right Box" << endl;
			}

			//Bottom Right Box
			if ((cMouseController->GetMousePositionX() >= m_windowWidth * 0.775 && cMouseController->GetMousePositionX() <= m_windowWidth * 0.90625)
				&& (cMouseController->GetMousePositionY() >= m_windowHeight * 0.775 && cMouseController->GetMousePositionY() <= m_windowHeight * 0.91667))
			{
				cout << "Bottom Right Box" << endl;
			}



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
