#include "DirectXTextureManager.h"

DirectXTextureManager::DirectXTextureManager(DirectXClass& ar_DirectXClass, std::string StandardTextureLocation)
{
	CreateTexture(ar_DirectXClass, StandardTextureLocation);
}

DirectXTextureManager::~DirectXTextureManager()
{
}

int DirectXTextureManager::CreateTexture(DirectXClass& ar_DirectXClass , std::string fileLocation)
{
	//- check if its already in use if so use it -//
	for (unsigned int i = 0; i < texturePool.size(); i++)
	{
		if (texturePool[i].FileLocation == fileLocation)
		{
			texturePool[i].UsedByCount++;
			return i;
		}
	}

	//- if not in use then make new texture -//
	DirectXSmartTexture newTexture;
	newTexture.FileLocation = fileLocation;
	newTexture.UsedByCount = 1;
	newTexture.texture = new Texture2D();
	newTexture.texture->Initialize(ar_DirectXClass.GetDevice(),ar_DirectXClass.GetDeviceContext(),fileLocation.c_str());

	//- add to pool after -//
	texturePool.push_back(newTexture);

	//- return 0 index position of texture -//
	return texturePool.size() - 1;
}

void DirectXTextureManager::DeleteTexture(int index)
{
	texturePool[index].UsedByCount--;
}

Texture2D* DirectXTextureManager::GetTexture(int index)
{
	return texturePool[index].texture;
}

void DirectXTextureManager::RemoveUnusedTextures()
{
	for (int i = 0; i < texturePool.size(); i++)
	{
		if (texturePool[i].UsedByCount < 1)
		{
			delete texturePool[i].texture;
			texturePool[i].texture = nullptr;
			texturePool.erase(texturePool.begin() + i);
		}
	}
}
