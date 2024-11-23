#pragma once

#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

// ServiceLocatorクラス
class ServiceLocator
{
public:
	template<typename T>
	std::shared_ptr<T> Register()
	{
		std::shared_ptr<T> service = std::make_shared<T>();
		services[std::type_index(typeid(T))] = service;
		return service;
	}

	template<typename T>
	void RegisterInstance(std::shared_ptr<T> service)
	{
		services[std::type_index(typeid(T))] = service;
	}

	// サービスを取得するメソッド
	template<typename T>
	std::shared_ptr<T> Resolve() const
	{
		const auto it = services.find(std::type_index(typeid(T)));

		if (it != services.end())
		{
			return std::static_pointer_cast<T>(it->second);
		}
		else
		{
			throw std::runtime_error("Service not found: " + typeid(T).name());
		}
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<void>> services;
};