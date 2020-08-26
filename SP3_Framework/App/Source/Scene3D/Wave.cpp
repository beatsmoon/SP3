#include "Wave.h"

CWave::CWave()
	: cShader(NULL)
	, cSimpleShader(NULL)
	, iWaveCounter(1)
	, bWriteScore(false)

{
}

CWave::~CWave()
{

	

}

void CWave::Init(void)
{
	// Setup the shaders
	cShader = new Shader("Shader//Scene3D.vs", "Shader//Scene3D.fs");
	// Setup the shaders
	cSimpleShader = new Shader("Shader//SimpleShader.vs", "Shader//SimpleShader.fs");
	
	cEntityManager = CEntityManager::GetInstance();

	cScore = CScore::GetInstance();

}




//Set the number and stats of enemies in each wave
void CWave::StartWave(int waveNumber)
{
	//Zombie Zombie Type 1
	CEnemy3D* cEnemy3DType1 = NULL;
	CEnemy3D* cEnemy3DType2 = NULL;
	CEnemy3D* cEnemy3DType3 = NULL;

	//Spawn wave 1 enemies
	//Immediately spawn enemies 
	for (int i = 0; i < waveNumber + 3; ++i)
	{
		int k;
		if (waveNumber <= 3)
		{
			k = Math::RandIntMinMax(1, 2);
		}
		else if (waveNumber > 3)
		{
			k = Math::RandIntMinMax(0, 2);
		}

		//Initialise type of enemy
		cEnemy3DType1 = new CEnemy3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f)), k);
		cEnemy3DType1->SetShader(cShader);
		cEnemy3DType1->Init();
		cEnemy3DType1->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cEnemy3DType1);

		CPoison3D* cPoison3D = new CPoison3D(cEnemy3DType1->GetPosition(),glm::uvec2(0, k), cEnemy3DType1);
		cPoison3D->SetShader(cShader);
		cPoison3D->Init();
		cPoison3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cPoison3D);

		cEntityManager->SetWaveStarted(true);
		
	}

}

void CWave::SpawnBoss(void)
{
	static int wave = 1;

	switch (wave % 3)
	{
	case 1: // boss 1
	{
		CBoss3D* cBoss3D = new CBoss3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f)), 0);
		cBoss3D->SetShader(cShader);
		cBoss3D->Init();
		cBoss3D->SetHealth(cBoss3D->GetHealth() + ((wave - 1) * 200));
		cBoss3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cBoss3D);

		/*CPoison3D* cPoison3D = new CPoison3D(cBoss3D->GetPosition(), glm::uvec2(1, 0), cBoss3D);
		cPoison3D->SetShader(cShader);
		cPoison3D->Init();
		cPoison3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cPoison3D);*/

		cEntityManager->SetBossStatus(true);

		wave += 1;

		break;
	}
	case 2:  // boss 2
	{
		CBoss3D* cBoss3D = new CBoss3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f)), 1);
		cBoss3D->SetShader(cShader);
		cBoss3D->Init();
		cBoss3D->SetHealth(cBoss3D->GetHealth() + ((wave - 1) * 200));
		cBoss3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cBoss3D);

		cEntityManager->SetBossStatus(true);

		wave += 1;

		break;
	}
	case 0:  // boss 3
	{
		CBoss3D* cBoss3D = new CBoss3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f)), 2);
		cBoss3D->SetShader(cShader);
		cBoss3D->Init();
		cBoss3D->SetHealth(cBoss3D->GetHealth() + ((wave - 1) * 200));
		cBoss3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cBoss3D);

		cEntityManager->SetBossStatus(true);

		wave += 1;

		break;
	}
	default: // last boss 
	{
		int rand = Math::RandIntMinMax(0, 2);

		CBoss3D* cBoss3D = new CBoss3D(glm::vec3(Math::RandFloatMinMax(-10.0f, 10.0f), 0.5f, Math::RandFloatMinMax(-10.0f, 10.0f)), rand);
		cBoss3D->SetShader(cShader);
		cBoss3D->Init();
		cBoss3D->SetHealth(cBoss3D->GetHealth() + ((wave - 1) * 300));
		cBoss3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cBoss3D);

		cEntityManager->SetBossStatus(true);

		wave += 1;

		break;
	}
	}
	
}


//Get the current wave number
int  CWave::GetWaveNumber()
{
	return iWaveCounter;
}

//Set the current wave number
void  CWave::SetWaveNumber(int waveNumber)
{
	iWaveCounter = waveNumber;
}

void CWave::SetWriteStatus(bool bWriteScore)
{
	this->bWriteScore = bWriteScore;
}
