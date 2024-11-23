#include "GraphicsContext.h"


void GraphicsContext::Create(ID3D12Device* device)
{
	ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));


}

ID3D12CommandList* GraphicsContext::GetCommandLists() const
{
	return commandList.Get();
}

void GraphicsContext::SetViewportAndRect(const D3D12_VIEWPORT& viewport, const D3D12_RECT& rect) const
{
	assert(rect.left < rect.right && rect.top < rect.bottom);
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &rect);
}

void GraphicsContext::TransitionResource(ID3D12Resource* gpuResource, D3D12_RESOURCE_STATES oldState,
	D3D12_RESOURCE_STATES newState) const
{
	const CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(gpuResource, oldState, newState);
	commandList->ResourceBarrier(1, &barrier);
}

void GraphicsContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const
{
	commandList->IASetPrimitiveTopology(topology);
}

void GraphicsContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBufferView) const
{
	commandList->IASetIndexBuffer(&indexBufferView);
}

void GraphicsContext::SetVertexBuffer(UINT startSlot, const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView) const
{
	commandList->IASetVertexBuffers(startSlot, 1, &vertexBufferView);
}

void GraphicsContext::Draw(UINT vertexCount, UINT vertexStartOffset) const
{
	DrawInstanced(vertexCount, 1, vertexStartOffset, 0);
}

void GraphicsContext::DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation) const
{
	commandList->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}

void GraphicsContext::SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle,
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) const
{
	commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
}

void GraphicsContext::SetRootSignature(ID3D12RootSignature* rootSignature) const
{
	commandList->SetGraphicsRootSignature(rootSignature);
}

void GraphicsContext::ClearBackground(D3D12_CPU_DESCRIPTOR_HANDLE colorBufferHandle) const
{
	//TODO:とりあえず空色でクリア あとで色指定出来るようにする
	float clearColor[] = { 0.390625f, 0.58203125f, 0.92578125f, 1.0f };
	commandList->ClearRenderTargetView(colorBufferHandle, clearColor, 0, nullptr);
}

void GraphicsContext::ClearStencil(D3D12_CPU_DESCRIPTOR_HANDLE stencilBufferHandle) const
{
	commandList->ClearDepthStencilView(stencilBufferHandle, D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
}

void GraphicsContext::SetDescriptorHeaps(ID3D12DescriptorHeap* descriptorHeaps, UINT heapCount) const
{
	commandList->SetDescriptorHeaps(heapCount, &descriptorHeaps);
}

void GraphicsContext::Close() const
{
	commandList->Close();
}

void GraphicsContext::Reset() const
{
	commandAllocator->Reset();
	commandList->Reset(commandAllocator.Get(), nullptr);
}

void GraphicsContext::SetGraphicsRootConstantBufferView(UINT rootParameter, D3D12_GPU_VIRTUAL_ADDRESS virtualAddress) const
{
	commandList->SetGraphicsRootConstantBufferView(rootParameter, virtualAddress);
}

void GraphicsContext::SetGraphicsRootDescriptorTable(UINT rootParameter,
	D3D12_GPU_DESCRIPTOR_HANDLE baseDescriptor) const
{
	commandList->SetGraphicsRootDescriptorTable(rootParameter, baseDescriptor);
}

void GraphicsContext::SetPipelineState(ID3D12PipelineState* pipelineState)
{
	commandList->SetPipelineState(pipelineState);
}
