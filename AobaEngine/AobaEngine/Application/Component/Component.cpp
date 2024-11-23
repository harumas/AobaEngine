#include "Component.h"
#include <memory>

Component::Component(const std::shared_ptr<GameObject>& gameObjectPtr) :
	gameObject(gameObjectPtr)
{
}
