#pragma once
#include <d3d12.h>
#include <wrl/client.h>

#include "ObjectService.h"
#include "ShaderPass.h"
#include "../Utility/Texture.h"
#include "../RenderPipeline.h"

class Material : public Object
{
public:
	std::shared_ptr<ShaderPass> shaderPass;

	Material();
	void ApplyShaderPass(const std::shared_ptr<ShaderPass>& shaderPass);
	void SetPass(GraphicsContext& context);
};
