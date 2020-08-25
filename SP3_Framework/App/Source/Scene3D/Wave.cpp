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

void CWave::UpdateHighScore()
{
	if (iWaveCounter > 10)
	{
		ofstream highscore;
		highscore.open("highscore.txt", ios::app);
		string sInString = to_string(cScore->GetScore());
		highscore << sInString << endl;
		highscore.close();
	}
	
}



//Set the number and stats of enemies in each wave
void CWave::SetWave(int waveNumber)
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
		//Spawn the enemies in world		
		cEntityManager->Add(cEnemy3DType1);

		CPoison3D* cPoison3D = new CPoison3D(cEnemy3DType1->GetPosition(), k, cEnemy3DType1);
		cPoison3D->SetShader(cShader);
		cPoison3D->Init();
		cPoison3D->ActivateCollider(cSimpleShader);
		cEntityManager->Add(cPoison3D);

		cEntityManager->SetWaveStarted(true);
		
	}
}


//Start a new wave
void  CWave::StartWave()
{

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
