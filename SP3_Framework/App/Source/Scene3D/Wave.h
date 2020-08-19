#pragma once
#include "Enemy3D.h"
#include <vector>

using namespace std;

class CWave
{
public:


	CWave();
	~CWave();

	virtual void Init(void);

	virtual void AddToEnemyVec1(CEnemy3D* cEnemy);
	virtual void AddToEnemyVec2(CEnemy3D* cEnemy);
	virtual void AddToEnemyVec3(CEnemy3D* cEnemy);
	
protected:
	
	vector<CEnemy3D*> vecEnemyType1;

	vector<CEnemy3D*> vecEnemyType2;

	vector<CEnemy3D*> vecEnemyType3;

	

};