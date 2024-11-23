#include "RenderTargetBuffer.h"
#include <d3dx12.h>
#include "Helper.h"


void RenderTargetBuffer::Create(ID3D12Device* device, IDXGISwapChain4* swapChain)
{
	this->device = device;
	this->swapChain = swapChain;

	// レンダーターゲットバッファーの生成
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = kFrameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(rtvHeap.ReleaseAndGetAddressOf())));

	// スワップチェーンと関連付けてレンダーターゲットビューを生成
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < kFrameCount; ++i)
	{
		ThrowIfFailed(swapChain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(renderTargets[i].ReleaseAndGetAddressOf())));
		device->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}
}

ID3D12Resource* RenderTargetBuffer::GetCurrentBackBuffer()
{
	return renderTargets[swapChain->GetCurrentBackBufferIndex()].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetBuffer::GetRTVHandle()
{
	auto frameIndex = swapChain->GetCurrentBackBufferIndex();
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	return rtvHandle;
}

void RenderTargetBuffer::SwapBuffer()
{
	ThrowIfFailed(swapChain->Present(1, 0));
}
