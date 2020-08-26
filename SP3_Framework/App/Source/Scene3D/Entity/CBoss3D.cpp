/**
 CBoss3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "CBoss3D.h"
#include "../EntityManager.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CBoss3D::CBoss3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPlayer3D(NULL)
	, cGroundMap(NULL)
	, cSoundController(NULL)
	, type(NULL)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CBoss3D::CBoss3D(const glm::vec3 vec3Position,
	int enemy_type,
	const glm::vec3 vec3Front,
	const float fYaw,
	const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fRotationSensitivity(0.1f)
	, cCamera(NULL)
	, cPlayer3D(NULL)
	, cGroundMap(NULL)
	, cSoundController(NULL)
{
	// Set the default position to the origin
	this->vec3Position = vec3Position;

	this->type = enemy_type;

	// Update the vectors for this enemy
	UpdateEnemyVectors();
}

/**
 @brief Destructor
 */
CBoss3D::~CBoss3D(void)
{
	if (cGroundMap)
	{
		// We set it to NULL only since it was declared somewhere else
		cGroundMap = NULL;
	}

	if (cPlayer3D)
	{
		// We set it to NULL only since it was declared somewhere else
		cPlayer3D = NULL;
	}

	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}

	if (cSoundController)
	{
		// We set it to NULL only since it was declared somewhere else
		cSoundController = NULL;
	}

	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CBoss3D::Init(void)
{
	// Check if the shader is ready
	if (!cShader)
	{
		cout << "CBoss3D::Init(): The shader is not available for this class instance." << endl;
		return false;
	}

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::BOSS);

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();

	// Store the handler to the CGroundMap
	cGroundMap = CGroundMap::GetInstance();

	cSoundController = CSoundController::GetInstance();

	// Movement Control
	iCurrentNumMovement = 0;
	iMaxNumMovement = 20;
	status = S_IDLE;


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	switch (type)
	{
	case T_BOSS1:
	{
		health = 400;
		speed = Math::RandFloatMinMax(0.8f, 1.3f);
		rangeOfSight = 6.f;
		splitting = 0;

		vec3Scale = glm::vec3(1.5, 1.5, 1.5);
		vec3ColliderScale = glm::vec3(0.675, 2.6, 0.675);

		std::string file_path = "OBJ/creeper.obj";
		bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
		{
			return NULL;
		}

		break;
	}
	case T_BOSS2:
	{
		health = 400;
		speed = Math::RandFloatMinMax(1.0f, 1.2f);
		rangeOfSight = 6.f;
		splitting = 0;

		vec3Scale = glm::vec3(0.45, 0.45, 0.45);
		vec3ColliderScale = glm::vec3(1, 1, 1);

		std::string file_path = "OBJ/slime.obj";
		bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
		if (!success)
		{
			return NULL;
		}

		break;
	}
	case T_BOSS3:
	{
		health = 400;
		speed = Math::RandFloatMinMax(0.1f, 0.7f);
		rangeOfSight = 6.f;
		splitting = 3;

		vec3Scale = glm::vec3(0.45, 0.45, 0.45);
		vec3ColliderScale = glm::vec3(1, 1, 1);

		std::string file_path = "OBJ/slime.obj";
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

	

	switch (type)
	{
	case T_BOSS1:
	{
		iTextureID = LoadTexture("Images/creeper.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load Images/creeper.tga" << endl;
			return false;
		}
		break;
	}
	case T_BOSS2:
	{
		iTextureID = LoadTexture("Images/magma.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load Images/magma.tga" << endl;
			return false;
		}
		break;
	}
	case T_BOSS3:
	{
		iTextureID = LoadTexture("Images/slime.tga");
		if (iTextureID == 0)
		{
			cout << "Unable to load Images/slime.tga" << endl;
			return false;
		}
		break;
	}
	}

	return true;
}


/**
 @brief Set model
 @param model A glm::mat4 variable containing the model for this class instance
 */
void CBoss3D::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable containing the model for this class instance
 */
void CBoss3D::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable containing the model for this class instance
 */
void CBoss3D::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CBoss3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CBoss3D::IsCameraAttached(void)
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
void CBoss3D::ProcessMovement(const Boss_Movement direction, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime * speed;
	if (direction == M_FORWARD)
		vec3Position += vec3Front * velocity;
	if (direction == M_BACKWARD)
		vec3Position -= vec3Front * velocity;
	if (direction == M_LEFT)
		vec3Position -= vec3Right * velocity;
	if (direction == M_RIGHT)
		vec3Position += vec3Right * velocity;

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
	}

	// Constraint the player's position
	Constraint();
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CBoss3D::ProcessRotate(const float fXOffset)
{
	// Update the yaw
	fYaw += fXOffset;// *fRotationSensitivity;

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdateEnemyVectors();
}

/**
@brief Update this class instance
*/
void CBoss3D::Update(const double dElapsedTime)
{
	// Store the enemy's current position, if rollback is needed.
	StorePositionForRollback();

	// play roaming sound when the boss is alive
	// sound delay to replay when the countdown is ended
	static double soundDelay = 1500.f;
	if (soundDelay < 1500.f)
	{
		soundDelay += 1.f;

	}
	else
	{
		soundDelay = 0.f;
		cSoundController->PlaySoundByID(4);
		cout << "yes" << endl;
	}

	// boss movement

	// update Boss AI status
	float fDistanceToPlayer = glm::length(cPlayer3D->GetPosition() - vec3Position);
	if (fDistanceToPlayer > rangeOfSight) // player is out of range
	{
		status = S_IDLE;
	}
	else // player is inside range
	{
		status = S_ATTACKING;
	}
	
	switch (status)
	{
	case CBoss3D::S_IDLE: // boss idle state
		{
			if (iCurrentNumMovement < iMaxNumMovement)
			{
				// moving forward
				ProcessMovement(M_FORWARD, (float)dElapsedTime);

				// Update the counter
				iCurrentNumMovement++;
			}
			else // rotate randomly
			{
				// Randomly choose a new direction up to +30 or -30 degrees to the current direction 
				ProcessRotate(Math::RandFloatMinMax(-30.0f, 30.0f));

				// Reset the counter to 0
				iCurrentNumMovement = 0;
			}
			break;
		}
	case CBoss3D::S_ATTACKING: // boss attacking state
		{
			// player is inside range (update vector to face and chase the player)
			UpdateEnemyVectors();

			// Reset the counter to 0
			iCurrentNumMovement = 0;

			// actions of the boss
			switch (type)
			{
			case T_BOSS1:
			case T_BOSS3:
			{
				// do nothing just to follow the player

				// moving forward
				ProcessMovement(M_FORWARD, (float)dElapsedTime);

				break;
			}
			case T_BOSS2:
			{
				// shoot projectiles at the player
				static float dCountdown = 0.0f;				
				dCountdown += dElapsedTime;
				if (dCountdown > 1)
				{
					CEntity3D* currEntity = dynamic_cast<CEntity3D*>(this);

					CProjectile* aProjectile = new CProjectile();
					aProjectile->SetShader(cShader);
					aProjectile->Init(vec3Position + vec3Front * 0.75f, vec3Front, 2.0f, 20.0f);
					aProjectile->ActivateCollider(cShader);
					aProjectile->SetStatus(true);
					aProjectile->SetSource(currEntity);
					aProjectile->SetCollisionDamage(5);
					aProjectile->SetGravityMultiplier(1.0);

					CEntityManager::GetInstance()->Add(aProjectile);
					dCountdown = 0.0f;
				}

				
			}
			}
			break;
		}
	}
}

/**
@brief Activate the CCollider for this class instance
@param cLineShader A Shader* variable which stores a shader which renders lines
*/
void CBoss3D::ActivateCollider(Shader* cLineShader)
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

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CBoss3D::PreRender(void)
{
	// Draw this as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
}

/**
@brief Render Render this instance
@param cShader A Shader* variable which contains the Shader to use in this class instance
*/
void CBoss3D::Render(void)
{
	// If the shader is in this class, then do not render
	if (!cShader)
	{
		cout << "CBoss3D::Render(): The shader is not available for this class instance." << endl;
		return;
	}

	// Activate shader
	cShader->use();

	// render enemyMesh
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	switch (type)
	{
	case T_BOSS1:
		{
			// create transformations
			model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y - 0.5f, vec3Position.z));
			model = glm::rotate(model, glm::radians(-fYaw + 90.f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, vec3Scale);
		}
	case T_BOSS2:
		{
			// create transformations
			model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y - 0.5f, vec3Position.z));
			model = glm::rotate(model, glm::radians(-fYaw + 90.f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, vec3Scale);
		}
	case T_BOSS3:
		{
			// create transformations
			model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y - 0.5f, vec3Position.z));
			model = glm::rotate(model, glm::radians(-fYaw + 90.f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, vec3Scale);
		}
	}

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
		model = glm::translate(model, vec3Position + vec3ColliderTranslate);
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
void CBoss3D::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

void CBoss3D::SetHealth(int health)
{
	this->health = health;
}

int CBoss3D::GetHealth()
{
	return health;
}

void CBoss3D::SetSplit(int split)
{
	this->splitting = split;
}

int CBoss3D::GetSplit()
{
	return splitting;
}

void CBoss3D::SplitIntoSmallerBoss()
{
	splitting--;

	CBoss3D* cboss = new CBoss3D(vec3Position, T_BOSS3);
	cboss->SetShader(cShader);
	cboss->Init();
	cboss->ActivateCollider(cShader);
	cboss->SetSplit(splitting);
	cboss->ScaleBossBasedOnSplit(splitting);
	cboss->SetHealth(cboss->GetHealth() * (1.0f / (4 - splitting)));
	CEntityManager::GetInstance()->Add(cboss);
}

void CBoss3D::ScaleBossBasedOnSplit(int splitSize)
{
	float size = 1.0f / (4 - splitSize);

	vec3Scale = size * vec3Scale;
	vec3ColliderScale = size * vec3ColliderScale;

	vec3ColliderTranslate.y = -0.5f * (1 - size);
}

int CBoss3D::GetBossType()
{
	return type;
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CBoss3D::UpdateEnemyVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front = glm::normalize(front);

	// Check if we are too far from the player
	if (cPlayer3D)
	{
		float fDistanceToPlayer = glm::length(cPlayer3D->GetPosition() - vec3Position);
		if (fDistanceToPlayer < rangeOfSight) // player is inside the view range of the boss
		{
			// Update the direction of the enemy (turning to face the player)
			front = glm::normalize(glm::vec3(cPlayer3D->GetPosition() - vec3Position));

			// Update the yaw and pitch
			fYaw = glm::degrees(glm::atan(front.z, front.x));
			fPitch = glm::degrees(glm::asin(front.y));
		}
	}

	vec3Front = front;
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));
	vec3Up = glm::normalize(glm::cross(vec3Right, vec3Front));

	// If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Right = vec3Right;
		cCamera->vec3Up = vec3Up;
	}
}

/**
 @brief Constraint the player's position
 */
void CBoss3D::Constraint(void)
{
	vec3Position = cGroundMap->GetExactPosition(vec3Position);
}