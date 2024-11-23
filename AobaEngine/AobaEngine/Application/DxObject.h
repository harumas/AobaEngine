#pragma once
#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

#include "AppContext.h"
#include "CommandQueue.h"

using Microsoft::WRL::ComPtr;

///DirectXのオブジェクトを保持するクラス
class DxObject
{
public:

	/*
	 *@fn
	 * DirectXのオブジェクトを生成します
	 *@return ウィンドウの幅
	 */
	void Create(HWND hwnd);

	void CreateCommittedResources(const D3D12_HEAP_PROPERTIES* pHeapProperties,
		D3D12_HEAP_FLAGS HeapFlags,
		const D3D12_RESOURCE_DESC* pDesc,
		D3D12_RESOURCE_STATES InitialResourceState,
		const D3D12_CLEAR_VALUE* pOptimizedClearValue,
		ComPtr<ID3D12Resource>& ppvResource);

	UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType);
	void CreateGraphicsPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC* psoDesc, ComPtr<ID3D12PipelineState>& pipelineState);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(UINT heapIndex) const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(UINT heapIndex) const;

	const std::unique_ptr<CommandQueue>& GetCommandQueue() const;
	ID3D12Device* GetDevice() const;
	IDXGISwapChain4* GetSwapChain() const;
	ID3D12RootSignature* GetRootSignature() const;
	ID3D12DescriptorHeap* GetDescriptorHeaps() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const;

private:

	std::shared_ptr<IAppContext> appContext;
	std::unique_ptr<CommandQueue> commandQueue;

	ComPtr<ID3D12Device> device;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;              // 深度バッファーヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;              // シェーダーリソースヒープ
	ComPtr<ID3D12DescriptorHeap> depthSRVHeap;         // 深度値テクスチャ用
	ComPtr<ID3D12Resource> depthBuffer;                // 深度バッファー
	ComPtr<ID3D12RootSignature> rootSignature;         // ルートシグネチャ

	void CreateD3D12Device(IDXGIFactory6* dxgiFactory, ID3D12Device** devicePtr);
	void CreateDXGIFactory(ComPtr<IDXGIFactory6>& dxgiFactory);
	void CreateSwapChain(IDXGIFactory6* dxgiFactory, HWND hwnd);
	void CreateRenderTargetBuffer();
	void CreateDepthStencilBuffer();
	void CreateShaderResourceBuffer();
	void CreateRootSignature();
};


