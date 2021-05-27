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

	bool Initialize(ID3D11Device* ap_device, HWND a_hwnd, std::wstring as_fileLocation);
	bool Render(ID3D11DeviceContext* ap_deviceContext, int ai_indexCount , DirectX::XMMATRIX a_worldMatrix, DirectX::XMMATRIX a_rotationMatrix, DirectX::XMMATRIX a_projectionMatrix, ID3D11ShaderResourceView* ap_texture);

private:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	bool InitializeShader(ID3D11Device* ap_device, HWND a_hwnd, WCHAR* ap_vsFileName, WCHAR* ap_psFileName);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* ap_errorMessage, HWND a_hwnd, WCHAR* ap_shaderFileName);

	bool SetShaderParameters(ID3D11DeviceContext* ap_deviceContext, DirectX::XMMATRIX a_worldMatrix, DirectX::XMMATRIX a_viewMatrix, DirectX::XMMATRIX a_projectionMatrix, ID3D11ShaderResourceView* ap_texture);
	void RenderShader(ID3D11DeviceContext* ap_deviceContext, int ai_indexCount);

	ID3D11VertexShader* mp_vertexShader;
	ID3D11PixelShader* mp_pixelShader;
	ID3D11InputLayout* mp_layout;
	ID3D11Buffer* mp_matrixBuffer;
	ID3D11SamplerState* mp_sampleState;

};

#endif // !_SHADER_H_