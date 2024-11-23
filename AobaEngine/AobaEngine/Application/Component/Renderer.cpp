#include "Renderer.h"

#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "../AppInfo.h"
#include <dxgiformat.h>
#include <windows.h>
#include <combaseapi.h>
#include <d3d12.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <memory>
#include <string>
#include <vector>
#include "Component.h"
#include "Material.h"
#include "../FbxLoader.h"
#include "../Helper.h"
#include "../RenderPipeline.h"
#include "../Utility/Mesh.h"

Renderer::Renderer(const std::shared_ptr<GameObject>& gameObjectPtr) :
	Component(gameObjectPtr),
	mesh(),
	hasMaterial(false),
	vertexBufferView_(),
	indexBufferView_()
{
}

void Renderer::LoadMesh(const std::wstring& filePath)
{
	// fbx���f���̃��[�h
	if (!FbxLoader::Load(filePath, &mesh, &texture))
	{
		ThrowMessage("failed load fbx file.");
	}

	//�O�p�|���S��
	mesh.topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// ���_�o�b�t�@�r���[�̐���
	CreateVertexBuffer();

	// �C���f�b�N�X�o�b�t�@�r���[�̐���
	CreateIndexBuffer();
}

void Renderer::CreateVertexBuffer()
{
	// ���_���W
	std::vector<Vertex> vertices = mesh.vertices;
	const UINT vertexBufferSize = sizeof(Vertex) * vertices.size();
	auto vertexHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto vertexResDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

	// ���_�o�b�t�@�[�̐���
	DxObject::CreateCommittedResources(
		&vertexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&vertexResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		vertexBuffer_);

	// ���_���̃R�s�[
	Vertex* vertexMap = nullptr;
	ThrowIfFailed(vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexMap)));
	std::copy(std::begin(vertices), std::end(vertices), vertexMap);
	vertexBuffer_->Unmap(0, nullptr);

	// ���_�o�b�t�@�[�r���[�̐���
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = vertexBufferSize;
	vertexBufferView_.StrideInBytes = sizeof(Vertex);
}

void Renderer::CreateIndexBuffer()
{
	// �C���f�b�N�X���W
	std::vector<unsigned short> indices = mesh.indices;
	const UINT indexBufferSize = sizeof(unsigned short) * indices.size();
	auto indexHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto indexResDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

	// �C���f�b�N�X�o�b�t�@�̐���
	DxObject::CreateCommittedResources(
		&indexHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&indexResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		indexBuffer_);

	// �C���f�b�N�X���̃R�s�[
	unsigned short* indexMap = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexMap));
	std::copy(std::begin(indices), std::end(indices), indexMap);
	indexBuffer_->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̐���
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = indexBufferSize;
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;

}


void Renderer::ApplyMaterial(const std::shared_ptr<Material>& material)
{
	this->material = material;
	hasMaterial = true;
}

void Renderer::OnDraw(GraphicsContext& context)
{
	material->SetPass(TODO);

	if (texture.bufferId != UINT16_ERROR)
	{
		auto handle = pipeline.GetGPUDescriptorHandle(texture.bufferId);
		context.SetGraphicsRootDescriptorTable(4, handle);
	}

	// �`�揈���̐ݒ�
	context.SetPrimitiveTopology(mesh.topology); // �v���~�e�B�u�g�|���W�̐ݒ� (�O�p�|���S��)
	context.SetVertexBuffer(0, vertexBufferView_);                // ���_�o�b�t�@
	context.SetIndexBuffer(indexBufferView_);                         // �C���f�b�N�X�o�b�t�@

	const DirectX::XMMATRIX world = gameObject.lock()->GetComponent<Transform>()->GetMatrix();
	const DirectX::XMMATRIX viewProj = Camera::current->GetViewMatrix() * Camera::current->GetProjectionMatrix(AppInfo::GetWindowAspectRatio());

	matrixBuffer.SetBufferData({ world,viewProj });
	matrixBuffer.SetConstantBufferView(context, 0);

	context.Draw(mesh.indices.size(), 0);
}
