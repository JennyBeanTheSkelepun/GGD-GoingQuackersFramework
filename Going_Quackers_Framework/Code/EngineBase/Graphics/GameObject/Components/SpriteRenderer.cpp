#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(GameObject* owner) : Component(owner)
{
	mp_vertexBuffer = 0;
	mp_indexBuffer = 0;
	mp_texture = 0;
	m_vertexCount = 0;
	m_indexCount = 0;
}

SpriteRenderer::~SpriteRenderer()
{
	ShutdownBuffers();
	ReleaseTexture();
}

void SpriteRenderer::Initialize()
{

}

void SpriteRenderer::Render()
{
	RenderBuffers(mp_deviceContext);
}

bool SpriteRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileName)
{
	bool result;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFileName);
	if (!result)
	{
		return false;
	}

	mp_deviceContext = deviceContext;

	return true;
}

bool SpriteRenderer::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 4;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top Left
	vertices[0].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);  // Top Right
	vertices[1].texture = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[2].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[2].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[3].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 1;  // Top Left
	indices[1] = 2;	 // Top Right
	indices[2] = 0;  // Bottom Left.

	indices[3] = 1;  // Top Right.
	indices[4] = 3;	 //	Bottom Right
	indices[5] = 2;  // Bottom Left

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
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
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mp_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void SpriteRenderer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &mp_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(mp_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool SpriteRenderer::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName)
{
	bool result;

	// Create the texture object.
	mp_texture = new Texture;
	if (!mp_texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = mp_texture->Initialize(device, deviceContext, fileName);
	if (!result)
	{
		return false;
	}

	return true;
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

void SpriteRenderer::ReleaseTexture()
{
	// Release the texture object.
	if (mp_texture)
	{
		mp_texture->Shutdown();
		delete mp_texture;
		mp_texture = 0;
	}

	return;
}
