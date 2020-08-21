#include "Score.h"

CScore::CScore()
	:iScore(0)
	,fMultiplier(1.f)
	,iScoreToAdd(100)
{
}


CScore::~CScore()
{
	
}


bool CScore::Init(void)
{
	


	return true;
}


void CScore::Update(void)
{

}

void CScore::SetScore(int iScore)
{
	this->iScore = iScore;
}

void CScore::AddScore(int iScoreToAdd)
{
	iScore += iScoreToAdd * GetMultiplier();
}

int CScore::GetScore(void)
{
	return iScore;
}

void CScore::SetMultiplier(float fMultiplier)
{
	this->fMultiplier = fMultiplier;
}

void CScore::AddMultiplier(float fMultiplierToAdd)
{
	fMultiplier += fMultiplierToAdd;
}

float CScore::GetMultiplier(void)
{
	return fMultiplier;
}

void CScore::SetScoreToAdd(int iSetScoreToAdd)
{
	iScoreToAdd = iSetScoreToAdd;
}

int CScore::GetScoreToAdd(void)
{
	return iScoreToAdd;
}

void CScore::AddScoreFromWave(int iWaveNumber, double dWaveTimer)
{
	int iWaveScore = ((1000 * iWaveNumber) / dWaveTimer) * fMultiplier;
	iScore += iWaveScore;

}








