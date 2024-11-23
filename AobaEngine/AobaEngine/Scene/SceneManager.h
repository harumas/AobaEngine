#pragma once
#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>
#include "BaseScene.h"

class SceneManager
{
public:
	template <typename T>
	void Register()
	{
		factory.emplace_back([]() { return std::make_unique<T>(); });
		sceneIndex++;
	}

	void Switch(int index);

	const std::unique_ptr<BaseScene>& GetCurrentScene();

private:
	int sceneIndex = 0;
	using FactoryFunction = std::function<std::unique_ptr<BaseScene>()>;

	std::unique_ptr<BaseScene> currentScene;
	std::vector<FactoryFunction> factory;
};
