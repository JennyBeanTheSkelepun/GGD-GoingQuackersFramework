#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <d3d11.h>
#include <directxmath.h>

#include "../Texture/TextureClass.h"

class GameObject
{
public:
	GameObject();
	GameObject(const GameObject& other);
	~GameObject();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileNames);

	void Update();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	
	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char*);
	void ReleaseTexture();

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
};

#endif // !_GAMEOBJECT_H_