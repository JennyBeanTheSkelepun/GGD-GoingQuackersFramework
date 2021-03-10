#include "DirectXGraphics.h"
#include "../../Game Systems/Components/SpriteRenderer.h"
#include "../../Game Systems/Components/Sprite.h"
#include "../../Game Systems/Time.h"

bool tempToggle = false;

void DirectXGraphics::StartAPIRenderLoop()
{
	mp_DirectXRenderLoop->Render(*mp_DirectX, *mp_Camera, *mp_ShaderManager, *mp_TextureManager, *mp_ImGui);
}

void DirectXGraphics::AddObjectToRenderLoop(Component& ar_component)
{
	return mp_DirectXRenderLoop->SetObjectToRender(ar_component);
}

bool DirectXGraphics::InitalizeGraphicalApi()
{
	return Initialize();
}

void DirectXGraphics::SetNewActiveCamera(VirtualCamera& NextActiveCamera)
{
}

int DirectXGraphics::LoadTexture(std::string TextureLocation)
{
	return mp_TextureManager->CreateTexture(*mp_DirectX, TextureLocation);
}

int DirectXGraphics::LoadShader(std::string ShaderLocation)
{
	return mp_ShaderManager->CreateShader(*mp_DirectX, *mp_Window, ShaderLocation);
}

int DirectXGraphics::LoadSpriteSheet(std::string SpriteSheetLocation)
{
	return 0;
}

void DirectXGraphics::GraphicsAPIUpdate()
{
	Update();
}


DirectXGraphics::DirectXGraphics() //<-------------- TODO NULLPTR INTIALISATION
{
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

	delete mp_ShaderManager;
	mp_ShaderManager = nullptr;

	delete mp_ImGui;
	mp_ImGui = nullptr;
}

bool DirectXGraphics::Initialize()
{
	mp_Window = new DirectXWindow();

	mp_DirectX = new DirectXClass();
	if (!mp_DirectX)
	{
		return false;
	}

	bool result = mp_DirectX->Initalize(mp_Window->mi_width, mp_Window->mi_height, VSYNC_ENABLED, mp_Window->m_hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(mp_Window->m_hwnd, L"Could Not Initalized Direct X 11", L"Error", MB_OK);
		return false;
	}

	mp_ImGui = new DirectXImGui(mp_Window->m_hwnd, mp_DirectX);
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


	mp_TextureManager = new DirectXTextureManager(*mp_DirectX, "../../../../stone.tga");
	if (!mp_TextureManager)
	{
		return false;
	}

	// Create the color shader object.
	mp_ShaderManager = new DirectXShaderManager(*mp_DirectX, *mp_Window, "../Shaders/TextureSimple.fx");
	if (!mp_ShaderManager)
	{
		MessageBox(mp_Window->m_hwnd, L"Could not initialize the Texture shader object.", L"Error", MB_OK);
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
