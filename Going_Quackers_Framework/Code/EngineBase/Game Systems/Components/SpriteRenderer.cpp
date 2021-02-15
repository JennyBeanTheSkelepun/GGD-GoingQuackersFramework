#include "SpriteRenderer.h"
#include "../../Data Structures/Vertex.h"
#include "../GameObject.h"
#include "Sprite.h"

#include <vector>

SpriteRenderer::SpriteRenderer(GameObject* owner) : Component(owner)
{
	mp_vertexBuffer = 0;
	mp_indexBuffer = 0;
	m_vertexCount = 0;
	m_indexCount = 0;
}

SpriteRenderer::~SpriteRenderer()
{
	ShutdownBuffers();;
}

void SpriteRenderer::Initialize()
{
	mp_spriteReference = mp_owner->GetComponent<Sprite>();
}

void SpriteRenderer::Render()
{
	RenderBuffers(mp_deviceContext);


}

bool SpriteRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{	
	Initialize();

	bool result;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	mp_device = device;
	mp_deviceContext = deviceContext;

	return true;
}

bool SpriteRenderer::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Create the vertex array.
	std::vector<Vertex2D> vertices =
	{
		Vertex2D(-1.0f, 1.0f, 0.0f, 0.0f), //Top Left
		Vertex2D(1.0f, 1.0f, 1.0f, 0.0f), //Top Right
		Vertex2D(-1.0f, -1.0f, 0.0f, 1.0f), //Bottom left
		Vertex2D(1.0f, -1.0f, 1.0f, 1.0f), //Bottom right
	};

	std::vector<DWORD> indices =
	{
		0, 1, 2,
		2, 1, 3,
	};

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex2D) * 4; //4 = vertices Count
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mp_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * 6; //6 = indices Count
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mp_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SpriteRenderer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex2D);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &mp_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(mp_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void SpriteRenderer::ShutdownBuffers()
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
