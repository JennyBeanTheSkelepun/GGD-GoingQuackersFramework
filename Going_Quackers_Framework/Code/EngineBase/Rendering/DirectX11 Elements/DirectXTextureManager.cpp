#include "DirectXTextureManager.h"

DirectXTextureManager::DirectXTextureManager(DirectXClass& ar_DirectXClass, std::string as_standardTextureLocation)
{
	CreateTexture(ar_DirectXClass, as_standardTextureLocation);
}

DirectXTextureManager::~DirectXTextureManager()
{
}

int DirectXTextureManager::CreateTexture(DirectXClass& ar_DirectXClass , std::string as_fileLocation)
{
	//- check if its already in use if so use it -//
	for (unsigned int li_i(0); li_i < m_texturePool.size(); ++li_i)
	{
		if (m_texturePool[li_i].ms_fileLocation == as_fileLocation)
		{
			m_texturePool[li_i].mi_usedByCount++;
			return li_i;
		}
	}

	//- if not in use then make new mp_texture -//
	DirectXSmartTexture l_newTexture;
	l_newTexture.ms_fileLocation = as_fileLocation;
	l_newTexture.mi_usedByCount = 1;
	l_newTexture.mp_texture = new Texture2D();
	l_newTexture.mp_texture->Initialize(ar_DirectXClass.GetDevice(), ar_DirectXClass.GetDeviceContext(), as_fileLocation.c_str());

	//- add to pool after -//
	m_texturePool.push_back(l_newTexture);

	//- return 0 ai_index position of mp_texture -//
	return m_texturePool.size() - 1;
}

int DirectXTextureManager::DeleteTexture(int ai_index)
{
	m_texturePool[ai_index].mi_usedByCount--;
	return -1;
}

Texture2D* DirectXTextureManager::GetTexture(int ai_index)
{
	return m_texturePool[ai_index].mp_texture;
}

void DirectXTextureManager::RemoveUnusedTextures()
{
	int li_texturePoolSize = m_texturePool.size();
	for (int li_i(0); li_i < li_texturePoolSize; ++li_i)
	{
		if (m_texturePool[li_i].mi_usedByCount < 1)
		{
			delete m_texturePool[li_i].mp_texture;
			m_texturePool[li_i].mp_texture = nullptr;
			m_texturePool.erase(m_texturePool.begin() + li_i);
		}
	}
}
