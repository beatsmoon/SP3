#pragma once

#include "Entity/Enemy3D.h"

#include "EntityManager.h"

#include "RenderControl/shader.h"

#include "DesignPatterns\SingletonTemplate.h"

#include "Entity/RangeIndicator.h"

#include <vector>



class CWave : public CSingletonTemplate<CWave>
{
	
	friend class CSingletonTemplate<CWave>;

public:
	CWave();
	~CWave();

	// Handler to the Shader Program instance
	Shader* cShader;
	// Handler to the LineShader Program instance
	Shader* cSimpleShader;

	virtual void Init(void);
	
	//Wave Control Functions

	//Set the number and stats of enemies in each wave
	virtual void SetWave(int waveNumber);

	//Start a new wave
	virtual void StartWave(void);

	//Get the current wave number
	virtual int GetWaveNumber(void);

	//Set the current wave number
	virtual void SetWaveNumber(int waveNumber);

protected:
	
	CEntityManager* cEntityManager;

	CScore* cScore;

	//Wave Control members
	int iWaveCounter;
};