#include "DirectXShaderManager.h"

DirectXShaderManager::DirectXShaderManager(DirectXClass& ar_DirectXClass, DirectXWindow& ar_DirectXWindow, std::wstring as_standardshaderLocation)
{
	CreateShader(ar_DirectXClass, ar_DirectXWindow, as_standardshaderLocation);
}

DirectXShaderManager::~DirectXShaderManager()
{
}

int DirectXShaderManager::CreateShader(DirectXClass& ar_DirectXClass, DirectXWindow& ar_DirectXWindow , std::wstring as_fileLocation)
{
	//- check if its already in use if so use it -//
	for (unsigned int li_i = 0; li_i < m_shaderPool.size(); li_i++)
	{
		if (m_shaderPool[li_i].ms_fileLocation == as_fileLocation)
		{
			m_shaderPool[li_i].mi_usedByCount++;
			return li_i;
		}
	}

	//- if not in use then make new shader -//
	DirectXSmartShader l_newshader;
	l_newshader.ms_fileLocation = as_fileLocation;
	l_newshader.mi_usedByCount = 1;
	l_newshader.mp_shader = new DirectXShader();
	l_newshader.mp_shader->Initialize(ar_DirectXClass.GetDevice(), ar_DirectXWindow.m_hwnd, as_fileLocation.c_str());

	//- add to pool after -//
	m_shaderPool.push_back(l_newshader);

	//- return 0 ai_index position of shader -//
	return m_shaderPool.size() - 1;
}

int DirectXShaderManager::DeleteShader(int ai_index)
{
	m_shaderPool[ai_index].mi_usedByCount--;
	return -1;
}

DirectXShader* DirectXShaderManager::GetShader(int ai_index)
{
	return m_shaderPool[ai_index].mp_shader;
}

void DirectXShaderManager::RemoveUnusedShader()
{
	int li_shaderPoolSize = m_shaderPool.size();
	for (int li_i(0); li_i < m_shaderPool.size(); ++li_i)
	{
		if (m_shaderPool[li_i].mi_usedByCount < 1)
		{
			delete m_shaderPool[li_i].mp_shader;
			m_shaderPool[li_i].mp_shader = nullptr;
			m_shaderPool.erase(m_shaderPool.begin() + li_i);
		}
	}
}