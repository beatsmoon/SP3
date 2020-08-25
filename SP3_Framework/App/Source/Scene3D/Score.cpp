#include "Score.h"

#include <string>

#include <iostream>

using namespace std;

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

void CScore::BubbleSort(int* list, int length)
{
	for (int iter = 1; iter < length; iter++)
	{
		for (int index = 0; index < length - iter; index++)
		{
			if (list[index] < list[index + 1])
			{
				Swap(list, index, index + 1);
			}
		}
	}
}

void CScore::Swap(int* list, int location1, int location2)
{
	int temp = list[location1];

	list[location1] = list[location2];
	list[location2] = temp;
}

void CScore::UpdateHighScores(void)
{
	//Open high score text file in write mode
	ofstream fHighScoreOld;
	fHighScoreOld.open("highscore.txt", ios::app);

	//Add in current score to high score list
	string sInString = to_string(iScore);
	fHighScoreOld << sInString << endl;
	
	fHighScoreOld.close();

	
}

void CScore::PrintHighScores(void)
{
	//Initialise array to sort into
	const int SCORES_SIZE = 10;
	int scores[SCORES_SIZE], count = 0;

	//Open high score text file in read mode
	ifstream fHighScoreRead;
	fHighScoreRead.open("highscore.txt");

	//Insert high scores into the array
	while (count < SCORES_SIZE && fHighScoreRead >> scores[count])
	{
		++count;
	}
	fHighScoreRead.close();

	//Sort high score list into the array
	BubbleSort(scores, SCORES_SIZE);

	for (int i = 0; i < SCORES_SIZE; ++i)
	{
		if (scores[i] > 0)
		{
			cout << scores[i] << endl;
		}
		
	}
	

	////Reset array index counter
	//count = 0;

	////Open high score text file in write mode and delete current contents in high score list
	//ofstream fHighScoreNew;
	//fHighScoreNew.open("highscore.txt", ios::trunc);
	//while (count < SCORES_SIZE && fHighScoreNew << to_string(scores[count]) << endl && scores[count] > 0)
	//{
	//	++count;
	//}

	//fHighScoreNew.close();
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








