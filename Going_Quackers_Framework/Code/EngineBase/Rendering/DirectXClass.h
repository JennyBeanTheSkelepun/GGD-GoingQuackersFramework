#ifndef _DIRECT_X_CLASS_
#define _DIRECT_X_CLASS_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

class DirectXClass
{
public:
	DirectXClass();
	DirectXClass(const DirectXClass& other);
	~DirectXClass();

	bool Initalize(int ai_screenWidth, int ai_screenHeight, bool ab_vsync, HWND hwnd, bool ab_fullScreen, float af_screenDepth, float af_screenNear);

	void BeginGameScene(float red, float green, float blue, float alpha);
	void BeginEditorScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix);
	void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix);
	void GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* cardName, int& memory);


	bool mb_vsync_enabled;
	int mi_videoCardMemory;
	char mc_videoCardDescription[128];
	IDXGISwapChain* mp_swapChain;
	ID3D11Device* mp_device;
	ID3D11DeviceContext* mp_deviceContext;
	ID3D11RenderTargetView* mp_renderTargetView;
	ID3D11Texture2D* mp_depthStencilBuffer;
	ID3D11DepthStencilState* mp_depthStencilState;
	ID3D11DepthStencilView* mp_depthStencilView;
	ID3D11RasterizerState* mp_rasterState;

	ID3D11Texture2D* mp_renderTextureTargetTexture;
	ID3D11RenderTargetView* mp_renderTextureRenderTargetView;
	ID3D11ShaderResourceView* mp_renderTextureResourceView;

	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_orthoMatrix;
};

#endif // !_DIRECT_X_CLASS_
