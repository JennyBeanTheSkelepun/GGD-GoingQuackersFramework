#ifndef _SPRITERENDERER_H_
#define _SPRITERENDERER_H_

#include "Component.h"
#include "../../Data Structures/Texture2D.h"

#include <d3d11.h>
#include <directxmath.h>

class Sprite;

class SpriteRenderer : public Component
{
public:
	SpriteRenderer(GameObject* owner);
	~SpriteRenderer();

	void Initialize() override;
	void Render() override;

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

private:
	//A reference to the sprite component that SHOULD be attached
	Sprite* mp_spriteReference;

	ID3D11Device* mp_device;
	ID3D11DeviceContext* mp_deviceContext;

	ID3D11Buffer* mp_vertexBuffer;
	ID3D11Buffer* mp_indexBuffer;
	int m_vertexCount;
	int m_indexCount;

	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	void ShutdownBuffers();
};

#endif