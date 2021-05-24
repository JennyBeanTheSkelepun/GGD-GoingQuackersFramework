#ifndef _SPRITERENDERER_H_
#define _SPRITERENDERER_H_

#include "Component.h"
#include <string>

class Graphics;

enum ModelType
{
	TWO_DIMENSIONS = 0,
	THREE_DIMENSIONS = 1
};

class SpriteRenderer : public Component
{
public:
	SpriteRenderer(GameObject* owner);
	~SpriteRenderer();

	void InitialzeTextureShader(std::string TextureLocation, std::wstring ShaderLocation);

	inline int GetTexture() { return mi_Texture; };
	inline int GetShader() { return mi_Shader; };

	void RemoveTextureShader();

	//- Basic Loops -//
	void OnDestroy() override;
	void Update() override;

	//- ImGui UpdateLoop -//
	void ImGUIUpdate() override;
	
	//- Scene Save and Load -//
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

private:

	int ModelType = ModelType::TWO_DIMENSIONS;
	int mi_Texture, mi_Shader, mi_ID;

	std::string m_TextureLocation, m_ShaderLocation;

	char* TextureSelectionInput;
	char* ShaderSelectionInput;
};

#endif