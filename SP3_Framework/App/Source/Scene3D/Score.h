#pragma once

#include "DesignPatterns/SingletonTemplate.h"



class CScore : public CSingletonTemplate<CScore>
{

	friend class CSingletonTemplate<CScore>;

public:

	//Retrive the player's score
	virtual int GetScore(void);

	//Set Score to a certain amount
	virtual void SetScore(int iScore);

	//Add a certain amount of score
	virtual void AddScore(int iScoreToAdd);

	virtual void SetMultiplier(float fMultiplier);

	virtual void AddMultiplier(float fMultiplierToAdd);

	virtual float GetMultiplier(void);

protected:
	
	CScore();
	~CScore();

	int iScore;
	float fMultiplier;




};