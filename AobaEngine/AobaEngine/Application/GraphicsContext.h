#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include <d3dx12.h>

#include <cassert>

#include "Helper.h"

using Microsoft::WRL::ComPtr;

class GraphicsContext
{
public:
	void Create(ID3D12Device* device);

	ID3D12CommandList* GetCommandLists() const;

	void SetViewportAndRect(const D3D12_VIEWPORT& viewport, const D3D12_RECT& rect) const;
	void TransitionResource(ID3D12Resource* gpuResource, D3D12_RESOURCE_STATES oldState, D3D12_RESOURCE_STATES newState) const;
	void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) const;
	void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& indexBufferView) const;
	void SetVertexBuffer(UINT startSlot, const D3D12_VERTEX_BUFFER_VIEW& vertexBufferView) const;
	void Draw(UINT vertexCount, UINT vertexStartOffset) const;
	void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation) const;
	void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) const;
	void SetRootSignature(ID3D12RootSignature* rootSignature) const;
	void ClearBackground(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle) const;
	void ClearStencil(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) const;
	void SetDescriptorHeaps(ID3D12DescriptorHeap* descriptorHeaps, UINT heapCount) const;
	void Close() const;
	void Reset() const;
	void SetGraphicsRootConstantBufferView(UINT rootParameter, D3D12_GPU_VIRTUAL_ADDRESS virtualAddress) const;
	void SetGraphicsRootDescriptorTable(UINT rootParameter, D3D12_GPU_DESCRIPTOR_HANDLE baseDescriptor) const;
	void SetPipelineState(ID3D12PipelineState* pipelineState);

private:
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
};
