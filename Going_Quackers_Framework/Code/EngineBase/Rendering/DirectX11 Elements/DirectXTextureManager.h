#ifndef _DIRECT_X_TEXTURE_MANAGER_H_
#define _DIRECT_X_TEXTURE_MANAGER_H_

#include <vector>
#include <string>

#include "DirectXClass.h"
#include "DirectXTexture2D.h"

struct DirectXSmartTexture
{
	std::string ms_fileLocation;
	int mi_usedByCount;
	Texture2D* mp_texture;
};

class DirectXTextureManager
{
public:
	DirectXTextureManager(DirectXClass& ar_DirectXClass, std::string as_StandardTextureLocation);
	~DirectXTextureManager();

	int CreateTexture(DirectXClass& ar_DirectXClass, std::string as_fileLocation);
	int DeleteTexture(int ai_index);

	void RemoveUnusedTextures();
	Texture2D* GetTexture(int ai_index);

	std::vector<DirectXSmartTexture> m_texturePool;
};

#endif /* NAME OF HEADER IN CAPS */