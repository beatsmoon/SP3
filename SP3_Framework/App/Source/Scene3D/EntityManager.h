#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include list
#include <list>

// Include Player
#include "Player3D.h"

// Inlude cameraEffect
#include "../CameraEffects/CameraEffects.h"

// Include Sound Controls
#include "../App/Source/SoundController/SoundController.h"

//Incldue Score
#include "Score.h"




class CEntityManager : public CSingletonTemplate<CEntityManager>
{
	friend class CSingletonTemplate<CEntityManager>;

	// Constants for checking despawn time for entites
	const double EXPLOSIVE_BARREL_DESPAWN_TIME = 10.0;
public:
	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Add a CEntity3D* to this class instance
	virtual void Add(CEntity3D* cEntity3D);
	// Remove a CEntity3D* from this class instance
	virtual bool Erase(CEntity3D* cEntity3D);

	// Collision Check for a CEntity3D*
	virtual bool CollisionCheck(CEntity3D* cEntity3D);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// CleanUp all CEntity3Ds which are marked for deletion
	virtual void CleanUp(void);

	// Render this class instance
	virtual void Render(void);

	void FindAndDeletePoison(CEntity3D* cEntity3D);


	//Wave related functions

	void SetWaveStarted(bool bIsWaveStarted);

	bool GetWaveStarted(void);

	void SetBossStatus(bool bIsBossAlive);

	bool GetBossStatus(void);

	bool CheckWave(void);

	bool CheckBoss(void);

	void DeleteBoss(void);

	void DeleteEnemies(void);
	//Score related function
	virtual void UpdateScore(void);

	void SetToMenu();
	void SetToGame();

protected:
	// Render Settings
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// List of CEntity3D
	std::list<CEntity3D*> lEntity3D;

	CPlayer3D* cPlayer3D;

	CCameraEffects* cCameraEffects;

	CSoundController* cSoundController;

	CScore* cScore;

	// Default Constructor
	CEntityManager(void);
	// Destructor
	virtual ~CEntityManager(void);

	//Wave related members

	bool bIsWaveStarted, bIsBossAlive;


	// This boolean will disable all entites except menu button and projectile
	bool bMenuOnly;



};
