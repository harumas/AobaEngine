#include "Light.h"

Light::Light() :constantLightBuffer(RenderPipeline::GetInstance().device, RenderPipeline::GetInstance().commandList)
{
}

void Light::OnDraw(GraphicsContext& context)
{
	const LightingData lightingData = { lightDirection,lightColor,ambientLight };

	constantLightBuffer.SetBufferData(lightingData);
	constantLightBuffer.SetConstantBufferView(TODO, 1);
}

