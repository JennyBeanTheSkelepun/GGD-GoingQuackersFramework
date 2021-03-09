#include "DirectXGraphics.h"
#include "../../Game Systems/Components/SpriteRenderer.h"
#include "../../Game Systems/Components/Sprite.h"
#include "../../Game Systems/Time.h"

bool tempToggle = false;

bool DirectXGraphics::SetupRenderingSystem()
{
	return false;
}

bool DirectXGraphics::ResizeRenderingSystem()
{
	return false;
}

bool DirectXGraphics::SetUpImGui()
{
	return false;
}

bool DirectXGraphics::StartRenderLoop()
{
	return mp_DirectXRenderLoop->Render();
}

void DirectXGraphics::AddObjectToRenderLoop(Component& ar_component)
{
	return mp_DirectXRenderLoop->SetObjectToRender(ar_component);
}

bool DirectXGraphics::SetUpCamera()
{
	return false;
}

bool DirectXGraphics::SetNewActiveCamera(VirtualCamera& NextActiveCamera)
{
	return false;
}

bool DirectXGraphics::SetupRenderTexture()
{
	return false;
}

bool DirectXGraphics::LoadTexture(std::string TextureLocation)
{
	return false;
}

bool DirectXGraphics::TileSheetLoad(std::string SpriteSheetLocation)
{
	return false;
}

bool DirectXGraphics::CreateShader(std::string ShaderLocation)
{
	return false;
}

bool DirectXGraphics::GetShader()
{
	return false;
}

bool DirectXGraphics::CreateEngineWindow()
{
	return false;
}

DirectXGraphics::DirectXGraphics()
{
	mp_DirectX = new DirectXClass();
	mp_Camera = 0;
	mp_Shader = 0;
}

DirectXGraphics::~DirectXGraphics()
{
	delete mp_DirectX;
	mp_DirectX = nullptr;

	delete mp_Camera;
	mp_Camera = nullptr;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
		gameObjects[i] = nullptr;
	}

	delete mp_Shader;
	mp_Shader = nullptr;

	delete mp_ImGui;
	mp_ImGui = nullptr;
}

bool DirectXGraphics::Initialize(int ai_screenWidth, int ai_screenHeight, HWND hwnd)
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

	mp_ImGui = new DirectXImGui(hwnd, mp_DirectX);
	if (!mp_ImGui)
		return false;

	//- OBject Creation -//
	// Create the camera object.
	mp_Camera = new DirectXCamera();
	if (!mp_Camera)
	{
		return false;
	}
	// Set the initial position of the camera.
	mp_Camera->SetPosition(Vector3(0.0f, 0.0f, -5.0f));

	mp_TextureManager = new DirectXTextureManager();
	if (!mp_TextureManager)
	{
		return false;
	}

	// Create the color shader object.
	mp_Shader = new DirectXShader();
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

	//// Create the model object.
	//GameObject* mp_Model = new GameObject();
	//if (!mp_Model)
	//{
	//	return false;
	//}
	//mp_Model->AddComponent<Sprite>()->LoadSprite(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext, "stone.tga");
	//SpriteRenderer* spriteRenderer =  mp_Model->AddComponent<SpriteRenderer>();
	//
	//// Initialize the model object.
	//result = spriteRenderer->Initialize(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}

	//mp_Model->GetTransform()->SetPosition(Vector2(2.0f, 0.0f));

	//// Create the model object.
	//GameObject* mp_Model2 = new GameObject();
	//if (!mp_Model2)
	//{
	//	return false;
	//}
	//mp_Model2->AddComponent<Sprite>()->LoadSprite(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext, "stone.tga");
	//result = mp_Model2->AddComponent<SpriteRenderer>()->Initialize(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}
	////mp_Model2->GetTransform()->SetLocalPosition(Vector2(-2.5f, 0.0f));
	//mp_Model2->GetTransform()->SetPosition(Vector2(-2.5f, 0.0f));
	//mp_Model2->GetTransform()->SetLocalScale(Vector2(0.5f, 0.5f));
	//mp_Model2->SetParent(mp_Model);
	//
	//// Create the model object.
	//GameObject* mp_Model3 = new GameObject();
	//if (!mp_Model3)
	//{
	//	return false;
	//}
	//mp_Model3->AddComponent<Sprite>()->LoadSprite(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext, "stone.tga");
	//result = mp_Model3->AddComponent<SpriteRenderer>()->Initialize(mp_DirectX->mp_device, mp_DirectX->mp_deviceContext);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}

	//mp_Model3->GetTransform()->SetPosition(Vector2(-5.0f, 0.0f));
	//mp_Model3->GetTransform()->SetLocalScale(Vector2(0.5f, 0.5f));
	//mp_Model3->SetParent(mp_Model2);

	//gameObjects.push_back(mp_Model);
	//gameObjects.push_back(mp_Model2);
	////gameObjects.push_back(mp_Model3);

	return true;
}

void DirectXGraphics::Update()
{
	mp_Camera->Update();

	//for (size_t i = 0; i < gameObjects.size(); i++)
	//{
	//	gameObjects[i]->Update();
	//}

	//mp_Model2->GetTransform()->SetLocalPosition(mp_Model2->GetTransform()->GetLocalPosition() + Vector2(-0.8f, 0.0f) * Time::GetDeltaTime());
	//gameObjects[0]->GetTransform()->SetPosition(gameObjects[0]->GetTransform()->GetPosition() + Vector2(-0.1f, 0.0f) * Time::GetDeltaTime());
	//gameObjects[1]->GetTransform()->SetPosition(gameObjects[1]->GetTransform()->GetPosition() + Vector2(0.5f, 0.0f) * Time::GetDeltaTime());
	//gameObjects[0]->GetTransform()->SetLocalRotation(gameObjects[0]->GetTransform()->GetLocalRotation() + 20.0f * Time::GetDeltaTime());
	//gameObjects[1]->GetTransform()->SetLocalScale(gameObjects[1]->GetTransform()->GetLocalScale() - Vector2(0.1f, 0.1f) * Time::GetDeltaTime());
	//gameObjects[2]->GetTransform()->SetLocalRotation(gameObjects[2]->GetTransform()->GetLocalRotation() + 100.0f * Time::GetDeltaTime());
	//mp_Model->GetTransform()->SetLocalScale(mp_Model->GetTransform()->GetLocalScale() - Vector2(0.1f, 0.1f) * Time::GetDeltaTime());

	mp_ImGui->Update(mp_DirectX->mp_renderTextureResourceView);
}
