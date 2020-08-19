#include "SceneManager.h"

CSceneManager::CSceneManager()
{
	iSceneIndex = 0;
	bApplicationShouldEnd = false;
}

CSceneManager::~CSceneManager()
{
}

bool CSceneManager::Init()
{
	for (size_t i = 0; i < vScenes.size(); ++i)
	{
		if (vScenes.at(i)->Init() == false)
		{
			std::cout << "Failed to load scene at " << i << " index of vScenes." << std::endl;
			return false;
		}
	}

	return true;
}

void CSceneManager::Exit()
{
	bApplicationShouldEnd = true;
	for (unsigned int i = 0; i < vScenes.size(); ++i)
	{
		if (vScenes.at(i))
			delete vScenes.at(i);
	}
}

void CSceneManager::AddScene(CScene3D* scene)
{
	vScenes.push_back(scene);
}

void CSceneManager::ChangeScene(int sceneID)
{
	//delete scenes.at(iSceneIndex);
	iSceneIndex = sceneID;
	//scenes.at(sceneID)->Init();
}

void CSceneManager::EnableScene(const int iSceneID)
{
	if (vScenes.at(iSceneID)->GetSceneStatus() == false)
		vScenes.at(iSceneID)->EnableScene();
}

void CSceneManager::DisableScene(const int iSceneID)
{
	if (vScenes.at(iSceneID)->GetSceneStatus() == true)
		vScenes.at(iSceneID)->DisableScene();
}

bool CSceneManager::CheckForApplicationEnd()
{
	return bApplicationShouldEnd;
}

void CSceneManager::SetApplicationToEnd()
{
	bApplicationShouldEnd = true;
}

int CSceneManager::GetCurrentScene() const
{
	return iSceneIndex;
}

CScene3D* CSceneManager::GetScene()
{
	return vScenes.at(iSceneIndex);
}

std::vector<CScene3D*> CSceneManager::GetSceneList()
{
	return vScenes;
}
