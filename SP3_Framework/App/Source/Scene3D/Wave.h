#pragma once

#include "Entity/Enemy3D.h"

#include "Entity/CBoss3D.h"

#include "EntityManager.h"

#include "RenderControl/shader.h"

#include "DesignPatterns\SingletonTemplate.h"

#include "Entity/Poison3D.h"

#include <fstream>

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
	virtual void StartWave(int waveNumber);

	virtual void SpawnBoss(void);

	//Get the current wave number
	virtual int GetWaveNumber(void);

	//Set the current wave number
	virtual void SetWaveNumber(int waveNumber);

	virtual void SetWriteStatus(bool bWriteScore);

	std::vector<CEntity3D*>& GetEnemies();
	void DeleteEnemy(CEntity3D* enemy);

	static const int MAX_WAVE_COUNT = 10;

protected:
	
	std::vector<CEntity3D*> vEnemies;
	CEntityManager* cEntityManager;

	CScore* cScore;

	//Wave Control members
	int iWaveCounter;

	bool bWriteScore;

	bool bAllWavesCleared;

};