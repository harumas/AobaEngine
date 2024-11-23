#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

#include "Utility/Texture.h"

struct Mesh;
struct Vertex;

struct aiMesh;
struct aiMaterial;

struct ImportSettings // インポートするときのパラメータ
{
	const wchar_t* filename = nullptr; // ファイルパス
	std::vector<Mesh>& meshes; // 出力先のメッシュ配列
};

class FbxLoader
{
public:
	static	bool Load(const std::wstring& filePath, Mesh* mesh, Texture* texture); // モデルをロードする

private:
	static void LoadMesh(Mesh& dst, const aiMesh* src);
	static std::wstring GetTexturePath(const wchar_t* filename, const aiMaterial* src);
};