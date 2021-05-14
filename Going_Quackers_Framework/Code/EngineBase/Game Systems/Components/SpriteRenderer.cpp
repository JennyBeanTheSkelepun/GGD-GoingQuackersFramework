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
	Graphics* temp = Graphics::getInstance();

	//- If Texture/Shader/Object is instantiated then dont try and remake it  -//
	if (mi_Texture == -1) { mi_Texture = temp->LoadTexture(TextureLocation); }
	if (mi_Shader == -1) { mi_Shader = temp->LoadShader(ShaderLocation); }
	if (mi_ID == -1) { mi_ID = temp->AddObjectToApiRenderILoop(this); }
}

void SpriteRenderer::RemoveTextureShader()
{
	Graphics* temp = Graphics::getInstance();

	////- If Texture/Shader/Object isnt instantiated then dont try and remove it -//
	//if (mi_Texture != -1) { mi_Texture = temp->RemoveTexture(mi_Texture); }
	//if (mi_Shader != -1) { mi_Shader = temp->RemoveShader(mi_Shader); }
	//if (mi_ID != -1) { mi_ID = temp->RemoveObjectFromRenderLoop(mi_ID); }
}

void SpriteRenderer::ImGUIUpdate()
{
	ImGui::Text("Test");
}
