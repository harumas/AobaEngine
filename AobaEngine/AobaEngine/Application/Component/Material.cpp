#include "Material.h"

#include "../Utility/TextureLoader.h"


Material::Material()
{
}

void Material::ApplyShaderPass(const std::shared_ptr<ShaderPass>& shaderPass)
{
	this->shaderPass = shaderPass;
}

void Material::SetPass(GraphicsContext& context)
{
	shaderPass->SetBuffer();

	const ComPtr<ID3D12PipelineState>& state = shaderPass->pipelineState;
	context.SetPipelineState(state.Get());
}

