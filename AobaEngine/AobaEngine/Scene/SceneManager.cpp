#include "SceneManager.h"
#include "../Application/Helper.h"

void SceneManager::Switch(int index)
{
	if (currentScene)
	{
		currentScene.reset();
	}

	if (index < factory.size())
	{
		currentScene = factory[index]();
		currentScene->Initialize();
		return;
	}

	ThrowMessage("Scene " + std::to_string(index) + " does not exist!");
}

const std::unique_ptr<BaseScene>& SceneManager::GetCurrentScene()
{
	return currentScene;
}


