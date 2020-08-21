#include "Score.h"




CScore::CScore()
	:iScore(0)
	,fMultiplier(1.f)

{
}


CScore::~CScore()
{

}

void CScore::SetScore(int iScore)
{
	this->iScore = iScore;
}

void CScore::AddScore(int iScoreToAdd)
{
	iScore += iScoreToAdd;
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



