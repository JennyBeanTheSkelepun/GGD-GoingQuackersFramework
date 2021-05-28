#include "DirectXGraphics.h"

DirectXGraphics::DirectXGraphics() : GraphicsInterface() //<-------------- TODO NULLPTR INTIALISATION
{
	mp_Camera = nullptr;
	mp_DirectX = nullptr;
	mp_DirectXRenderLoop = nullptr;
	mp_ImGui = nullptr;
	mp_ShaderManager = nullptr;
	mp_TextureManager = nullptr;
	mp_AudioManager = nullptr;
	mp_Window = nullptr;
}

DirectXGraphics::~DirectXGraphics()
{
	delete mp_DirectX;
	mp_DirectX = nullptr;

	delete mp_Camera;
	mp_Camera = nullptr;

	delete mp_ShaderManager;
	mp_ShaderManager = nullptr;

	delete mp_ImGui;
	mp_ImGui = nullptr;
}

void DirectXGraphics::StartAPIRenderLoop()
{
	mp_DirectXRenderLoop->Render(*mp_DirectX, *mp_Camera, *mp_ShaderManager, *mp_TextureManager, *mp_ImGui);
}

void DirectXGraphics::ResizeWindowCall()
{
	mp_DirectX->Initalize(mp_Window->mi_width, mp_Window->mi_height, VSYNC_ENABLED, mp_Window->m_hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}

Vector2 DirectXGraphics::GetWindowDimensions()
{
	return Vector2(mp_Window->mi_width, mp_Window->mi_height);
}

int DirectXGraphics::AddObjectToRenderLoop(SpriteRenderer* ar_component)
{
	return mp_DirectXRenderLoop->SetObjectToRender(ar_component);
}

int DirectXGraphics::RemoveObjectFromRenderLoop(int index) //<---------------------------------------- TODO ACTUALY REMOVE OBJECTS
{
	return mp_DirectXRenderLoop->RemoveObjectToRenderLoop(index);
}

bool DirectXGraphics::InitalizeGraphicalApi()
{
	return Initialize();
}

void DirectXGraphics::SetNewActiveCamera(VirtualCamera* NextActiveCamera)
{
	mp_Camera->SetNewVirtualCamera(NextActiveCamera);
}

VirtualCamera* DirectXGraphics::GetActiveCamera()
{
	return mp_Camera->GetVirtualCamera();
}

int DirectXGraphics::LoadTexture(std::string TextureLocation)
{
	return mp_TextureManager->CreateTexture(*mp_DirectX, TextureLocation);
}

int DirectXGraphics::RemoveTexture(int index)
{
	return mp_TextureManager->DeleteTexture(index);
}

void DirectXGraphics::CleanUpTexturePool()
{
	mp_TextureManager->RemoveUnusedTextures();
}

int DirectXGraphics::LoadShader(std::wstring ShaderLocation)
{
	return mp_ShaderManager->CreateShader(*mp_DirectX, *mp_Window, ShaderLocation);
}

int DirectXGraphics::RemoveShader(int index)
{
	return mp_ShaderManager->DeleteShader(index);
}

void DirectXGraphics::CleanUpShaderPool()
{
	mp_ShaderManager->RemoveUnusedShader();
}

int DirectXGraphics::LoadSpriteSheet(std::string SpriteSheetLocation)
{
	return -1;
}

int DirectXGraphics::RemoveSpriteSheet(int index)
{
	return -1;
}

void DirectXGraphics::CleanUpSpriteSheetPool()
{
}

void DirectXGraphics::GraphicsAPIUpdate()
{
	Update();
}

bool DirectXGraphics::Initialize()
{
	//- Window needs direct memeory location-//
	mp_Window = new DirectXWindow(this);
	mp_Window->SetupWindow();

	mp_DirectX = new DirectXClass();
	//- the create and test mp_DirectX -//
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


	mp_TextureManager = new DirectXTextureManager(*mp_DirectX, "Assets/stone.tga");
	if (!mp_TextureManager)
	{
		MessageBox(mp_Window->m_hwnd, L"Could not initialize the Texture Manager object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	mp_ShaderManager = new DirectXShaderManager(*mp_DirectX, *mp_Window, L"Assets/Shaders/TextureSimple.fx");
	if (!mp_ShaderManager)
	{
		MessageBox(mp_Window->m_hwnd, L"Could not initialize the Shader Manager object.", L"Error", MB_OK);
		return false;
	}

	mp_AudioManager = AudioManager::GetInstance();
	if (!mp_AudioManager)
	{
		MessageBox(mp_Window->m_hwnd, L"Could not Initalize the Audio Manager", L"Error", MB_OK);
	}

	mp_DirectXRenderLoop = new DirectXRenderLoop(mp_DirectX);

	return true;
}

void DirectXGraphics::Update()
{
	mp_Camera->Update();
	mp_ImGui->Update(mp_DirectX->mp_renderTextureResourceView, mp_Window->mi_width, mp_Window->mi_height);
	mp_AudioManager->Update();
}
