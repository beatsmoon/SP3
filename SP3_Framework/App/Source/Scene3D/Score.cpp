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

void CScore::BubbleSort()
{
	for (int iter = 1; iter < vScoreBoard.size(); iter++)
	{
		for (int index = 0; index < vScoreBoard.size() - iter; index++)
		{
			if (vScoreBoard[index] < vScoreBoard[index + 1])
			{
				Swap(index, index + 1);
			}
		}
	}
}

void CScore::Swap(int location1, int location2)
{
	int temp = vScoreBoard[location1];

	vScoreBoard[location1] = vScoreBoard[location2];
	vScoreBoard[location2] = temp;
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

void CScore::GetHighScores(void)
{
	std::string str;	

	//Open high score text file in read mode
	ifstream fHighScoreRead;
	fHighScoreRead.open("highscore.txt");

	while (std::getline(fHighScoreRead, str))
	{
		int iString = std::stoi(str);
		vScoreBoard.push_back(iString);
	}

	fHighScoreRead.close();

	//Sort high score list into the vector
	BubbleSort();
}

std::vector<int>& CScore::GetScoreboard()
{
	return vScoreBoard;
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








