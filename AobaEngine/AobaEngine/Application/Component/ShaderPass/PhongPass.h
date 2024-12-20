#pragma once
#include "../ShaderPass.h"
#include "../ConstantBuffer.h" 
#include "../Camera.h"

struct StandardProperty
{
	DirectX::XMVECTOR baseColor;
	DirectX::XMVECTOR eyePosition;
};

class PhongPass : public ShaderPass
{
	ConstantBuffer<StandardProperty> constantBuffer;

public:
	StandardProperty property;

	PhongPass() :
		constantBuffer(RenderPipeline::GetInstance().device, RenderPipeline::GetInstance().commandList),
		property({ {0.3,0.3,0,0},{ 0,0,0,0 } })
	{
		shaderPath = L"Engine/Shaders/PhongShaders.hlsl";

		psoDesc = {};
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // ラスタライザーステート
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // ブレンドステート
		psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // サンプルマスクの設定
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // トポロジタイプ
		psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED; // ストリップ時のカット設定
		psoDesc.NumRenderTargets = 1; // レンダーターゲット数
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // レンダーターゲットフォーマット
		psoDesc.SampleDesc.Count = 1; // マルチサンプリングの設定

		// 深度ステンシル 
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度バッファーで使用するフォーマット
		psoDesc.DepthStencilState.DepthEnable = true; // 深度バッファーを使用するか
		psoDesc.DepthStencilState.StencilEnable = false; // ステンシルテストを行うか
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 書き込む
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 小さい方を採用する
	}

	void SetBuffer() override
	{
		Vector3 pos = Camera::current->transform->position;
		property.eyePosition = XMVectorSet(pos.x, pos.y, pos.z, 0);

		constantBuffer.SetBufferData(property);
		constantBuffer.SetConstantBufferView(TODO, 3);
	}
};
