#pragma once
#include "Component.h"
#include "ConstantBuffer.h"
#include "Material.h"
#include <DirectXMath.h>
#include <memory>
#include <string>
#include "GameObject.h"
#include "../Utility/Mesh.h"
#include <d3d12.h>
#include <wrl/client.h>

struct Matrices
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProj;
};

class Renderer : public Component
{
public:
	explicit Renderer(const std::shared_ptr<GameObject>& gameObjectPtr);

	void LoadMesh(const std::wstring& filePath);
	void ApplyMaterial(const std::shared_ptr<Material>& material);
	void OnDraw(GraphicsContext& context) override;

private:
	Mesh mesh;
	Texture texture;
	std::shared_ptr<Material> material;

	bool hasMaterial;

	ComPtr<ID3D12Resource> vertexBuffer_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	ComPtr<ID3D12Resource> indexBuffer_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	ConstantBuffer<Matrices> matrixBuffer;

	void CreateVertexBuffer();
	void CreateIndexBuffer();
};
