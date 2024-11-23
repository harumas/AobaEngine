#pragma once

#pragma once
#include "Component.h"
#include "ConstantBuffer.h"
#include "../RenderPipeline.h"
#include "../Utility/MathHelper.h"

class PointLight : public Object
{
public:
	PointLight();

	// ライティング用
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	Vector3 lightColor = { 0.8f, 0.8f, 1.0f };
	float lightRange = 3.0f;

	void OnDraw(GraphicsContext& context) override;
private:
	struct LightingData
	{
		DirectX::XMVECTOR lightPosition;
		DirectX::XMVECTOR lightColor;
		float range;
	};

	ConstantBuffer<LightingData> constantLightBuffer;
};

