#pragma once
#include "Component.h"
#include "ConstantBuffer.h"
#include "../RenderPipeline.h"
#include <DirectXMath.h>

class Light : public Object
{
public:
	Light();

	// ライティング用
	DirectX::XMVECTOR lightDirection = { -0.3f, -0.3f, 0.8f };
	DirectX::XMVECTOR lightColor = { 0.8f, 0.8f, 0.8f };
	DirectX::XMVECTOR ambientLight = { 0.4f, 0.4f, 0.4f };

	void OnDraw(GraphicsContext& context) override;
private:
	struct LightingData
	{
		DirectX::XMVECTOR lightDirection;
		DirectX::XMVECTOR lightColor;
		DirectX::XMVECTOR ambientLight;
	};

	ConstantBuffer<LightingData> constantLightBuffer;
};
