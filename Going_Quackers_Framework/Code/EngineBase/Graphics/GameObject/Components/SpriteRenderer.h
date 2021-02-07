#pragma once
#include "Component.h"

class Texture;

class SpriteRenderer : public Component
{
public:
	SpriteRenderer(GameObject* parent);
	~SpriteRenderer();

	void Initialize() override;
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileName);

	void Render(ID3D11DeviceContext* deviceContext) override;

	int GetIndexCount() { return m_indexCount; }
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName);
	void ReleaseTexture();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
};