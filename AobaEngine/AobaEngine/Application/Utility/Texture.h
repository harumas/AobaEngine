#pragma once
#include <d3d12.h>
#include <intsafe.h>
#include <wrl/client.h>

struct Texture
{
	Microsoft::WRL::ComPtr<ID3D12Resource> textureBuffer;
	UINT bufferId = UINT16_ERROR;
};
