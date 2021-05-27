#ifndef _DIRECT_X_SHADER_MANAGER_H_
#define _DIRECT_X_SHADER_MANAGER_H_

#include <string>
#include <vector>

#include "DirectXClass.h"
#include "DirectXShader.h"
#include "DirectXWindow.h"

struct DirectXSmartShader {

	std::wstring ms_fileLocation;
	int mi_usedByCount;
	DirectXShader* mp_shader;
};

class DirectXShaderManager
{
public:
	DirectXShaderManager(DirectXClass& ar_DirectXClass, DirectXWindow& ar_DirectXWindow, std::wstring as_fileLocation);
	~DirectXShaderManager();

	int CreateShader(DirectXClass& ar_DirectXClass, DirectXWindow& ar_DirectXWindow, std::wstring as_fileLocation);
	int DeleteShader(int ai_index);
	DirectXShader* GetShader(int ai_index);
	void RemoveUnusedShader();
	std::vector<DirectXSmartShader> m_shaderPool;

};

#endif /* _DIRECT_X_SHADER_MANAGER_H_ */