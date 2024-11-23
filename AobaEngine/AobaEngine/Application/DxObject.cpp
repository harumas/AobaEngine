#include <vector>

#include "DxObject.h"

#include <d3dx12.h>

#include "Helper.h"

void DxObject::Create(HWND hwnd)
{
	ComPtr<IDXGIFactory6> dxgiFactory;
	CreateDXGIFactory(dxgiFactory);

	// デバイスの初期化
	CreateD3D12Device(dxgiFactory.Get(), device.ReleaseAndGetAddressOf());

	// コマンドキューの初期化
	commandQueue = std::make_unique<CommandQueue>(device.Get());

	// スワップチェーンの初期化
	CreateSwapChain(dxgiFactory.Get(), hwnd);

	// レンダーターゲットバッファーの初期化
	CreateRenderTargetBuffer();

	// 深度バッファーの初期化
	CreateDepthStencilBuffer();

	// シェーダーリソースバッファーの初期化
	CreateShaderResourceBuffer();

	//深度シェーダーリソースビュー
	D3D12_DESCRIPTOR_HEAP_DESC depthSrvHeapDesc = {};
	depthSrvHeapDesc.NodeMask = 0;
	depthSrvHeapDesc.NumDescriptors = 1;
	depthSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	depthSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(device->CreateDescriptorHeap(&depthSrvHeapDesc, IID_PPV_ARGS(&depthSRVHeap)));

	// ルートシグネチャの初期化
	CreateRootSignature();
}

// D3D12Deviceの生成
void DxObject::CreateD3D12Device(IDXGIFactory6* dxgiFactory, ID3D12Device** devicePtr)
{
	ID3D12Device* tmpDevice = nullptr;

	// グラフィックスボードの選択
	std::vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; SUCCEEDED(dxgiFactory->EnumAdapters(i, &tmpAdapter)); ++i)
	{
		adapters.push_back(tmpAdapter);
	}

	for (auto adapter : adapters)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		adapter->GetDesc(&adapterDesc);
		// AMDを含むアダプターオブジェクトを探して格納（見つからなければnullptrでデフォルト）
		// 製品版の場合は、オプション画面から選択させて設定する必要がある
		std::wstring strAdapter = adapterDesc.Description;
		if (strAdapter.find(L"NVIDIA") != std::string::npos)
		{
			tmpAdapter = adapter;
			break;
		}
	}

	// Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	for (auto level : levels)
	{
		if (D3D12CreateDevice(tmpAdapter, level, IID_PPV_ARGS(&tmpDevice)) == S_OK)
		{
			break; // 生成可能なバージョンが見つかったらループを打ち切り
		}
	}

	*devicePtr = tmpDevice;
}


void DxObject::CreateCommittedResources(const D3D12_HEAP_PROPERTIES* pHeapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue, ComPtr<ID3D12Resource>& ppvResource)
{
	ThrowIfFailed(device->CreateCommittedResource(
		pHeapProperties,
		HeapFlags,
		pDesc,
		InitialResourceState,
		pOptimizedClearValue,
		IID_PPV_ARGS(ppvResource.ReleaseAndGetAddressOf())
	));
}

UINT DxObject::GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType)
{
	return device->GetDescriptorHandleIncrementSize(descriptorHeapType);
}

void DxObject::CreateGraphicsPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC* psoDesc,
	ComPtr<ID3D12PipelineState>& pipelineState)
{
	ThrowIfFailed(device->CreateGraphicsPipelineState(psoDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf())));
}

D3D12_CPU_DESCRIPTOR_HANDLE DxObject::GetCPUDescriptorHandle(UINT heapIndex) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE startAddress = srvHeap->GetCPUDescriptorHandleForHeapStart();
	startAddress.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * heapIndex;
	return startAddress;
}

D3D12_GPU_DESCRIPTOR_HANDLE DxObject::GetGPUDescriptorHandle(UINT heapIndex) const
{
	D3D12_GPU_DESCRIPTOR_HANDLE startAddress = srvHeap->GetGPUDescriptorHandleForHeapStart();
	startAddress.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * heapIndex;
	return startAddress;
}

const std::unique_ptr<CommandQueue>& DxObject::GetCommandQueue() const
{
	return commandQueue;
}

ID3D12Device* DxObject::GetDevice() const
{
	return device.Get();
}

IDXGISwapChain4* DxObject::GetSwapChain() const
{
	return swapChain.Get();
}

ID3D12RootSignature* DxObject::GetRootSignature() const
{
	return rootSignature.Get();
}

ID3D12DescriptorHeap* DxObject::GetDescriptorHeaps() const
{
	return srvHeap.Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE DxObject::GetDSVHandle() const
{
	return dsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void DxObject::CreateDXGIFactory(ComPtr<IDXGIFactory6>& dxgiFactory)
{
	UINT dxgiFactoryFlags = 0;

#ifdef _DEBUG
	// デバッグレイヤーを有効にする
	ComPtr<ID3D12Debug> debugLayer;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer))))
	{
		debugLayer->EnableDebugLayer();
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	// DXGIFactoryの初期化
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf())));
}

void DxObject::CreateSwapChain(IDXGIFactory6* dxgiFactory, HWND hwnd)
{
	// スワップチェーンの生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.Width = appContext->GetWindowWidth();
	swapchainDesc.Height = appContext->GetWindowHeight();
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.SampleDesc.Count = 1;
	ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
		commandQueue->Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChain.ReleaseAndGetAddressOf())));
}

void DxObject::CreateRenderTargetBuffer()
{
}

void DxObject::CreateDepthStencilBuffer()
{
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ThrowIfFailed(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap.ReleaseAndGetAddressOf())));

	// 深度バッファー作成
	D3D12_RESOURCE_DESC depthResDesc = {};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = appContext->GetWindowWidth();
	depthResDesc.Height = appContext->GetWindowHeight();
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	depthResDesc.MipLevels = 1;
	depthResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthResDesc.Alignment = 0;

	auto depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// クリアバリューの設定
	D3D12_CLEAR_VALUE _depthClearValue = {};
	_depthClearValue.DepthStencil.Depth = 1.0f;      //深さ１(最大値)でクリア
	_depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //32bit深度値としてクリア
	ThrowIfFailed(device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //デプス書き込みに使用
		&_depthClearValue,
		IID_PPV_ARGS(depthBuffer.ReleaseAndGetAddressOf())));

	// 深度バッファービュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	device->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DxObject::CreateShaderResourceBuffer()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 50;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(srvHeap.ReleaseAndGetAddressOf())));

	D3D12_SHADER_RESOURCE_VIEW_DESC resDesc = {};
	resDesc.Format = DXGI_FORMAT_R32_FLOAT;
	resDesc.Texture2D.MipLevels = 1;
	resDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	resDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(depthBuffer.Get(), &resDesc, depthSRVHeap->GetCPUDescriptorHandleForHeapStart());
}

void DxObject::CreateRootSignature()
{
	// ルートパラメータの生成
	// ディスクリプタテーブルの実体
	CD3DX12_DESCRIPTOR_RANGE1 discriptorRanges[2];
	discriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC); // SRV (テクスチャ)
	discriptorRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC); // SRV (深度テクスチャ)

	CD3DX12_ROOT_PARAMETER1 rootParameters[5];
	rootParameters[0].InitAsConstantBufferView(0, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_ALL); // CBV (Model + ViewProjection)
	rootParameters[1].InitAsConstantBufferView(1, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_ALL); // CBV (EnvironmentLight)
	rootParameters[2].InitAsConstantBufferView(2, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_ALL); // CBV (PointLight)
	rootParameters[3].InitAsConstantBufferView(3, 0, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_ALL); // CBV (Material)
	rootParameters[4].InitAsDescriptorTable(1, discriptorRanges, D3D12_SHADER_VISIBILITY_ALL); // SRV (Texture)

	// サンプラーの生成
	// テクスチャデータからどう色を取り出すかを決めるための設定
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートパラメータ、サンプラーからルートシグネチャを生成
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> rootSignatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, &errorBlob));
	ThrowIfFailed(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf())));
}

