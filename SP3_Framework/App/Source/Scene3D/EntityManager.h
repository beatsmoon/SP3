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

// Include 
#include "../App/Source/SoundController/SoundController.h"

#include "Wave.h"

class CEntityManager : public CSingletonTemplate<CEntityManager>
{
	friend class CSingletonTemplate<CEntityManager>;

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


	//Wave Control Functions

	//Set the number and stats of enemies in each wave
	virtual void SetWave(int waveNumber, CEnemy3D* cEnemy3D, int numberOfZombies);

	//Start a new wave
	virtual void StartWave(void);

	//Get the current wave number
	virtual int GetWaveNumber(void);

	//Set the current wave number
	virtual void SetWaveNumber(int waveNumber);

	//Set the status for wave
	virtual void SetWaveStatus(bool waveStatus);

	//Get the status of wave
	virtual bool GetWaveStatus(void);
	


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

	vector<CWave*> vecCWave;

	// Default Constructor
	CEntityManager(void);
	// Destructor
	virtual ~CEntityManager(void);

	//Wave Control members
	int iWaveCounter;
	bool bWaveActive;


};
