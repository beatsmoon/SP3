/**
 CPlayer3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Player3D.h"

// Include Meshbuilder and Mtxx44 for rendering
#include "../Meshes/MeshBuilder.h"
#include "../Meshes/Mtx44.h"

#include <iostream>
#include <vector>

using namespace std;

void CPlayer3D::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP)
	{
		// update elapse time to physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);

		// call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update();

		// get the displacement from cphysics engine and update the player position
		vec3Position += cPhysics3D.GetDisplacement();

		// if the player is still jumping and the initial velocity reached zero or below zero
		// then it has reached the peak of its jump
		if ((cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP) && (cPhysics3D.GetInitialVelocity().y <= 0.0f))
		{
			cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
			cPhysics3D.SetElapsedTime(0.0f);
		}

	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		// update elapse time to physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);

		// call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update();

		// get the displacement from cphysics engine and update the player position
		vec3Position += cPhysics3D.GetDisplacement();

		// constrain the player's position to the groundmap
		glm::vec3 vec3CheckPosition = cGroundMap->GetExactPosition(vec3Position);

		if (vec3CheckPosition.y > vec3Position.y)
		{
			vec3Position = vec3CheckPosition;
			cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
		}

	}
}

/**
 @brief Default Constructor
 */
CPlayer3D::CPlayer3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fMouseSensitivity(0.1f)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, fCameraSwayAngle(0.0f)
	, fCameraSwayDeltaAngle(0.25f)
	, bCameraSwayDirection(false)
	, bCameraSwayActive(false)
	, cCamera(NULL)
	, cGroundMap(NULL)
	, scopeMode(false)
	, shootingMode(true)
	, fCameraRecoilAngle(0.0f)
	, fCameraRecoilDeltaAngle(2.0f)
	, bCameraRecoilDirection(true) //false = down , true = up
	, bCameraRecoilActive(true)
	, bUpdateCameraRecoil(false)
{
	// Set the default position so it is above the ground
	vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Constructor with vectors
 @param position A const glm::vec3 variable which contains the position of the camera
 @param up A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CPlayer3D::CPlayer3D(const glm::vec3 vec3Position,
	const glm::vec3 vec3Front,
	const float fYaw,
	const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, bCameraSwayDirection(false)
	, bCameraSwayActive(false)
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cGroundMap(NULL)
	, cSoundController(NULL)
	, scopeMode(false)
	, shootingMode(true)
	, fCameraRecoilAngle(0.0f)
	, fCameraRecoilDeltaAngle(2.0f)
	, bCameraRecoilDirection(true) //false = down , true = up
	, bCameraRecoilActive(true)
	, bUpdateCameraRecoil(false)
{
	// Set the default position so it is above the ground
	this->vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Destructor
 */
CPlayer3D::~CPlayer3D(void)
{
	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}

	if (cSoundController)
	{
		cSoundController = NULL;
	}

	if (cGroundMap)
	{
		cGroundMap = NULL;
	}
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CPlayer3D::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CPlayer3D::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::PLAYER);

	// init playerMesh
	vec3Scale = glm::vec3(1, 1, 1); // OBJ scale
	vec3ColliderScale = glm::vec3(1.0, 1.0, 1.0); // collider scale
	// load playerMesh OBJ
	playerMesh = MeshBuilder::GenerateOBJ("enemy", "OBJ/chicken.obj");
	// load and create a texture 
	iTextureID = LoadTexture("Images/chicken.tga");
	if (iTextureID == 0)
	{
		cout << "Unable to load Images/chicken.tga" << endl;
		return false;
	}

	// Initialise groundMap
	cGroundMap = CGroundMap::GetInstance();

	// initialise CPhysics3D
	cPhysics3D.Init();
	// set the status to fall by default
	cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);

	cSoundController = CSoundController::GetInstance();

	maxHealth = 100;
	currHealth = 100;
	speed = 1.0f;
	realoadSpeedMultiplier = 1.0f;
	damageMultiplier = 1.0f;
	numOfVirus = 0;

	return true;
}

/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
@return A glm::mat4 variable which contains the view matrix
*/
glm::mat4 CPlayer3D::GetViewMatrix(void) const
{
	return glm::lookAt(vec3Position, vec3Position + vec3Front, vec3Up);
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CPlayer3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CPlayer3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::ProcessMovement(const Player_Movement movement, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime * speed;
	if (movement == FORWARD)
	{
		vec3Position += vec3Front * velocity;
	}
	if (movement == BACKWARD)
	{
		vec3Position -= vec3Front * velocity;
	}
	if (movement == LEFT)
	{
		vec3Position -= vec3Right * velocity;
	}
	if (movement == RIGHT)
	{
		vec3Position += vec3Right * velocity;
	}

	if (movement == FORWARD_RUNNING)
	{
		vec3Position += vec3Front * velocity;
		bCameraSwayActive = true;
	}
	else
	{
		bCameraSwayActive = false;
	}

	/*if (direction != 0 && direction != 1 && direction != 2 && direction != 3)
	{
		fCameraSwayAngle = 0.f;
	}*/
	

	// Indicate that the camera sway is to be updated
	if (bCameraSwayActive)
	{
		bUpdateCameraSway = true;
	}
	
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CPlayer3D::ProcessRotate(float fXOffset, float fYOffset, const bool constrainPitch)
{
	fXOffset *= fMouseSensitivity;
	fYOffset *= fMouseSensitivity;

	//if (scopeMode == true)
	//{
	//	if (GetWeapon()->GetScopeZoom() == 2.0f) // pistol
	//	{

	//		fXOffset *= 0.9;
	//		fYOffset *= 0.9;
	//	}
	//	else // rifle
	//	{

	//		fXOffset *= 0.7;
	//		fYOffset *= 0.7;
	//	}
	//}
	if (scopeMode == true)
	{
		if (GetWeapon()->GetScope()->GetTierLevel() == CGunScope::TIERLEVEL::TIER1)
		{
			fXOffset *= 0.4;
			fYOffset *= 0.4;
		}
		else if (GetWeapon()->GetScope()->GetTierLevel() == CGunScope::TIERLEVEL::TIER2)
		{
			fXOffset *= 0.2;
			fYOffset *= 0.2;
		}
		else if (GetWeapon()->GetScope()->GetTierLevel() == CGunScope::TIERLEVEL::TIER3)
		{
			fXOffset *= 0.08;
			fYOffset *= 0.08;
		}
	}


	fYaw += fXOffset;
	fPitch += fYOffset;

	// make sure that when the pitch is out of bounds, screen dosent get flipped
	if (constrainPitch)
	{ 
		if (fPitch > 89.0f)
		{
			fPitch = 89.0f;
		}
		if (fPitch < -89.0f)
		{
			fPitch = -89.0f;
		}

	}

	UpdatePlayerVectors();

}

void CPlayer3D::SetPosition(glm::vec3 pos)
{
	vec3Position = pos;
}

void CPlayer3D::SetWeapon(const int iSlot, CWeapon* cWeaponInfo)
{
	if (iSlot == 0)
	{
		cPrimaryWeapon = cWeaponInfo;
	}
	else if (iSlot == 1)
	{
		cSecondaryWeapon = cWeaponInfo;
	}

}

CWeapon* CPlayer3D::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
	{
		return cPrimaryWeapon;
	}
	else if (iCurrentWeapon == 1)
	{
		return cSecondaryWeapon;
	}
	return NULL;
}

CWeapon* CPlayer3D::GetInventoryWeapon(const int iSlot) const
{
	switch (iSlot)
	{
	case 0:
		return cPrimaryWeapon;
	case 1:
		return cSecondaryWeapon;
	default:
		return NULL;
	}
}

glm::vec3 CPlayer3D::CalculateBulletDir()
{
	//glm::vec3 front;
	//front.x = cos(glm::radians(fYaw + Math::RandFloatMinMax(-10, 10))) * cos(glm::radians(fPitch));
	//front.y = sin(glm::radians(fPitch));
	//front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	//vec3Front = glm::normalize(front);

	glm::vec3 front;
	front.x = cos(glm::radians(fYaw + Math::RandFloatMinMax(-2, 2))) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch + Math::RandFloatMinMax(-2, 2)));
	front.z = sin(glm::radians(fYaw + Math::RandFloatMinMax(-2, 2))) * cos(glm::radians(fPitch));
	vec3Front = glm::normalize(front);


	return vec3Front;
}

void CPlayer3D::TriggerRecoil()
{
	//Indicate that camera recoil is to be updated
	if (bCameraRecoilActive)
		bUpdateCameraRecoil = true;
}



void CPlayer3D::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

CProjectile* CPlayer3D::DischargeWeapon(void)
{
	if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
	{
		switch (cPrimaryWeapon->GetWeaponName())
		{
		case Weapon_Type::W_SHOTGUN:
			//cSoundController->PlaySoundByID(11);
			return cPrimaryWeapon->Discharge(vec3Position, CalculateBulletDir(), (CEntity3D*)this);
		case Weapon_Type::W_PISTOL:
			//cSoundController->PlaySoundByID(10);
			return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);
		case Weapon_Type::W_AK47:
			//cSoundController->PlaySoundByID(9);
			return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);
		case Weapon_Type::W_SNIPER:
			//cSoundController->PlaySoundByID(12);
			return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);
		}

	/*	if (cPrimaryWeapon->GetWeaponName() == Weapon_Type::W_SHOTGUN)
			return cPrimaryWeapon->Discharge(vec3Position, CalculateBulletDir(), (CEntity3D*)this);
		else
			return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);*/

	}
	else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
	{
		switch (cSecondaryWeapon->GetWeaponName())
		{
		case Weapon_Type::W_SHOTGUN:
			//cSoundController->PlaySoundByID(11);
			return cSecondaryWeapon->Discharge(vec3Position, CalculateBulletDir(), (CEntity3D*)this);
		case Weapon_Type::W_PISTOL:
			//cSoundController->PlaySoundByID(10);
			return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);
		case Weapon_Type::W_AK47:
			//cSoundController->PlaySoundByID(9);
			return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);
		case Weapon_Type::W_SNIPER:
			//cSoundController->PlaySoundByID(12);
			return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);
		}

		/*if (cSecondaryWeapon->GetWeaponName() == Weapon_Type::W_SHOTGUN)
			return cSecondaryWeapon->Discharge(vec3Position, CalculateBulletDir(), (CEntity3D*)this);
		else
			return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CEntity3D*)this);*/
	}
	return NULL;

	return NULL;
}

void CPlayer3D::DropWeapon(void)
{
	if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
	{
		cPrimaryWeapon = NULL;
	}
	else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
	{
		cSecondaryWeapon = NULL;
	}
}

void CPlayer3D::SetScopeMode(bool isScopeMode)
{
	this->scopeMode = isScopeMode;
}

void CPlayer3D::SetShootingMode(bool isAuto)
{
	this->shootingMode = isAuto;
}

void CPlayer3D::SetCurrHealth(int currhealth)
{
	this->currHealth = currhealth;
}

void CPlayer3D::SetMaxHealth(int maxhealth)
{
	this->maxHealth = maxhealth;
}

void CPlayer3D::SetSpeed(float _speed)
{
	this->speed = _speed;
}

void CPlayer3D::SetDmageMultiplier(float multiplier)
{
	damageMultiplier = multiplier;
}

void CPlayer3D::SetReloadSpeedMultiplier(float multiplier)
{
	realoadSpeedMultiplier = multiplier;
}

void CPlayer3D::SetNumOfVirus(int num)
{
	numOfVirus = num;
}

void CPlayer3D::AddVirus(CVirus* _virus)
{
	virus.push_back(_virus);
}

bool CPlayer3D::GetScopeMode()
{
	return scopeMode;
}

bool CPlayer3D::GetShootingMode()
{
	return shootingMode;
}

int CPlayer3D::GetMaxHealth()
{
	return maxHealth;
}

int CPlayer3D::GetCurrHealth()
{
	return currHealth;
}

float CPlayer3D::GetSpeed()
{
	return speed;
}

float CPlayer3D::GetDmageMultiplier()
{
	return damageMultiplier;
}

float CPlayer3D::GetReloadSpeedMultiplier()
{
	return realoadSpeedMultiplier;
}

int CPlayer3D::GetNumOfVirus()
{
	return numOfVirus;
}

void CPlayer3D::SetToJump(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::JUMP);
		cPhysics3D.SetInitialVelocity(glm::vec3(0.f, 10.0f, 0.f));

		cSoundController->PlaySoundByID(3);
	}
}


/**
 @brief Update this class instance
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::Update(const double dElapsedTime)
{
	cPrimaryWeapon->Update(dElapsedTime);
	cSecondaryWeapon->Update(dElapsedTime);

	UpdateJumpFall(dElapsedTime);

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}

	// Constraint the player's position
	Constraint();

	// implement the player's camera sway
	if ((bUpdateCameraSway) && (bCameraSwayActive))
	{
		glm::mat4 rotationMat(1); // creats a identity matrix
		rotationMat = glm::rotate(rotationMat, glm::radians(fCameraSwayAngle), vec3Front);
		vec3Up = glm::vec3(rotationMat * glm::vec4(vec3WorldUp, 1.0f));

		if (bCameraSwayDirection == false)
		{
			fCameraSwayAngle -= fCameraSwayDeltaAngle;
			if (fCameraSwayAngle < fCameraSwayAngle_LeftLimit)
			{
				fCameraSwayAngle = fCameraSwayAngle_LeftLimit;
				bCameraSwayDirection = !bCameraSwayDirection;
			}
		}
		else if (bCameraSwayDirection == true)
		{
			fCameraSwayAngle += fCameraSwayDeltaAngle;
			if (fCameraSwayAngle > fCameraSwayAngle_RightLimit)
			{
				fCameraSwayAngle = fCameraSwayAngle_RightLimit;
				bCameraSwayDirection = !bCameraSwayDirection;
			}
		}
		bUpdateCameraSway = false;
	}

	//update
//Implement player/camera recoil
	if ((bUpdateCameraRecoil) && (bCameraRecoilActive))
	{
		glm::mat4 rotationMat(1);// Creates a identity matrix
		rotationMat = glm::rotate(rotationMat, glm::radians(fCameraRecoilAngle), vec3Right);
		vec3Up = glm::vec3(rotationMat * glm::vec4(vec3WorldUp, 1.0f));

		if (bCameraRecoilDirection == true)
		{
			//track how much angle it move due to recoil
			//fCameraRecoilAngle += fCameraRecoilDeltaAngle;
			fCameraRecoilAngle += GetWeapon()->GetRecoilAngle();
			if (GetWeapon()->GetBarrel() != NULL)
				fCameraRecoilAngle -= GetWeapon()->GetBarrel()->GetRecoilReduction();
			//add to pitch to tilt up
			fPitch += fCameraRecoilAngle;
			//fPitch += GetWeapon()->GetRecoilAngle();
			if (fPitch > fCameraSwayAngle_UpLimit)
			{
				fPitch = fCameraSwayAngle_UpLimit;
				//bCameraSwayDirection = !bCameraSwayDirection;
			}
		}
		UpdatePlayerVectors();
		bUpdateCameraRecoil = false;
	}
	if (fCameraRecoilAngle > 0)
	{
		fCameraRecoilAngle -= 0.1f;
		fPitch -= 0.1f;
	}
	UpdatePlayerVectors();
	if (fCameraRecoilAngle < 0)
		fCameraRecoilAngle = 0;

	static int prevNumOfVirus = numOfVirus;

	if (prevNumOfVirus != numOfVirus)
	{
		for (std::vector<CVirus*>::iterator it = virus.begin(); it != virus.end(); ++it)
		{
			CVirus* go = (CVirus*)*it;

			if (it == virus.end() - 1)
			{
				go->Update(dElapsedTime);
			}
		}
		prevNumOfVirus = numOfVirus;
	}
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CPlayer3D::PreRender(void)
{
	// Draw ground tile as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

void CPlayer3D::RenderMesh(Mesh* mesh)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	// create transformations
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	model = glm::scale(model, vec3Scale);

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
 */
void CPlayer3D::Render(void)
{
	// Don't render the player if the camera is attached to it
	if (cCamera)
		return;

	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CPlayer3D::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	modelStack.PushMatrix();
	RenderMesh(playerMesh);
	modelStack.PopMatrix();

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

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

void CPlayer3D::RenderVirusHUD()
{
	for (std::vector<CVirus*>::iterator it = virus.begin(); it != virus.end(); ++it)
	{
		CVirus* go = (CVirus*)*it;

		go->PreRender();
		go->Render();
		go->PostRender();
	}
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CPlayer3D::UpdatePlayerVectors(void)
{
	

	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	vec3Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp)); 
	if (!bCameraSwayActive)
	{
		vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));
	}

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Constraint the player's position
 */
void CPlayer3D::Constraint(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		vec3Position = cGroundMap->GetExactPosition(vec3Position);
	}
	glm::vec3 tempVec3Position = cGroundMap->GetExactPosition(vec3Position);
	vec3Position.x = tempVec3Position.x;
	vec3Position.z = tempVec3Position.z;
	if (vec3Position.y < tempVec3Position.y)
	{
		vec3Position.y = tempVec3Position.y;
	}


}
