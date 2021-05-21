#include "SpriteRenderer.h"
#include "../../Data Structures/Vertex.h"
#include "../GameObject.h"
#include "Sprite.h"
#include "../../Rendering/Graphics.h"

#include <vector>

SpriteRenderer::SpriteRenderer(GameObject* owner) : Component(owner, ComponentTypes::SPRITERENDERER)
{
	mi_Texture = 0;
	mi_Shader = 0;
	//- Add To Rendering Loop -//
	mi_ID = Graphics::getInstance()->AddObjectToApiRenderILoop(this);

	TextureSelectionInput = new char[100] { "stone.tga" };
	ShaderSelectionInput = new char[100] { "Code/EngineBase/Rendering/Shaders/TextureSimple.fx" };
}

SpriteRenderer::~SpriteRenderer()
{
	RemoveTextureShader();
}

void SpriteRenderer::Initialze(std::string TextureLocation, std::wstring ShaderLocation)
{
	m_TextureLocation = TextureLocation;
	m_ShaderLocation.assign(ShaderLocation.begin(), ShaderLocation.end());
	Graphics* temp = Graphics::getInstance();

	//- If Texture/Shader/Object is instantiated then dont try and remake it  -//
	if (mi_Texture == -1) 
	{ 
		mi_Texture = Graphics::getInstance()->LoadTexture(TextureLocation);
	}

	if (mi_Shader == -1) 
	{ 
		mi_Shader = Graphics::getInstance()->LoadShader(ShaderLocation); 
	}

	if (mi_ID == -1) 
	{ 
		mi_ID = Graphics::getInstance()->AddObjectToApiRenderILoop(this); 
	}
}

void SpriteRenderer::RemoveTextureShader()
{
	//- If Texture/Shader/Object isnt instantiated then dont try and remove it -//
	if (mi_Texture != -1) 
	{
		mi_Texture = Graphics::getInstance()->RemoveTexture(mi_Texture); 
	}

	if (mi_Shader != -1) 
	{
		mi_Shader = Graphics::getInstance()->RemoveShader(mi_Shader); 
	}

	if (mi_ID != -1) 
	{
		mi_ID = Graphics::getInstance()->RemoveObjectFromRenderLoop(mi_ID); 
	}
}

void SpriteRenderer::ImGUIUpdate()
{
	std::string t1 = "Enter Texture Location";
	std::string t3 = "Enter Shader Location";

	ImGui::InputText(t1.c_str(), TextureSelectionInput, 128);
	ImGui::InputText(t3.c_str(), ShaderSelectionInput, 128);

	if (ImGui::Button("Update Texture Shader"))
	{
		std::string s(ShaderSelectionInput);
		std::wstring ws;
		ws.assign(s.begin(), s.end());

		RemoveTextureShader();
		Initialze(std::string(TextureSelectionInput), ws);

		m_TextureLocation = std::string(TextureSelectionInput);
		m_ShaderLocation = std::string(ShaderSelectionInput);
	}
}

void SpriteRenderer::OnDestroy()
{
	RemoveTextureShader();
}

void SpriteRenderer::SceneLoad(json* componentJSON)
{
	m_TextureLocation = (*componentJSON)["TextureLocation"];
	m_ShaderLocation = (*componentJSON)["ShaderLocation"];
}

json* SpriteRenderer::SceneSave()
{
	json* returnObj = new json({
		{"TextureLocation", m_TextureLocation},
		{"ShaderLocation", m_ShaderLocation}
	});

	return returnObj;
}