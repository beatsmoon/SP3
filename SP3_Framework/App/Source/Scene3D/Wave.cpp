#include "Wave.h"

CWave::CWave()
	:vecEnemyType1(NULL)
	,vecEnemyType2(NULL)
	,vecEnemyType3(NULL)
{
}

CWave::~CWave()
{
	vector<CEnemy3D*>::iterator it = vecEnemyType1.begin();
	vector<CEnemy3D*>::iterator it2 = vecEnemyType2.begin();
	vector<CEnemy3D*>::iterator it3 = vecEnemyType3.begin();

	while (it != vecEnemyType1.end())
	{
		delete* it;
		it = vecEnemyType1.erase(it);
	}

	while (it2 != vecEnemyType2.end())
	{
		delete* it2;
		it2 = vecEnemyType1.erase(it2);
	}

	while (it3 != vecEnemyType3.end())
	{
		delete* it3;
		it3 = vecEnemyType1.erase(it3);
	}

}

void CWave::AddToEnemyVec1(CEnemy3D* cEnemy)
{
	vecEnemyType1.push_back(cEnemy);
}
void CWave::AddToEnemyVec2(CEnemy3D* cEnemy)
{
	vecEnemyType2.push_back(cEnemy);
}
void CWave::AddToEnemyVec3(CEnemy3D* cEnemy)
{
	vecEnemyType3.push_back(cEnemy);
}

void CWave::Init(void)
{

}
