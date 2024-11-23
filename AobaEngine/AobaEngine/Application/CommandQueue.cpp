#include "CommandQueue.h"

#include "Helper.h"

CommandQueue::CommandQueue(ID3D12Device* device)
	: fenceValue(0)
	, fenceEvent(nullptr)
{
	// コマンドキューの作成
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // タイムアウト無し
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // コマンドリストと合わせる
	ThrowIfFailed(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.ReleaseAndGetAddressOf())));

	// フェンスの作成
	ThrowIfFailed(device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf())));
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

CommandQueue::~CommandQueue()
{
	CloseHandle(fenceEvent);
}

void CommandQueue::ExecuteCommandList(ID3D12CommandList* commandLists)
{
	commandQueue->ExecuteCommandLists(1, &commandLists);
	ThrowIfFailed(commandQueue->Signal(fence.Get(), ++fenceValue));
}

void CommandQueue::WaitForFence() const
{
	if (fence->GetCompletedValue() < fenceValue)
	{
		ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

ID3D12CommandQueue* CommandQueue::Get() const
{
	return commandQueue.Get();
}
