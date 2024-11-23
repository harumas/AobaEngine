#pragma once
#include <memory>
#include "Component.h"
#include "GameObject.h"
#include "../Component/Transform.h"
#include <DirectXMath.h>

class Camera : public Component
{
public:
	Camera(const std::shared_ptr<GameObject>& gameObject);

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjectionMatrix(float aspectRatio) const;

	void Init(float fov, float nearPlane, float farPlane);
	void SetYaw(float yaw);
	void SetPitch(float pitch);

	std::shared_ptr<Transform> transform;
	static std::shared_ptr<Camera> current;
private:
	float fov;
	float nearPlane;
	float farPlane;
};

