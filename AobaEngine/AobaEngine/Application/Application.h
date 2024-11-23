#pragma once
#include <string>

#include "AppContext.h"
#include "Win32Application.h"
#include "../Scene/SceneManager.h"

class Application
{
public:
	Application(HINSTANCE hInstance, const std::shared_ptr<AppContext>& appContext);
	void Run(int bootSceneIndex);

private:
	ServiceLocator serviceLocator;

	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<Win32Application> winApplication;
	std::unique_ptr<RenderPipeline> renderPipeline;
	std::shared_ptr<AppContext> appContext;
	std::shared_ptr<DxObject> dxObject;

	void Update();
	void Dispose();
};
