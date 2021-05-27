#include "DirectXClass.h"

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
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (mp_swapChain)
	{
		mp_swapChain->SetFullscreenState(false, NULL);
	}

	if (mp_rasterState)
	{
		mp_rasterState->Release();
		delete mp_rasterState;
		mp_rasterState = nullptr;
	}

	if (mp_depthStencilView)
	{
		mp_depthStencilView->Release();
		delete mp_depthStencilView;
		mp_depthStencilView = nullptr;
	}

	if (mp_depthStencilState)
	{
		mp_depthStencilState->Release();
		delete mp_depthStencilState;
		mp_depthStencilState = nullptr;
	}

	if (mp_depthStencilBuffer)
	{
		mp_depthStencilBuffer->Release();
		delete mp_depthStencilBuffer;
		mp_depthStencilBuffer = nullptr;
	}

	if (mp_renderTargetView)
	{
		mp_renderTargetView->Release();
		delete mp_renderTargetView;
		mp_renderTargetView = nullptr;
	}

	if (mp_deviceContext)
	{
		mp_deviceContext->Release();
		delete mp_deviceContext;
		mp_deviceContext = nullptr;
	}

	if (mp_device)
	{
		mp_device->Release();
		delete mp_device;
		mp_device = nullptr;
	}

	if (mp_swapChain)
	{
		mp_swapChain->Release();
		delete mp_swapChain;
		mp_swapChain = nullptr;
	}

	if (mp_renderTextureTargetTexture)
	{
		mp_renderTextureTargetTexture->Release();
		delete mp_renderTextureTargetTexture;
		mp_renderTextureTargetTexture = nullptr;
	}

	if (mp_renderTextureRenderTargetView)
	{
		mp_renderTextureRenderTargetView->Release();
		delete mp_renderTextureRenderTargetView;
		mp_renderTextureRenderTargetView = nullptr;
	}

	if (mp_renderTextureResourceView)
	{
		mp_renderTextureResourceView->Release();
		delete mp_renderTextureResourceView;
		mp_renderTextureResourceView = nullptr;
	}

}

bool DirectXClass::ResizeRenderBuffers(HWND hwnd, float af_screenDepth, float af_screenNear)
{
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


	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	// Set up the description of the depth buffer.
	int ai_screenWidth = clientRect.right;
	int ai_screenHeight = clientRect.bottom;


	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetVeiwDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


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

	//l_depthBufferDesc.Width = 1280;
	//l_depthBufferDesc.Height = 780;

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

	// Create the mp_texture for the depth buffer using the filled out description.
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

	// Setup the l_viewport for rendering.
	viewport.Width = (float)ai_screenWidth;
	viewport.Height = (float)ai_screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the l_viewport.
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

bool DirectXClass::Initalize(int ai_screenWidth, int ai_screenHeight, bool ab_vsync,
	HWND hwnd, bool ab_fullScreen, float af_screenDepth, float af_screenNear)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	// Set up the description of the depth buffer.
	ai_screenWidth = clientRect.right;
	ai_screenHeight = clientRect.bottom;

	HRESULT l_result;
	IDXGIFactory* lp_factory;
	IDXGIAdapter* lp_adapter;
	IDXGIOutput* lp_adapterOutput;
	unsigned int li_numModes, li_i, li_numerator, li_denominator;
	unsigned long long li_stringLength;
	DXGI_MODE_DESC* lp_displayModeList;
	DXGI_ADAPTER_DESC l_adapterDesc;
	int li_error;
	DXGI_SWAP_CHAIN_DESC l_swapChainDesc;
	D3D_FEATURE_LEVEL l_featureLevel;
	ID3D11Texture2D* lp_backBufferPtr;
	D3D11_TEXTURE2D_DESC l_depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC l_depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC l_depthStencilViewDesc;
	D3D11_RASTERIZER_DESC l_rasterDesc;
	D3D11_VIEWPORT l_viewport;
	float lf_fieldOfView, lf_screenAspect;

	// Store the vsync setting.
	mb_vsync_enabled = ab_vsync;

	// Create a DirectX graphics interface lp_factory.
	l_result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&lp_factory);
	if (FAILED(l_result))
	{
		return false;
	}

	// Use the lp_factory to create an lp_adapter for the primary graphics interface (video card).
	l_result = lp_factory->EnumAdapters(0, &lp_adapter);
	if (FAILED(l_result))
	{
		return false;
	}

	// Enumerate the primary lp_adapter output (monitor).
	l_result = lp_adapter->EnumOutputs(0, &lp_adapterOutput);
	if (FAILED(l_result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the lp_adapter output (monitor).
	l_result = lp_adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &li_numModes, NULL);
	if (FAILED(l_result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	lp_displayModeList = new DXGI_MODE_DESC[li_numModes];
	if (!lp_displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	l_result = lp_adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &li_numModes, lp_displayModeList);
	if (FAILED(l_result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the li_numerator and li_denominator of the refresh rate for that monitor.
	for (li_i = 0; li_i < li_numModes; li_i++)
	{
		if (lp_displayModeList[li_i].Width == (unsigned int)ai_screenWidth)
		{
			if (lp_displayModeList[li_i].Height == (unsigned int)ai_screenHeight)
			{
				li_numerator = lp_displayModeList[li_i].RefreshRate.Numerator;
				li_denominator = lp_displayModeList[li_i].RefreshRate.Denominator;
			}
		}
	}

	// Get the lp_adapter (video card) description.
	l_result = lp_adapter->GetDesc(&l_adapterDesc);
	if (FAILED(l_result))
	{
		return false;
	}

	// Store the dedicated video card ari_memory in megabytes.
	mi_videoCardMemory = (int)(l_adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	li_error = wcstombs_s(&li_stringLength, mc_videoCardDescription, 128, l_adapterDesc.Description, 128);
	if (li_error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] lp_displayModeList;
	lp_displayModeList = 0;

	// Release the lp_adapter output.
	lp_adapterOutput->Release();
	lp_adapterOutput = 0;

	// Release the lp_adapter.
	lp_adapter->Release();
	lp_adapter = 0;

	// Release the lp_factory.
	lp_factory->Release();
	lp_factory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&l_swapChainDesc, sizeof(l_swapChainDesc));

	// Set to a single back buffer.
	l_swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	l_swapChainDesc.BufferDesc.Width = ai_screenWidth;
	l_swapChainDesc.BufferDesc.Height = ai_screenHeight;

	// Set regular 32-bit surface for the back buffer.
	l_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (mb_vsync_enabled)
	{
		l_swapChainDesc.BufferDesc.RefreshRate.Numerator = li_numerator;
		l_swapChainDesc.BufferDesc.RefreshRate.Denominator = li_denominator;
	}
	else
	{
		l_swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		l_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	l_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	l_swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	l_swapChainDesc.SampleDesc.Count = 1;
	l_swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (ab_fullScreen)
	{
		l_swapChainDesc.Windowed = false;
	}
	else
	{
		l_swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	l_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	l_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	l_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	l_swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	l_featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (mp_swapChain != nullptr) { ZeroMemory(&mp_swapChain, sizeof(mp_swapChain)); }
	if (mp_device != nullptr) { ZeroMemory(&mp_device, sizeof(mp_device)); }
	if (mp_deviceContext != nullptr) { ZeroMemory(&mp_deviceContext, sizeof(mp_deviceContext)); }
	// Create the swap chain, Direct3D device, and Direct3D device context.
	l_result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &l_featureLevel, 1,
		D3D11_SDK_VERSION, &l_swapChainDesc, &mp_swapChain, &mp_device, NULL, &mp_deviceContext);
	if (FAILED(l_result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	l_result = mp_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&lp_backBufferPtr);
	if (FAILED(l_result))
	{
		return false;
	}

	if (mp_renderTargetView != nullptr) { ZeroMemory(&mp_renderTargetView, sizeof(mp_renderTargetView)); }
	// Create the render target view with the back buffer pointer.
	l_result = mp_device->CreateRenderTargetView(lp_backBufferPtr, NULL, &mp_renderTargetView);
	if (FAILED(l_result))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC l_textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC l_renderTargetVeiwDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC l_shaderResourceViewDesc;

	ZeroMemory(&l_textureDesc, sizeof(l_textureDesc));

	l_textureDesc.Width = ai_screenWidth;
	l_textureDesc.Height = ai_screenHeight;
	l_textureDesc.MipLevels = 1;
	l_textureDesc.ArraySize = 1;
	l_textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	l_textureDesc.SampleDesc.Count = 1;
	l_textureDesc.Usage = D3D11_USAGE_DEFAULT;
	l_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	l_textureDesc.CPUAccessFlags = 0;
	l_textureDesc.MiscFlags = 0;

	
	if (mp_renderTextureTargetTexture != nullptr) { ZeroMemory(&mp_renderTextureTargetTexture, sizeof(mp_renderTextureTargetTexture)); }
	mp_device->CreateTexture2D(&l_textureDesc, NULL, &mp_renderTextureTargetTexture);

	l_renderTargetVeiwDesc.Format = l_textureDesc.Format;
	l_renderTargetVeiwDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	l_renderTargetVeiwDesc.Texture2D.MipSlice = 0;

	if (mp_renderTextureRenderTargetView != nullptr) { ZeroMemory(&mp_renderTextureRenderTargetView, sizeof(mp_renderTextureRenderTargetView)); }
	mp_device->CreateRenderTargetView(mp_renderTextureTargetTexture, &l_renderTargetVeiwDesc, &mp_renderTextureRenderTargetView);

	//resourchView
	l_shaderResourceViewDesc.Format = l_textureDesc.Format;
	l_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	l_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	l_shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (mp_renderTextureResourceView != nullptr) { ZeroMemory(&mp_renderTextureResourceView, sizeof(mp_renderTextureResourceView)); }
	mp_device->CreateShaderResourceView(mp_renderTextureTargetTexture, &l_shaderResourceViewDesc, &mp_renderTextureResourceView);

	// Release pointer to the back buffer as we no longer need it.
	lp_backBufferPtr->Release();
	lp_backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&l_depthBufferDesc, sizeof(l_depthBufferDesc));
	
	//l_depthBufferDesc.Width = 1280;
	//l_depthBufferDesc.Height = 780;

	l_depthBufferDesc.Width = ai_screenWidth;
	l_depthBufferDesc.Height = ai_screenHeight;
	l_depthBufferDesc.MipLevels = 1;
	l_depthBufferDesc.ArraySize = 1;
	l_depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	l_depthBufferDesc.SampleDesc.Count = 1;
	l_depthBufferDesc.SampleDesc.Quality = 0;
	l_depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	l_depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	l_depthBufferDesc.CPUAccessFlags = 0;
	l_depthBufferDesc.MiscFlags = 0;

	if (mp_depthStencilBuffer != nullptr) { ZeroMemory(&mp_depthStencilBuffer, sizeof(mp_depthStencilBuffer)); }
	l_result = mp_device->CreateTexture2D(&l_depthBufferDesc, NULL, &mp_depthStencilBuffer);
	if (FAILED(l_result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&l_depthStencilDesc, sizeof(l_depthStencilDesc));

	// Set up the description of the stencil state.
	l_depthStencilDesc.DepthEnable = true;
	l_depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	l_depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	l_depthStencilDesc.StencilEnable = true;
	l_depthStencilDesc.StencilReadMask = 0xFF;
	l_depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	l_depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	l_depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	l_depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	l_depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	l_depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	l_depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	l_depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	l_depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (mp_depthStencilState != nullptr) { ZeroMemory(&mp_depthStencilState, sizeof(mp_depthStencilState)); }
	// Create the depth stencil state.
	l_result = mp_device->CreateDepthStencilState(&l_depthStencilDesc, &mp_depthStencilState);
	if (FAILED(l_result))
	{
		return false;
	}

	// Set the depth stencil state.
	mp_deviceContext->OMSetDepthStencilState(mp_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&l_depthStencilViewDesc, sizeof(l_depthStencilViewDesc));

	// Set up the depth stencil view description.
	l_depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	l_depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	l_depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (mp_depthStencilView != nullptr) { ZeroMemory(&mp_depthStencilView, sizeof(mp_depthStencilView)); }
	// Create the depth stencil view.
	l_result = mp_device->CreateDepthStencilView(mp_depthStencilBuffer, &l_depthStencilViewDesc, &mp_depthStencilView);
	if (FAILED(l_result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mp_deviceContext->OMSetRenderTargets(1, &mp_renderTextureRenderTargetView, mp_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	l_rasterDesc.AntialiasedLineEnable = false;
	l_rasterDesc.CullMode = D3D11_CULL_BACK;
	l_rasterDesc.DepthBias = 0;
	l_rasterDesc.DepthBiasClamp = 0.0f;
	l_rasterDesc.DepthClipEnable = true;
	l_rasterDesc.FillMode = D3D11_FILL_SOLID;
	l_rasterDesc.FrontCounterClockwise = false;
	l_rasterDesc.MultisampleEnable = false;
	l_rasterDesc.ScissorEnable = false;
	l_rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (mp_rasterState != nullptr) { ZeroMemory(&mp_rasterState, sizeof(mp_rasterState)); }
	// Create the rasterizer state from the description we just filled out.
	l_result = mp_device->CreateRasterizerState(&l_rasterDesc, &mp_rasterState);
	if (FAILED(l_result))
	{
		return false;
	}

	// Now set the rasterizer state.
	mp_deviceContext->RSSetState(mp_rasterState);

	// Setup the l_viewport for rendering.
	l_viewport.Width = (float)ai_screenWidth;
	l_viewport.Height = (float)ai_screenHeight;
	l_viewport.MinDepth = 0.0f;
	l_viewport.MaxDepth = 1.0f;
	l_viewport.TopLeftX = 0.0f;
	l_viewport.TopLeftY = 0.0f;

	// Create the l_viewport.
	mp_deviceContext->RSSetViewports(1, &l_viewport);

	// Setup the projection matrix.
	lf_fieldOfView = 3.141592654f / 4.0f;
	lf_screenAspect = (float)ai_screenWidth / (float)ai_screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(lf_fieldOfView, lf_screenAspect, af_screenNear, af_screenDepth);

	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = DirectX::XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)ai_screenWidth, (float)ai_screenHeight, af_screenNear, af_screenDepth);

	return true;
}

void DirectXClass::BeginGameScene(float af_red, float af_green, float af_blue, float af_alpha)
{
	float l_color[4]{af_red, af_green, af_blue, af_alpha};

	// Clear the back buffer.
	mp_deviceContext->ClearRenderTargetView(mp_renderTargetView, l_color);

	// Clear the depth buffer.
	mp_deviceContext->ClearDepthStencilView(mp_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Clear Render Texture
	mp_deviceContext->ClearRenderTargetView(mp_renderTextureRenderTargetView, l_color);
}

void DirectXClass::BeginEditorScene(float af_red, float af_green, float af_blue, float af_alpha)
{
	float l_color[4]{ af_red, af_green, af_blue, af_alpha };

	// Clear the back buffer.
	mp_deviceContext->ClearRenderTargetView(mp_renderTargetView, l_color);

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

void DirectXClass::GetProjectionMatrix(DirectX::XMMATRIX& a_projectionMatrix)
{
	a_projectionMatrix = m_projectionMatrix;
}

void DirectXClass::GetWorldMatrix(DirectX::XMMATRIX& a_worldMatrix)
{
	a_worldMatrix = m_worldMatrix;
}

void DirectXClass::GetOrthoMatrix(DirectX::XMMATRIX& a_orthoMatrix)
{
	a_orthoMatrix = m_orthoMatrix;
}

void DirectXClass::GetVideoCardInfo(char* apc_cardName, int& ari_memory)
{
	strcpy_s(apc_cardName, 128, mc_videoCardDescription);
	ari_memory = mi_videoCardMemory;
}
