#include "Graphics.h"

bool tempToggle = false;

Graphics::Graphics()
{
	mp_DirectX = 0;
	mp_Camera = 0;
	mp_Model = 0;
	mp_Shader = 0;
}

Graphics::Graphics(const Graphics& other)
{

}

Graphics::~Graphics()
{
	delete mp_DirectX;
	mp_DirectX = nullptr;

	delete mp_Camera;
	mp_Camera = nullptr;

	delete mp_Model;
	mp_Model = nullptr;

	delete mp_Shader;
	mp_Shader = nullptr;

	delete mp_ImGui;
	mp_ImGui = nullptr;
}

bool Graphics::Initialize(int ai_screenWidth, int ai_screenHeight, HWND hwnd)
{
	mp_DirectX = new DirectXClass();
	if (!mp_DirectX)
	{
		return false;
	}

	bool result = mp_DirectX->Initalize(ai_screenWidth, ai_screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could Not Initalized Direct X 11", L"Error", MB_OK);
		return false;
	}

	// ImGui Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->mp_DirectX->GetDevice(), this->mp_DirectX->GetDeviceContext());
	ImGui::StyleColorsDark();

	//- OBject Creation -//
	// Create the camera object.
	mp_Camera = new Camera();
	if (!mp_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	mp_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create the model object.
	mp_Model = new GameObject();
	if (!mp_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = mp_Model->Initialize(mp_DirectX->GetDevice(), mp_DirectX->GetDeviceContext(), ((char*)"stone.tga"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	mp_Shader = new Shader();
	if (!mp_Shader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = mp_Shader->Initialize(mp_DirectX->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}


	mp_ImGui = new EngineGuiClass();
	mp_ImGui->Initalize();
	if (!mp_ImGui)
		return false;

	return true;
}

void Graphics::Update()
{
	mp_Camera->Update();
	mp_Model->Update();


	mp_ImGui->Update(mp_DirectX->mp_renderTextureResourceView);
}

bool Graphics::Render()
{	
	//-----------------------//
	//- Inital Scene Render -//
	//-----------------------//

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mp_DirectX->mp_deviceContext->OMSetRenderTargets(1, &mp_DirectX->mp_renderTextureRenderTargetView, mp_DirectX->mp_depthStencilView);

	// Clear the buffers to begin the scene.
	mp_DirectX->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	mp_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	mp_DirectX->GetWorldMatrix(worldMatrix);
	mp_Camera->GetViewMatrix(viewMatrix);
	mp_DirectX->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mp_Model->Render(mp_DirectX->GetDeviceContext());

	// Render the model using the color shader.
	result = mp_Shader->Render(mp_DirectX->GetDeviceContext(), mp_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, mp_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	//-----------------//
	//- Engine Render -//
	//-----------------//
	mp_DirectX->mp_deviceContext->OMSetRenderTargets(1, &mp_DirectX->mp_renderTargetView, mp_DirectX->mp_depthStencilView);

	// Clear the buffers to begin the scene.
	mp_DirectX->BeginScene(0.0f, 0.0f, 0.0f, 0.0f); //<-- Background colour for editor

	// Generate the view matrix based on the camera's position.
	mp_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	mp_DirectX->GetWorldMatrix(worldMatrix);
	mp_Camera->GetViewMatrix(viewMatrix);
	mp_DirectX->GetProjectionMatrix(projectionMatrix);

	//- ImGui Call to render -//
	mp_ImGui->Render();

	mp_DirectX->EndScene();

	return true;
}