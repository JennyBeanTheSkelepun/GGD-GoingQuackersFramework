#include "Graphics.h"
#include "GameObject/Components/SpriteRenderer.h"

bool tempToggle = false;

Graphics::Graphics()
{
	mp_DirectX = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Shader = 0;
}

Graphics::Graphics(const Graphics& other)
{

}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int ai_screenWidth, int ai_screenHeight, HWND hwnd)
{
	mp_DirectX = std::make_unique<DirectXClass>();
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
	m_Camera = std::make_unique<Camera>();
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create the model object.
	m_Model = std::make_unique<GameObject>();
	if (!m_Model)
	{
		return false;
	}
	SpriteRenderer* spriteRenderer = m_Model->AddComponent<SpriteRenderer>();

	// Initialize the model object.
	result = spriteRenderer->Initialize(mp_DirectX->GetDevice(), mp_DirectX->GetDeviceContext(), ((char*)"stone.tga"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_Shader = std::make_unique<Shader>();
	if (!m_Shader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_Shader->Initialize(mp_DirectX->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}


	mp_ImGui = std::make_unique<EngineGuiClass>();
	mp_ImGui->Initalize();
	if (!mp_ImGui)
		return false;

	return true;
}

void Graphics::Shutdown()
{
	// Release the model object.
	if (m_Model)
		m_Model->~GameObject();

	if (mp_DirectX)
		mp_DirectX->Shutdown();

	return;
}

void Graphics::Update()
{
	m_Camera->Update();
	m_Model->Update();


	mp_ImGui->Update(mp_DirectX->resourchView);
}

bool Graphics::Render()
{	
	//-----------------------//
	//- Inital Scene Render -//
	//-----------------------//

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mp_DirectX->mp_deviceContext->OMSetRenderTargets(1, &mp_DirectX->renderTargetView, mp_DirectX->mp_depthStencilView);

	// Clear the buffers to begin the scene.
	mp_DirectX->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	mp_DirectX->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	mp_DirectX->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(mp_DirectX->GetDeviceContext());

	// Render the model using the color shader.
	result = m_Shader->Render(mp_DirectX->GetDeviceContext(), m_Model->GetComponent<SpriteRenderer>()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetComponent<SpriteRenderer>()->GetTexture());
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
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	mp_DirectX->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	mp_DirectX->GetProjectionMatrix(projectionMatrix);

	//- ImGui Call to render -//
	mp_ImGui->Render();

	mp_DirectX->EndScene();

	return true;
}