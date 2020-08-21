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

	//Set Multiplier to a certain number
	virtual void SetMultiplier(float fMultiplier);

	//Add to the multiplier
	virtual void AddMultiplier(float fMultiplierToAdd);

	//Retrive multiplier value
	virtual float GetMultiplier(void);

	//Set the score to add for each enemy killed
	virtual void SetScoreToAdd(int iSetScoreToAdd);

	//Get the score to add for each enemy killed
	virtual int GetScoreToAdd(void);

	virtual void AddScoreFromWave(int iWaveNumber, double dWaveTimer);

	//Update score
	virtual void Update(void);

	virtual bool Init(void);

protected:
	
	CScore();
	~CScore();

	unsigned int iScore, iScoreToAdd;

	float fMultiplier;




};