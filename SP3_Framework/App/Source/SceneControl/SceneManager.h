#include <vector>

#include "../Scene2D/Scene2D.h"
#include "../Scene3D/Scene3D.h"
#include "../Scene3D/SceneMenu3D.h"
#include "../Scene3D/SceneGame3D.h"
#include "../Scene3D/SceneShop3D.h"

#include "../Library/Source/DesignPatterns/SingletonTemplate.h"

namespace Scenes
{
	const unsigned int MENU = 0;
	const unsigned int GAME = 1;
	const unsigned int SHOP = 2;
}

class CSceneManager : public CSingletonTemplate<CSceneManager>
{
	friend class CSingletonTemplate<CSceneManager>;

	std::vector<CScene3D*> vScenes;
	int iSceneIndex;

	// Used to handle quitting of application from anywhere in the code
	bool bApplicationShouldEnd;

protected:
	CSceneManager();
	~CSceneManager();

public:
	bool Init();
	void Exit();

	void AddScene(CScene3D* scene);
	void ChangeScene(int sceneID);

	void EnableScene(const int iSceneID);
	void DisableScene(const int iSceneID);

	bool CheckForApplicationEnd();
	void SetApplicationToEnd();

	int GetCurrentScene() const;

	CScene3D* GetScene();

	std::vector<CScene3D*> GetSceneList();
};
