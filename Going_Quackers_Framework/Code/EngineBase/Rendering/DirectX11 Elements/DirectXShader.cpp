#include "DirectXShader.h"

DirectXShader::DirectXShader()
{
	mp_vertexShader = nullptr;
	mp_pixelShader = nullptr;
	mp_layout = nullptr;
	mp_matrixBuffer = nullptr;
	mp_sampleState = nullptr;
}

DirectXShader::~DirectXShader()
{
	ShutdownShader();
}

bool DirectXShader::Initialize(ID3D11Device* ap_device, HWND a_hwnd, std::wstring as_fileLocation)
{
	// Initialize the vertex and pixel shaders.
	return !InitializeShader(ap_device, a_hwnd, (WCHAR*)(as_fileLocation.c_str()), (WCHAR*)(as_fileLocation.c_str()));
}

bool DirectXShader::Render(ID3D11DeviceContext* ap_deviceContext, int ai_indexCount, DirectX::XMMATRIX a_worldMatrix, DirectX::XMMATRIX a_viewMatrix, DirectX::XMMATRIX a_projectionMatrix, ID3D11ShaderResourceView* ap_texture = nullptr)
{
	// Set the shader parameters that it will use for rendering.
	if (!SetShaderParameters(ap_deviceContext, a_worldMatrix, a_viewMatrix, a_projectionMatrix, ap_texture))
		return false;

	// Now render the prepared buffers with the shader.
	RenderShader(ap_deviceContext, ai_indexCount);
	return true;
}

bool DirectXShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT lp_result;
	ID3D10Blob* lp_errorMessage;
	ID3D10Blob* lp_vertexShaderBuffer;
	ID3D10Blob* lp_pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC l_polygonLayout[3];
	unsigned int li_numElements;
	D3D11_BUFFER_DESC l_matrixBufferDesc;
	D3D11_SAMPLER_DESC l_samplerDesc;

	// Initialize the pointers this function will use to null.
	lp_errorMessage = nullptr;
	lp_vertexShaderBuffer = nullptr;
	lp_pixelShaderBuffer = nullptr;
	
	// Compile the vertex shader code.
	lp_result = D3DCompileFromFile(vsFilename, NULL, NULL, "GQFVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &lp_vertexShaderBuffer, &lp_errorMessage);
	if (FAILED(lp_result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (lp_errorMessage)
		{
			OutputShaderErrorMessage(lp_errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	lp_result = D3DCompileFromFile(psFilename, NULL, NULL, "GQFPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &lp_pixelShaderBuffer, &lp_errorMessage);
	if (FAILED(lp_result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (lp_errorMessage)
		{
			OutputShaderErrorMessage(lp_errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	
	// Create the vertex shader from the buffer.
	lp_result = device->CreateVertexShader(lp_vertexShaderBuffer->GetBufferPointer(), lp_vertexShaderBuffer->GetBufferSize(), NULL, &mp_vertexShader);
	if (FAILED(lp_result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	lp_result = device->CreatePixelShader(lp_pixelShaderBuffer->GetBufferPointer(), lp_pixelShaderBuffer->GetBufferSize(), NULL, &mp_pixelShader);
	if (FAILED(lp_result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	l_polygonLayout[0].SemanticName = "POSITION";
	l_polygonLayout[0].SemanticIndex = 0;
	l_polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	l_polygonLayout[0].InputSlot = 0;
	l_polygonLayout[0].AlignedByteOffset = 0;
	l_polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	l_polygonLayout[0].InstanceDataStepRate = 0;

	l_polygonLayout[1].SemanticName = "TEXCOORD";
	l_polygonLayout[1].SemanticIndex = 0;
	l_polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	l_polygonLayout[1].InputSlot = 0;
	l_polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	l_polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	l_polygonLayout[1].InstanceDataStepRate = 0;
	
	l_polygonLayout[2].SemanticName = "COLOR";
	l_polygonLayout[2].SemanticIndex = 0;
	l_polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	l_polygonLayout[2].InputSlot = 0;
	l_polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	l_polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	l_polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	li_numElements = sizeof(l_polygonLayout) / sizeof(l_polygonLayout[0]);

	// Create the vertex input layout.
	lp_result = device->CreateInputLayout(l_polygonLayout, li_numElements, lp_vertexShaderBuffer->GetBufferPointer(), lp_vertexShaderBuffer->GetBufferSize(), &mp_layout);
	if (FAILED(lp_result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	lp_vertexShaderBuffer->Release();
	lp_vertexShaderBuffer = 0;

	lp_pixelShaderBuffer->Release();
	lp_pixelShaderBuffer = 0;
	
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	l_matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	l_matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	l_matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	l_matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	l_matrixBufferDesc.MiscFlags = 0;
	l_matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	lp_result = device->CreateBuffer(&l_matrixBufferDesc, NULL, &mp_matrixBuffer);
	if (FAILED(lp_result))
	{
		return false;
	}

	// Create a ap_texture sampler state description.
	l_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	l_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	l_samplerDesc.MipLODBias = 0.0f;
	l_samplerDesc.MaxAnisotropy = 1;
	l_samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	l_samplerDesc.BorderColor[0] = 0;
	l_samplerDesc.BorderColor[1] = 0;
	l_samplerDesc.BorderColor[2] = 0;
	l_samplerDesc.BorderColor[3] = 0;
	l_samplerDesc.MinLOD = 0;
	l_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the ap_texture sampler state.
	lp_result = device->CreateSamplerState(&l_samplerDesc, &mp_sampleState);
	if (FAILED(lp_result))
	{
		return false;
	}

	return true;
}

void DirectXShader::ShutdownShader()
{
	if (mp_sampleState)
	{
		mp_sampleState->Release();
		mp_sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (mp_matrixBuffer)
	{
		mp_matrixBuffer->Release();
		mp_matrixBuffer = 0;
	}

	// Release the layout.
	if (mp_layout)
	{
		mp_layout->Release();
		mp_layout = 0;
	}

	// Release the pixel shader.
	if (mp_pixelShader)
	{
		mp_pixelShader->Release();
		mp_pixelShader = 0;
	}

	// Release the vertex shader.
	if (mp_vertexShader)
	{
		mp_vertexShader->Release();
		mp_vertexShader = 0;
	}
}

void DirectXShader::OutputShaderErrorMessage(ID3D10Blob* ap_errorMessage, HWND a_hwnd, WCHAR* ap_shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(ap_errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = ap_errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	ap_errorMessage->Release();
	ap_errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(a_hwnd, L"Error compiling shader.  Check shader-error.txt for message.", ap_shaderFilename, MB_OK);
}

bool DirectXShader::SetShaderParameters(ID3D11DeviceContext* ap_deviceContext, DirectX::XMMATRIX a_worldMatrix, 
	DirectX::XMMATRIX a_viewMatrix, DirectX::XMMATRIX a_projectionMatrix, ID3D11ShaderResourceView* ap_texture)
{
	HRESULT l_result;
	D3D11_MAPPED_SUBRESOURCE l_mappedResource;
	MatrixBuffer* lp_dataPtr;
	unsigned int li_bufferNumber;
	
	// Transpose the matrices to prepare them for the shader.
	a_worldMatrix = DirectX::XMMatrixTranspose(a_worldMatrix);
	a_viewMatrix = DirectX::XMMatrixTranspose(a_viewMatrix);
	a_projectionMatrix = DirectX::XMMatrixTranspose(a_projectionMatrix);

	// Lock the constant buffer so it can be written to.
	l_result = ap_deviceContext->Map(mp_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &l_mappedResource);
	if (FAILED(l_result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	lp_dataPtr = (MatrixBuffer*)l_mappedResource.pData;

	// Copy the matrices into the constant buffer.
	lp_dataPtr->world = a_worldMatrix;
	lp_dataPtr->view = a_viewMatrix;
	lp_dataPtr->projection = a_projectionMatrix;
	lp_dataPtr->color = DirectX::XMFLOAT3(255, 0, 0);

	// Unlock the constant buffer.
	ap_deviceContext->Unmap(mp_matrixBuffer, 0);
	
	// Set the position of the constant buffer in the vertex shader.
	li_bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	//ap_deviceContext->UpdateSubresource(mp_matrixBuffer, 0, NULL, lp_dataPtr, 0, 0);
	ap_deviceContext->VSSetConstantBuffers(li_bufferNumber, 1, &mp_matrixBuffer);

	if (ap_texture != nullptr)
		ap_deviceContext->PSSetShaderResources(0, 1, &ap_texture);

	return true;
}

void DirectXShader::RenderShader(ID3D11DeviceContext* ap_deviceContext, int ai_indexCount)
{
	// Set the vertex input layout.
	ap_deviceContext->IASetInputLayout(mp_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	ap_deviceContext->VSSetShader(mp_vertexShader, NULL, 0);
	ap_deviceContext->PSSetShader(mp_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	ap_deviceContext->PSSetSamplers(0, 1, &mp_sampleState);

	// Render the triangle.
	ap_deviceContext->DrawIndexed(ai_indexCount, 0, 0);
}
