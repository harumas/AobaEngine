#pragma once
#include <d3d12.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class RenderTargetBuffer
{
public:
	void Create(ID3D12Device* device, IDXGISwapChain4* swapChain);
	ID3D12Resource* GetCurrentBackBuffer();
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle();
	void SwapBuffer();

private:
	static constexpr unsigned int kFrameCount = 2;

	ID3D12Device* device = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ComPtr<ID3D12Resource> renderTargets[kFrameCount]; // バックバッファー
	ComPtr<ID3D12DescriptorHeap> rtvHeap;              // レンダーターゲットヒープ
};
