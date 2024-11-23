#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include "../RenderPipeline.h"
#include <dxgiformat.h>
#include <windows.h>
#include <combaseapi.h>
#include <d3dcommon.h>
#include <string>

#include "../DxObject.h"
#include "../Helper.h"

using Microsoft::WRL::ComPtr;

class ShaderPass
{
public:
	std::wstring shaderPath;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ComPtr<ID3D12PipelineState> pipelineState;         // パイプラインステート

	void Compile(ID3D12RootSignature* rootSignature, DxObject* dxObject)
	{
		// パイプラインステートの生成
		// シェーダーオブジェクトの生成
#if defined(_DEBUG)
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		ComPtr<ID3DBlob> vsBlob;
		ComPtr<ID3DBlob> psBlob;

		ThrowIfFailed(D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", compileFlags, 0, &vsBlob, nullptr));
		ThrowIfFailed(D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", compileFlags, 0, &psBlob, nullptr));

		// 頂点レイアウトの生成
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		// パイプラインステートオブジェクト(PSO)を生成
		psoDesc.pRootSignature = rootSignature;
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) }; // 入力レイアウトの設定
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());                       // 頂点シェーダ
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());                       // ピクセルシェーダ

		dxObject->CreateGraphicsPipelineState(&psoDesc, pipelineState);
	}

	virtual	void SetBuffer() {}
};
