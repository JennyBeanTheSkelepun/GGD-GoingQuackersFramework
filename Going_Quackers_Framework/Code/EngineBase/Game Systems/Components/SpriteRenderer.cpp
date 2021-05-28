#include "SpriteRenderer.h"
#include "../../Rendering/Graphics.h"
#include "../GameObject.h"

#include <vector>

//- Constructors -//
SpriteRenderer::SpriteRenderer(GameObject* owner) : Component(owner, ComponentTypes::SPRITERENDERER, "Sprite Renderer")
{
	mi_Texture = 0;
	mi_Shader = 0;
	//- Add To Rendering Loop -//
	mi_ID = Graphics::getInstance()->AddObjectToApiRenderILoop(this);

	TextureSelectionInput = new char[100] { "Assets/stone.tga" };
	ShaderSelectionInput = new char[100] { "Assets/Shaders/TextureSimple.fx" };
}

//- Deconstructors -//
SpriteRenderer::~SpriteRenderer()
{
	RemoveTextureShader();
}

void SpriteRenderer::OnDestroy()
{
	this->~SpriteRenderer();
}

//- Update Function -//
void SpriteRenderer::Update()
{
}

//- ImGui -//
void SpriteRenderer::ImGUIUpdate()
{
	ImGui::InputText("Enter Texture Location" , TextureSelectionInput, 128);
	//ImGui::InputText("Enter Shader Location" , ShaderSelectionInput, 128);

	if (ImGui::Button("Update Texture Shader"))
	{
		std::string s(ShaderSelectionInput);
		std::wstring ws;
		ws.assign(s.begin(), s.end());

		RemoveTextureShader();
		InitialzeTextureShader(std::string(TextureSelectionInput), ws);

		m_TextureLocation = std::string(TextureSelectionInput);
		m_ShaderLocation = std::string(ShaderSelectionInput);
	}
}

//- Scene Save / Load -//
void SpriteRenderer::SceneLoad(json* componentJSON)
{
	m_TextureLocation = (*componentJSON)["TextureLocation"];
	m_ShaderLocation = (*componentJSON)["ShaderLocation"];


	if (m_TextureLocation != "" && m_ShaderLocation != "") {
		std::string s(m_ShaderLocation);
		std::wstring ws;
		ws.assign(s.begin(), s.end());

		RemoveTextureShader();
		InitialzeTextureShader(std::string(m_TextureLocation), ws);

		// TODO: Weird crash when changing scene when more than one texture is loaded?
	}
	else {
		Debug::getInstance()->LogError("Scene Load Error: Sprite Renderer for object " + mp_owner->GetName() + " is missing either a Texture or Shader location. \n" +
		"To prevent this error in future, please remember to update Texture/Shader in component settings and save the scene after. - Jen");
	}
}

json* SpriteRenderer::SceneSave()
{
	json* returnObj = new json({
		{"TextureLocation", m_TextureLocation},
		{"ShaderLocation", m_ShaderLocation}
		});

	return returnObj;
}

//- Custom Functions -//
void SpriteRenderer::InitialzeTextureShader(std::string TextureLocation, std::wstring ShaderLocation)
{
	m_TextureLocation = TextureLocation;
	m_ShaderLocation.assign(ShaderLocation.begin(), ShaderLocation.end());
	Graphics* temp = Graphics::getInstance();

	//- If Texture/mp_shader/Object is instantiated then dont try and remake it  -//
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
	//- If Texture/mp_shader/Object isnt instantiated then dont try and remove it -//
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