#ifndef _DIRECT_2D_OBJECT_H_
#define _DIRECT_2D_OBJECT_H_

#include "DirectXClass.h"

class DirectXTwoDObject
{
public:
	DirectXTwoDObject();
	~DirectXTwoDObject();

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

#endif /* _DIRECT_2D_OBJECT_H_ */