#include "Graphics.h"
#include "Game Systems/Components/SpriteRenderer.h"
#include "Game Systems/Components/Sprite.h"
#include "Game Systems/Time.h"

bool tempToggle = false;

Graphics::Graphics()
{
	mp_DirectX = 0;
	mp_Camera = 0;
	mp_Model = 0;
	mp_Shader = 0;
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
	ImGui_ImplDX11_Init(mp_DirectX->GetDevice(), mp_DirectX->GetDeviceContext());
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
	mp_Model->AddComponent<Sprite>()->LoadSprite(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext, "stone.tga");
	SpriteRenderer* spriteRenderer =  mp_Model->AddComponent<SpriteRenderer>();
	
	// Initialize the model object.
	result = spriteRenderer->Initialize(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	mp_Model2 = new GameObject();
	if (!mp_Model2)
	{
		return false;
	}
	mp_Model2->AddComponent<Sprite>()->LoadSprite(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext, "stone.tga");
	result = mp_Model2->AddComponent<SpriteRenderer>()->Initialize(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	mp_Model2->GetTransform()->SetLocalPosition(Vector2(0.0f, 0.0f));
	//mp_Model2->GetTransform()->SetLocalScale(Vector2(0.5f, 0.5f));
	mp_Model2->SetParent(mp_Model);

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
	if (!mp_ImGui)
		return false;

	return true;
}

void Graphics::Update()
{
	mp_Camera->Update();
	mp_Model->Update();
	mp_Model2->Update();

	mp_Model->GetTransform()->SetLocalRotation(mp_Model->GetTransform()->GetLocalRotation() + 0.1f * Time::GetDeltaTime());

	mp_ImGui->Update(mp_DirectX->mp_renderTextureResourceView);
}

bool Graphics::Render()
{	
	if (mp_ImGui->mb_playGame)
		ActiveGameRender();
	else
		EditorRender();
	
	return true;
}

//---------------------------------------------------------------------------//
//------------ Dont touch below unless your sure what your doing ------------//
//---------------------------------------------------------------------------//

//- Editor View with Render Texture -//
bool Graphics::EditorRender()
{
	//- Decliration of matix -//
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mp_DirectX->mp_deviceContext->OMSetRenderTargets(1, &mp_DirectX->mp_renderTextureRenderTargetView, mp_DirectX->mp_depthStencilView);

	// Clear the buffers to begin the scene.
	mp_DirectX->BeginGameScene(0.0f, 0.0f, 0.0f, 1.0f);

	float color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	mp_DirectX->GetDeviceContext()->ClearRenderTargetView(mp_DirectX->mp_renderTextureRenderTargetView, color);

	// Generate the view matrix based on the camera's position.
	mp_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	mp_DirectX->GetWorldMatrix(worldMatrix);
	mp_Camera->GetViewMatrix(viewMatrix);
	mp_DirectX->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mp_Model->Render();
	mp_Model2->Render();

	// Render the model using the color shader.
	result = mp_Shader->Render(mp_DirectX->GetDeviceContext(), 6, mp_Model->GetComponent<Transform>()->GetLocalToWorldMatrix(), viewMatrix, projectionMatrix, mp_Model->GetComponent<Sprite>()->GetTexture());
	if (!result)
	{
		return false;
	}

	// Render the model using the color shader.
	result = mp_Shader->Render(mp_DirectX->GetDeviceContext(), 6, mp_Model2->GetComponent<Transform>()->GetLocalToWorldMatrix(), viewMatrix, projectionMatrix, mp_Model2->GetComponent<Sprite>()->GetTexture());
	if (!result)
	{
		return false;
	}

	//-----------------//
	//- Engine Render -//
	//-----------------//
	mp_DirectX->mp_deviceContext->OMSetRenderTargets(1, &mp_DirectX->mp_renderTargetView, mp_DirectX->mp_depthStencilView);

	// Clear the buffers to begin the scene.
	mp_DirectX->BeginEditorScene(0.0f, 0.0f, 0.0f, 0.0f); //<-- Background colour for editor

	// Generate the view matrix based on the camera's position.
	mp_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	mp_DirectX->GetWorldMatrix(worldMatrix);
	mp_Camera->GetViewMatrix(viewMatrix);
	mp_DirectX->GetProjectionMatrix(projectionMatrix);
	
	//- ImGui Call to render -//
	mp_ImGui->Render();
	mp_DirectX->EndScene();
}

//- Game View -//
bool Graphics::ActiveGameRender()
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mp_DirectX->mp_deviceContext->OMSetRenderTargets(1, &mp_DirectX->mp_renderTargetView, mp_DirectX->mp_depthStencilView);

	// Clear the buffers to begin the scene.
	mp_DirectX->BeginGameScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	mp_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	mp_DirectX->GetWorldMatrix(worldMatrix);
	mp_Camera->GetViewMatrix(viewMatrix);
	mp_DirectX->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mp_Model->Render();

	// Render the model using the color shader.
	result = mp_Shader->Render(mp_DirectX->GetDeviceContext(), 6, mp_Model->GetComponent<Transform>()->GetWorldMatrix(), viewMatrix, projectionMatrix, mp_Model->GetComponent<Sprite>()->GetTexture());
	if (!result)
	{
		return false;
	}

	mp_ImGui->Render();
	mp_DirectX->EndScene();
}