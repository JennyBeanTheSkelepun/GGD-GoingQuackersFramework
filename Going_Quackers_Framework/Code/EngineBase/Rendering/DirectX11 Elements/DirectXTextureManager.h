#ifndef _DIRECT_X_TEXTURE_MANAGER_H_
#define _DIRECT_X_TEXTURE_MANAGER_H_

#include <vector>
#include <string>

#include "DirectXClass.h"
#include "DirectXTexture2D.h"

struct DirectXSmartTexture
{
	std::string FileLocation;
	int UsedByCount;
	Texture2D* texture;
};

class DirectXTextureManager
{
public:
	DirectXTextureManager(DirectXClass& ar_DirectXClass, std::string StandardTextureLocation);
	~DirectXTextureManager();

	int CreateTexture(DirectXClass& ar_DirectXClass, std::string fileLocation);
	int DeleteTexture(int index);
	void RemoveUnusedTextures();
	Texture2D* GetTexture(int index);


	std::vector<DirectXSmartTexture> texturePool;
};

#endif /* NAME OF HEADER IN CAPS */