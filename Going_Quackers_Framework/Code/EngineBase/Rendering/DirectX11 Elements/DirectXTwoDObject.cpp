#include "DirectXTwoDObject.h"
#include "../../Data Structures/Vertex.h"
#include <vector>

DirectXTwoDObject::DirectXTwoDObject()
{
	mp_device = nullptr;
	mp_deviceContext = nullptr;
	mp_indexBuffer = nullptr;
	mp_vertexBuffer = nullptr;
}

DirectXTwoDObject::~DirectXTwoDObject()
{
	// Release the index buffer.
	if (mp_indexBuffer)
	{
		mp_indexBuffer->Release();
		mp_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (mp_vertexBuffer)
	{
		mp_vertexBuffer->Release();
		mp_vertexBuffer = 0;
	}
}

bool DirectXTwoDObject::Initialize(ID3D11Device* ap_device, ID3D11DeviceContext* ap_deviceContext, RenderObjectType shape)
{
	// Initialize the vertex and index buffers.
	if (!InitializeBuffers(ap_device, shape))
	{
		return false;
	}
	mp_device = ap_device;
	mp_deviceContext = ap_deviceContext;
	return true;
}

bool DirectXTwoDObject::InitializeBuffers(ID3D11Device* ap_device, RenderObjectType shape)
{
	D3D11_BUFFER_DESC l_vertexBufferDesc, l_indexBufferDesc;
	D3D11_SUBRESOURCE_DATA l_vertexData, l_indexData;
	HRESULT l_result;

	std::vector<Vertex2D> l_vertices;

	switch ((int)shape)
	{
	case (int)RenderObjectType::SQUARE:
		l_vertices =
		{
			Vertex2D(-0.5f, 0.5f, 0.0f, 0.0f), Vertex2D(0.5f, 0.5f, 1.0f, 0.0f), //Top Right
			Vertex2D(-0.5f, -0.5f, 0.0f, 1.0f), Vertex2D(0.5f, -0.5f, 1.0f, 1.0f), //Bottom right
		};
		break;
	case (int)RenderObjectType::LINE:
		l_vertices =
		{
			Vertex2D(0.0f, 0.05f, 0.0f, 0.0f), Vertex2D(1.0f, 0.05f, 1.0f, 0.0f), //Top Right
			Vertex2D(0.0f, -0.05f, 0.0f, 1.0f), Vertex2D(1.0f, -0.05f, 1.0f, 1.0f), //Bottom right
		};
		break;
	}

	std::vector<DWORD> l_indices =
	{
		0, 1, 2,
		2, 1, 3,
	};

	// Set up the description of the static vertex buffer.
	l_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	l_vertexBufferDesc.ByteWidth = sizeof(Vertex2D) * 4; //4 = l_vertices Count
	l_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	l_vertexBufferDesc.CPUAccessFlags = 0;
	l_vertexBufferDesc.MiscFlags = 0;
	l_vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	l_vertexData.pSysMem = l_vertices.data();
	l_vertexData.SysMemPitch = 0;
	l_vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	l_result = ap_device->CreateBuffer(&l_vertexBufferDesc, &l_vertexData, &mp_vertexBuffer);
	if (FAILED(l_result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	l_indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	l_indexBufferDesc.ByteWidth = sizeof(unsigned long) * 6; //6 = l_indices Count
	l_indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	l_indexBufferDesc.CPUAccessFlags = 0;
	l_indexBufferDesc.MiscFlags = 0;
	l_indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	l_indexData.pSysMem = l_indices.data();
	l_indexData.SysMemPitch = 0;
	l_indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	l_result = ap_device->CreateBuffer(&l_indexBufferDesc, &l_indexData, &mp_indexBuffer);
	if (FAILED(l_result))
	{
		return false;
	}

	return true;
}

void DirectXTwoDObject::Render()
{
	RenderBuffers(mp_deviceContext);
}

void DirectXTwoDObject::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int li_stride;
	unsigned int li_offset;
	//- Set vertex buffer li_stride and li_offset -//
	li_stride = sizeof(Vertex2D);
	li_offset = 0;
	//- Set the vertex buffer to active in the input assembler so it can be rendered -//
	deviceContext->IASetVertexBuffers(0, 1, &mp_vertexBuffer, &li_stride, &li_offset);
	//- Set the index buffer to active in the input assembler so it can be rendered. -//
	deviceContext->IASetIndexBuffer(mp_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//- Set the type of primitive that should be rendered from this vertex buffer -//
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}