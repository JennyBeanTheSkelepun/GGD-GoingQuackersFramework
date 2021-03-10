#pragma once

#include "DirectXClass.h"

class DirectX2DObject
{
public:
	DirectX2DObject();
	~DirectX2DObject();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Render();

private:
	ID3D11Device* mp_device;
	ID3D11DeviceContext* mp_deviceContext;

	ID3D11Buffer* mp_vertexBuffer;
	ID3D11Buffer* mp_indexBuffer;

	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
};

