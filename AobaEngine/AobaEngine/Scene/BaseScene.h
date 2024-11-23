#pragma once
#include <string>

#include "../Application/Component/ShaderPass/ShaderPassPool.h"
#include "../Application/Utility/ServiceLocator.h"

class BaseScene
{
public:
	BaseScene(const ServiceLocator& serviceLocator)
		: shaderPassPool(serviceLocator.Resolve<ShaderPassPool>())
	{
	}

	virtual ~BaseScene() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	virtual std::string_view GetSceneName()
	{
		return "Unknown";
	}
protected:
	std::shared_ptr<ShaderPassPool> shaderPassPool;
};
