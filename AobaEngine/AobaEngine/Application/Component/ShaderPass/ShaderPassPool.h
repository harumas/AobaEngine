#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "../ShaderPass.h"

class ShaderPassPool
{
public:
	ShaderPassPool(const std::shared_ptr<DxObject>& dxObject) : rootSignature(nullptr), dxObject(dxObject)
	{
	}

	void SetRootSignature(ID3D12RootSignature* rootSignature)
	{
		this->rootSignature = rootSignature;
	}

	template<typename T>
	std::shared_ptr<ShaderPass> GetShaderPass()
	{
		const std::type_index id = typeid(T);

		if (shaderPassPool.find(id) != shaderPassPool.end())
		{
			return shaderPassPool[id];
		}

		// シェーダのコンパイル
		std::shared_ptr<T> shaderPass = std::make_shared<T>();
		shaderPass->Compile(rootSignature);

		shaderPassPool.emplace(id, shaderPass);
		return std::dynamic_pointer_cast<T>(shaderPass);
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<ShaderPass>> shaderPassPool;
	ID3D12RootSignature* rootSignature;
	std::shared_ptr<DxObject> dxObject;
};
