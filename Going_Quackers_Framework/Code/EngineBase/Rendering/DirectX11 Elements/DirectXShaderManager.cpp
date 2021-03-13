#include "DirectXShaderManager.h"

DirectXShaderManager::DirectXShaderManager(DirectXClass& ar_DirectXClass, DirectXWindow& ar_DirectXWindow, std::wstring StandardshaderLocation)
{
	CreateShader(ar_DirectXClass, ar_DirectXWindow, StandardshaderLocation);
}

DirectXShaderManager::~DirectXShaderManager()
{
}

int DirectXShaderManager::CreateShader(DirectXClass& ar_DirectXClass, DirectXWindow& ar_DirectXWindow , std::wstring fileLocation)
{
	//- check if its already in use if so use it -//
	for (unsigned int i = 0; i < ShaderPool.size(); i++)
	{
		if (ShaderPool[i].FileLocation == fileLocation)
		{
			ShaderPool[i].UsedByCount++;
			return i;
		}
	}

	//- if not in use then make new shader -//
	DirectXSmartShader newshader;
	newshader.FileLocation = fileLocation;
	newshader.UsedByCount = 1;
	newshader.Shader = new DirectXShader();
	newshader.Shader->Initialize(ar_DirectXClass.GetDevice(), ar_DirectXWindow.m_hwnd, fileLocation.c_str());

	//- add to pool after -//
	ShaderPool.push_back(newshader);

	//- return 0 index position of shader -//
	return ShaderPool.size() - 1;
}

int DirectXShaderManager::DeleteShader(int index)
{
	ShaderPool[index].UsedByCount--;
	return -1;
}

DirectXShader* DirectXShaderManager::GetShader(int index)
{
	return ShaderPool[index].Shader;
}

void DirectXShaderManager::RemoveUnusedShader()
{
	for (int i = 0; i < ShaderPool.size(); i++)
	{
		if (ShaderPool[i].UsedByCount < 1)
		{
			delete ShaderPool[i].Shader;
			ShaderPool[i].Shader = nullptr;
			ShaderPool.erase(ShaderPool.begin() + i);
		}
	}
}