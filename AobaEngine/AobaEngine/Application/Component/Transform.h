#pragma once
#include "Component.h" 
#include "../Utility/MathHelper.h"
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <DirectXMathVector.inl>
#include <memory>
#include "GameObject.h"

using namespace DirectX;

class Transform : public Component
{
public:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Transform(const std::shared_ptr<GameObject>& gameObjectPtr) :
		Component(gameObjectPtr),
		position(0, 0, 0),
		rotation(0, 0, 0),
		scale(1, 1, 1)
	{
	}

	Vector3 Forward() const
	{
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation);
		return Vector3(XMVector3TransformNormal(Vector3::FORWARD, rotationMatrix));
	}

	Vector3 Right() const
	{
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation);
		return Vector3(XMVector3TransformNormal(Vector3::RIGHT, rotationMatrix));
	}

	Vector3 Up() const
	{
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation);
		return Vector3(XMVector3TransformNormal(Vector3::UP, rotationMatrix));
	}

	XMMATRIX GetMatrix() const
	{
		return XMMatrixScalingFromVector(scale)
			* XMMatrixRotationRollPitchYawFromVector(rotation)
			* XMMatrixTranslationFromVector(position);
	}
};

