#ifndef _SPRITERENDERER_H_
#define _SPRITERENDERER_H_

#include "Component.h"
#include "../../Texture/TextureClass.h"

#include <d3d11.h>
#include <directxmath.h>

class SpriteRenderer : public Component
{
public:
	SpriteRenderer(GameObject* owner);
	~SpriteRenderer();

	void Initialize() override;
	void Render() override;

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void SetSprite(char* fileName);

	Texture* GetTexture() { return mp_texture; }

private:
	ID3D11Device* mp_device;
	ID3D11DeviceContext* mp_deviceContext;

	ID3D11Buffer* mp_vertexBuffer;
	ID3D11Buffer* mp_indexBuffer;
	int m_vertexCount;
	int m_indexCount;

	Texture* mp_texture;

private:
	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName);

	void ShutdownBuffers();
	void ReleaseTexture();
};

#endif