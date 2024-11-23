#pragma once
#include <string>
#include "Texture.h"

class TextureLoader
{
public:
	static void LoadTexture(Texture& texture, const std::wstring& texturePath);
private:
	static UINT idCounter;
};

