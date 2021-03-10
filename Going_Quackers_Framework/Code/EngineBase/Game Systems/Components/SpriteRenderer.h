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

	//void Initialize() override;
	//void Render() override;

	//bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void Initialze(std::string TextureLocation, std::string ShaderLocation);

	inline int GetTexture() { return mi_Texture; };
	inline int GetShader() { return mi_Shader; };

	void RemoveTextureShader();

//private:

	int ModelType = ModelType::TWO_DIMENSIONS;
	
	//Texture Index
	int mi_Texture, mi_Shader, mi_ID;

	//A reference to the sprite component that SHOULD be attached
	//Sprite* mp_spriteReference;

	//ID3D11Device* mp_device;
	//ID3D11DeviceContext* mp_deviceContext;

	//ID3D11Buffer* mp_vertexBuffer;
	//ID3D11Buffer* mp_indexBuffer;
	//int m_vertexCount;
	//int m_indexCount;

	//bool InitializeBuffers(ID3D11Device* device);
	//void RenderBuffers(ID3D11DeviceContext* deviceContext);

	//void ShutdownBuffers();
};

#endif