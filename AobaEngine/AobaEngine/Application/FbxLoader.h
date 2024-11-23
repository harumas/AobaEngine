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

struct ImportSettings // �C���|�[�g����Ƃ��̃p�����[�^
{
	const wchar_t* filename = nullptr; // �t�@�C���p�X
	std::vector<Mesh>& meshes; // �o�͐�̃��b�V���z��
};

class FbxLoader
{
public:
	static	bool Load(const std::wstring& filePath, Mesh* mesh, Texture* texture); // ���f�������[�h����

private:
	static void LoadMesh(Mesh& dst, const aiMesh* src);
	static std::wstring GetTexturePath(const wchar_t* filename, const aiMaterial* src);
};