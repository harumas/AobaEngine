#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <cassert>

using Microsoft::WRL::ComPtr;


class CommandQueue
{
public:
	CommandQueue(ID3D12Device* device);
	~CommandQueue();

	void ExecuteCommandList(ID3D12CommandList* commandList);
	void WaitForFence() const;
	ID3D12CommandQueue* Get() const;

private:
	ComPtr<ID3D12CommandQueue> commandQueue;

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceValue;
	HANDLE fenceEvent;
};
