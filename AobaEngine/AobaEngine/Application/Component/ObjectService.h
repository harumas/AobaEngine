#pragma once
#include <memory>
#include <vector>

class GraphicsContext;

class Object
{
protected:
	Object()
	{

	}
	virtual ~Object() = default;

public:
	virtual void OnCreate() {}
	virtual void OnUpdate() {}
	virtual void OnDraw(GraphicsContext& context) {}
	virtual void OnDestroy() {}

private:
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;

	Object(const Object&&) = delete;
	Object& operator=(const Object&&) = delete;
};


class ObjectService
{
public:
	ObjectService() = default;
	~ObjectService()
	{
		for (const auto& object : objects)
		{
			object->OnDestroy();
		}
	}

	template<typename T, typename... Ts>
	std::shared_ptr<T> Create(Ts&&... params)
	{
		std::shared_ptr<T> ptr = std::make_shared<T>(params...);

		objects.push_back(ptr);

		ptr->OnCreate();
		return ptr;
	}

	void Register(const std::shared_ptr<Object>& object)
	{
		objects.push_back(object);
	}

	void ProcessOnUpdate()
	{
		for (const auto& object : objects)
		{
			object->OnUpdate();
		}
	}

	void ProcessOnDraw(GraphicsContext& graphicsContext)
	{
		for (const auto& object : objects)
		{
			object->OnDraw(graphicsContext);
		}
	}
private:
	std::vector<std::shared_ptr<Object>> objects;
};
