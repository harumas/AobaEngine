#include "FbxLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <d3dx12.h>
#include <filesystem>

#include "Utility/Mesh.h"
#include "Utility/Texture.h"
#include "Utility/TextureLoader.h"

namespace fs = std::filesystem;

std::wstring GetDirectoryPath(const std::wstring& origin)
{
	fs::path p = origin.c_str();
	return p.remove_filename().c_str();
}

std::string ToUTF8(const std::wstring& value)
{
	auto length = WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, nullptr, 0, nullptr, nullptr);
	auto buffer = new char[length];

	WideCharToMultiByte(CP_UTF8, 0U, value.data(), -1, buffer, length, nullptr, nullptr);

	std::string result(buffer);
	delete[] buffer;
	buffer = nullptr;

	return result;
}

// std::string(マルチバイト文字列)からstd::wstring(ワイド文字列)を得る
std::wstring ToWideString(const std::string& str)
{
	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(num1);

	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);

	assert(num1 == num2);
	return wstr;
}

bool FbxLoader::Load(const std::wstring& filePath, Mesh* mesh, Texture* texture)
{
	if (filePath.empty())
	{
		return false;
	}

	auto path = ToUTF8(filePath);

	Assimp::Importer importer;
	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

	auto scene = importer.ReadFile(path, flag);

	if (scene == nullptr)
	{
		// もし読み込みエラーがでたら表示する
		printf(importer.GetErrorString());
		printf("\n");
		return false;
	}

	// 読み込んだデータを自分で定義したMesh構造体に変換する
	const auto pMesh = scene->mMeshes[0];
	LoadMesh(*mesh, pMesh);

	const auto pMaterial = scene->mMaterials[0];
	const std::wstring texturePath = GetTexturePath(filePath.c_str(), pMaterial);

	// テクスチャの読み込み
	TextureLoader::LoadTexture(*texture, texturePath);

	scene = nullptr;

	return true;
}

void FbxLoader::LoadMesh(Mesh& dst, const aiMesh* src)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);

	dst.vertices.resize(src->mNumVertices);

	for (auto i = 0u; i < src->mNumVertices; ++i)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		auto tangent = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &zero3D;
		auto color = (src->HasVertexColors(0)) ? &(src->mColors[0][i]) : &zeroColor;

		Vertex vertex = {};
		vertex.Position = DirectX::XMFLOAT3(position->x, position->y, position->z);
		vertex.Normal = DirectX::XMFLOAT3(normal->x, normal->y, normal->z);
		vertex.UV = DirectX::XMFLOAT2(uv->x, uv->y);
		vertex.Tangent = DirectX::XMFLOAT3(tangent->x, tangent->y, tangent->z);
		vertex.Color = DirectX::XMFLOAT4(color->r, color->g, color->b, color->a);

		dst.vertices[i] = vertex;
	}

	dst.indices.resize(src->mNumFaces * 3);

	for (auto i = 0u; i < src->mNumFaces; ++i)
	{
		const auto& face = src->mFaces[i];

		dst.indices[i * 3 + 0] = face.mIndices[0];
		dst.indices[i * 3 + 1] = face.mIndices[1];
		dst.indices[i * 3 + 2] = face.mIndices[2];
	}
}

std::wstring FbxLoader::GetTexturePath(const wchar_t* filename, const aiMaterial* src)
{
	aiString path;

	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		// テクスチャパスは相対パスで入っているので、ファイルの場所とくっつける
		auto dir = GetDirectoryPath(filename);
		auto file = std::string(path.C_Str());
		return dir + ToWideString(file);
	}

	return L"";
}