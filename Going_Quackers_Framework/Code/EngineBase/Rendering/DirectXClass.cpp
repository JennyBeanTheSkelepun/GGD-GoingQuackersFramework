#include "DirectXClass.h"

DirectXClass* DirectXClass::SingletonInstance = 0;
DirectXClass* DirectXClass::getInstance()
{
	if (SingletonInstance == 0)
		SingletonInstance = new DirectXClass();
	return SingletonInstance;
}

DirectXClass::DirectXClass()
{
	mp_swapChain = 0;
	mp_device = 0;
	mp_deviceContext = 0;
	mp_renderTargetView = 0;
	mp_depthStencilState = 0;
	mp_depthStencilView = 0;
	mp_rasterState = 0;
}

DirectXClass::~DirectXClass()
{
	//// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	//if (mp_swapChain)
	//{
	//	mp_swapChain->SetFullscreenState(false, NULL);
	//}

	//if (mp_rasterState)
	//{
	//	mp_rasterState->Release();
	//	delete mp_rasterState;
	//	//mp_rasterState = nullptr;
	//}

	//if (mp_depthStencilView)
	//{
	//	mp_depthStencilView->Release();
	//	delete mp_depthStencilView;
	//	mp_depthStencilView = nullptr;
	//}

	//if (mp_depthStencilState)
	//{
	//	mp_depthStencilState->Release();
	//	delete mp_depthStencilState;
	//	mp_depthStencilState = nullptr;
	//}

	//if (mp_depthStencilBuffer)
	//{
	//	mp_depthStencilBuffer->Release();
	//	delete mp_depthStencilBuffer;
	//	mp_depthStencilBuffer = nullptr;
	//}

	//if (mp_renderTargetView)
	//{
	//	mp_renderTargetView->Release();
	//	delete mp_renderTargetView;
	//	mp_renderTargetView = nullptr;
	//}

	//if (mp_deviceContext)
	//{
	//	mp_deviceContext->Release();
	//	delete mp_deviceContext;
	//	mp_deviceContext = nullptr;
	//}

	//if (mp_device)
	//{
	//	mp_device->Release();
	//	delete mp_device;
	//	mp_device = nullptr;
	//}

	//if (mp_swapChain)
	//{
	//	mp_swapChain->Release();
	//	delete mp_swapChain;
	//	mp_swapChain = nullptr;
	//}

	//if (mp_renderTextureTargetTexture)
	//{
	//	mp_renderTextureTargetTexture->Release();
	//	delete mp_renderTextureTargetTexture;
	//	mp_renderTextureTargetTexture = nullptr;
	//}

	//if (mp_renderTextureRenderTargetView)
	//{
	//	mp_renderTextureRenderTargetView->Release();
	//	delete mp_renderTextureRenderTargetView;
	//	mp_renderTextureRenderTargetView = nullptr;
	//}

	//if (mp_renderTextureResourceView)
	//{
	//	mp_renderTextureResourceView->Release();
	//	delete mp_renderTextureResourceView;
	//	mp_renderTextureResourceView = nullptr;
	//}

}

bool DirectXClass::ResizeRenderBuffers(HWND hwnd)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	// Set up the description of the depth buffer.
	int ai_screenWidth = clientRect.right;
	int ai_screenHeight = clientRect.bottom;




	return true;
}

bool DirectXClass::Initalize(int ai_screenWidth, int ai_screenHeight, bool ab_vsync,
	HWND hwnd, bool ab_fullScreen, float af_screenDepth, float af_screenNear)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	// Set up the description of the depth buffer.
	ai_screenWidth = clientRect.right;
	ai_screenHeight = clientRect.bottom;

	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// Store the vsync setting.
	mb_vsync_enabled = ab_vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)ai_screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)ai_screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	mi_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, mc_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = ai_screenWidth;
	swapChainDesc.BufferDesc.Height = ai_screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (mb_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (ab_fullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &mp_swapChain, &mp_device, NULL, &mp_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = mp_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = mp_device->CreateRenderTargetView(backBufferPtr, NULL, &mp_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetVeiwDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = ai_screenWidth;
	textureDesc.Height = ai_screenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	mp_device->CreateTexture2D(&textureDesc, NULL, &mp_renderTextureTargetTexture);

	renderTargetVeiwDesc.Format = textureDesc.Format;
	renderTargetVeiwDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetVeiwDesc.Texture2D.MipSlice = 0;

	mp_device->CreateRenderTargetView(mp_renderTextureTargetTexture, &renderTargetVeiwDesc, &mp_renderTextureRenderTargetView);

	//resourchView
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	mp_device->CreateShaderResourceView(mp_renderTextureTargetTexture, &shaderResourceViewDesc, &mp_renderTextureResourceView);

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	
	//depthBufferDesc.Width = 1280;
	//depthBufferDesc.Height = 780;

	depthBufferDesc.Width = ai_screenWidth;
	depthBufferDesc.Height = ai_screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = mp_device->CreateTexture2D(&depthBufferDesc, NULL, &mp_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = mp_device->CreateDepthStencilState(&depthStencilDesc, &mp_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	mp_deviceContext->OMSetDepthStencilState(mp_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = mp_device->CreateDepthStencilView(mp_depthStencilBuffer, &depthStencilViewDesc, &mp_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mp_deviceContext->OMSetRenderTargets(1, &mp_renderTextureRenderTargetView, mp_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = mp_device->CreateRasterizerState(&rasterDesc, &mp_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	mp_deviceContext->RSSetState(mp_rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)ai_screenWidth;
	viewport.Height = (float)ai_screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	mp_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)ai_screenWidth / (float)ai_screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, af_screenNear, af_screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = DirectX::XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)ai_screenWidth, (float)ai_screenHeight, af_screenNear, af_screenDepth);

	return true;
}

void DirectXClass::BeginGameScene(float red, float green, float blue, float alpha)
{
	float color[4]{red, green, blue, alpha};

	// Clear the back buffer.
	mp_deviceContext->ClearRenderTargetView(mp_renderTargetView, color);

	// Clear the depth buffer.
	mp_deviceContext->ClearDepthStencilView(mp_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Clear Render Texture
	mp_deviceContext->ClearRenderTargetView(mp_renderTextureRenderTargetView, color);
}

void DirectXClass::BeginEditorScene(float red, float green, float blue, float alpha)
{
	float color[4]{ red, green, blue, alpha };

	// Clear the back buffer.
	mp_deviceContext->ClearRenderTargetView(mp_renderTargetView, color);

	// Clear the depth buffer.
	mp_deviceContext->ClearDepthStencilView(mp_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectXClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (mb_vsync_enabled)
	{
		// Lock to screen refresh rate.
		mp_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		mp_swapChain->Present(0, 0);
	}
}

ID3D11Device* DirectXClass::GetDevice()
{
	return mp_device;
}

ID3D11DeviceContext* DirectXClass::GetDeviceContext()
{
	return mp_deviceContext;
}

void DirectXClass::GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}

void DirectXClass::GetWorldMatrix(DirectX::XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
}

void DirectXClass::GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
}

void DirectXClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, mc_videoCardDescription);
	memory = mi_videoCardMemory;
}
