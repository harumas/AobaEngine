#pragma once
#include <d3d12.h>
#include <vector>

struct Vertex
{
	DirectX::XMFLOAT3 Position; // �ʒu���W
	DirectX::XMFLOAT3 Normal; // �@��
	DirectX::XMFLOAT2 UV; // uv���W
	DirectX::XMFLOAT3 Tangent; // �ڋ��
	DirectX::XMFLOAT4 Color; // ���_�F
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	D3D12_PRIMITIVE_TOPOLOGY topology;
};
