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
	if (mi_Texture == -1) { mi_Texture = Graphics::getInstance()->LoadTexture(TextureLocation); }
	if (mi_Shader == -1) { mi_Shader = Graphics::getInstance()->LoadShader(ShaderLocation); }
	if (mi_ID == -1) { mi_ID = Graphics::getInstance()->AddObjectToApiRenderILoop(this); }
}

void SpriteRenderer::RemoveTextureShader()
{
	//- If Texture/Shader/Object isnt instantiated then dont try and remove it -//
	if (mi_Texture != -1) { mi_Texture = Graphics::getInstance()->RemoveTexture(mi_Texture); }
	if (mi_Shader != -1) { mi_Shader = Graphics::getInstance()->RemoveShader(mi_Shader); }
	if (mi_ID != -1) { mi_ID = Graphics::getInstance()->RemoveObjectFromRenderLoop(mi_ID); }
}

void SpriteRenderer::ImGUIUpdate()
{
	std::string t1 = "Enter Texture Location";
	std::string t2;

	std::string t3 = "Enter Shader Location";
	std::string t4;

	ImGui::InputText(t1.c_str(), t2, 128);
	ImGui::InputText(t3.c_str(), t4, 128);

	if (ImGui::Button("Update Texture Shader"))
	{
		std::string s(t4);
		std::wstring ws;
		ws.assign(s.begin(), s.end());

		RemoveTextureShader();
		Initialze(std::string(t2), ws);

		m_TextureLocation = std::string(t2);
		m_ShaderLocation = std::string(t4);
	}
}

void SpriteRenderer::OnDestroy()
{
	RemoveTextureShader();
}

void SpriteRenderer::SceneLoad(json* componentJSON)
{
	(*componentJSON)["TextureLocation"] = m_TextureLocation;
	(*componentJSON)["ShaderLocation"] = m_ShaderLocation;
}
