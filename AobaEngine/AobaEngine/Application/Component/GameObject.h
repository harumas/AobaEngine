#pragma once
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "Transform.h"
#include "ObjectService.h"

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	static std::shared_ptr<GameObject> Create(const std::string& name)
	{
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>(name);
		object->AddComponent<Transform>();
		return object;
	}

	explicit GameObject(const std::string& name) : name(name)
	{
	}

	std::string name;

	template<typename T>
	std::shared_ptr<T> AddComponent()
	{
		std::shared_ptr<T> component = std::make_shared<T>(shared_from_this());
		components.emplace(typeid(T), component);
		ObjectService::Register(std::dynamic_pointer_cast<T>(component));

		component->OnCreate();

		return component;
	}

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		return std::dynamic_pointer_cast<T>(components[typeid(T)]);
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
};
