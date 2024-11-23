#pragma once
#include "ObjectService.h"
#include <memory>

class GameObject;

class Component : public Object
{
public:
	std::weak_ptr<GameObject> gameObject;

	explicit Component(const std::shared_ptr<GameObject>& gameObjectPtr);
	virtual ~Component() {}
};
