#ifndef _SHADER_H_
#define _SHADER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

class DirectXShader
{
public:
	DirectXShader();
	~DirectXShader();

	bool Initialize(ID3D11Device* device, HWND hwnd, std::wstring fileLocation);
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount , DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX rotationMatrix, DirectX::XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

private:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	ID3D11VertexShader* mp_vertexShader;
	ID3D11PixelShader* mp_pixelShader;
	ID3D11InputLayout* mp_layout;
	ID3D11Buffer* mp_matrixBuffer;
	ID3D11SamplerState* mp_sampleState;

};

#endif // !_SHADER_H_