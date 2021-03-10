#ifndef _SPRITERENDERER_H_
#define _SPRITERENDERER_H_

#include "Component.h"
#include "../../Rendering/Graphics.h"

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

	void Initialze(std::string TextureLocation, std::string ShaderLocation);

	inline int GetTexture() { return mi_Texture; };
	inline int GetShader() { return mi_Shader; };

	void RemoveTextureShader();

//private:

	int ModelType = ModelType::TWO_DIMENSIONS;
	int mi_Texture, mi_Shader, mi_ID;
};

#endif