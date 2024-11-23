#include "Application.h"

#include "Input/Input.h"
#include <windows.h>
#include <string>

#include "AppContext.h"
#include "RenderPipeline.h"
#include "../Scene/SceneManager.h"

Application::Application(HINSTANCE hInstance, const std::shared_ptr<AppContext>& appContext)
{
	serviceLocator.RegisterInstance<AppContext>(appContext);

	// サービスロケーターにオブジェクトを登録
	dxObject = serviceLocator.Register<DxObject>();
	const auto shaderPassPool = std::make_shared<ShaderPassPool>(dxObject);
	serviceLocator.RegisterInstance<ShaderPassPool>(shaderPassPool);

	//パイプラインの初期化 
	sceneManager = std::make_unique<SceneManager>();
	renderPipeline = std::make_unique<RenderPipeline>(serviceLocator);

	// ウィンドウアプリケーションの初期化
	auto updateFunction = [this]() { Update(); };
	auto disposeFunction = [this]() { Dispose(); };

	winApplication = std::make_unique<Win32Application>(hInstance, appContext, updateFunction, disposeFunction);
}

void Application::Run(int bootSceneIndex)
{
	// ウィンドウの作成
	AppHandle handle = winApplication->InitWindow();

	// DirectXのオブジェクトの初期化 
	dxObject->Create(handle.hwnd);

	// 入力機能の初期化 
	Input::Initialize();

	// レンダーパイプラインの初期化
	renderPipeline->OnInit(handle.hwnd);

	// 初期シーンを読み込み
	sceneManager->Switch(bootSceneIndex);

	renderPipeline->OnPostInit();

	winApplication->Run();
}

void Application::Update()
{
	const std::unique_ptr<BaseScene>& scene = sceneManager->GetCurrentScene();

	Input::Update();

	scene->Update();

	renderPipeline->OnUpdate();

	Input::PostUpdate();
}

void Application::Dispose()
{
	Input::Shutdown();
}
