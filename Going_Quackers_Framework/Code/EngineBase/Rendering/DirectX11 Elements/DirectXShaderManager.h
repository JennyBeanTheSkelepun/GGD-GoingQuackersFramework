#ifndef _DIRECT_X_SHADER_MANAGER_H_
#define _DIRECT_X_SHADER_MANAGER_H_

#include <string>
#include <vector>

#include "DirectXClass.h"
#include "DirectXShader.h"
#include "DirectXWindow.h"

struct DirectXSmartShader {

	std::string FileLocation;
	int UsedByCount;
	DirectXShader* Shader;
};

class DirectXShaderManager
{
public:
	DirectXShaderManager(DirectXClass& ar_DirectXClass, DirectXWindow& ar_DirectXWindow, std::string fileLocation);
	~DirectXShaderManager();

	int CreateShader(DirectXClass& ar_DirectXClass, DirectXWindow& ar_DirectXWindow, std::string fileLocation);
	int DeleteShader(int index);
	DirectXShader* GetShader(int index);
	void RemoveUnusedShader();

	std::vector<DirectXSmartShader> ShaderPool;

};

#endif /* _DIRECT_X_SHADER_MANAGER_H_ */