#pragma once
#include <d3d12.h>
#include <vector>

struct Vertex
{
	DirectX::XMFLOAT3 Position; // 位置座標
	DirectX::XMFLOAT3 Normal; // 法線
	DirectX::XMFLOAT2 UV; // uv座標
	DirectX::XMFLOAT3 Tangent; // 接空間
	DirectX::XMFLOAT4 Color; // 頂点色
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	D3D12_PRIMITIVE_TOPOLOGY topology;
};
