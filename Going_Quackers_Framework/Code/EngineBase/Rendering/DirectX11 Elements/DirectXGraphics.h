#ifndef _DIRECTX_GRAPHICS_H_
#define _DIRECTX_GRAPHICS_H_

#include <Windows.h>

#include "../../Rendering/Interface/Graphics_API_Interface.h"
#include "../../Rendering/DirectX11 Elements/DirectXWindow.h"
#include "../../Rendering/DirectX11 Elements/DirectXClass.h"
#include "../../Rendering/DirectX11 Elements/DirectXImGui.h"
#include "../../Rendering/DirectX11 Elements/DirectXCamera.h"
#include "../../Rendering/DirectX11 Elements/DirectXTextureManager.h"
#include "../../Rendering/DirectX11 Elements/DirectXShaderManager.h"
#include "../../Rendering/DirectX11 Elements/DirectXRenderLoop.h"

class SpriteRenderer;
class VirtualCamera;

const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class DirectXGraphics : public GraphicsInterface
{
public:

	bool InitalizeGraphicalApi();
	void SetNewActiveCamera(VirtualCamera* vCam) override;
	VirtualCamera* GetActiveCamera() override;

	int AddObjectToRenderLoop(SpriteRenderer* ar_component);
	int RemoveObjectFromRenderLoop(int index);
	
	int LoadTexture(std::string TextureLocation);
	int RemoveTexture(int index);
	void CleanUpTexturePool();

	int LoadShader(std::wstring ShaderLocation);
	int RemoveShader(int index);
	void CleanUpShaderPool();

	int LoadSpriteSheet(std::string SpriteSheetLocation);
	int RemoveSpriteSheet(int index);
	void CleanUpSpriteSheetPool();
	
	void GraphicsAPIUpdate();
	void StartAPIRenderLoop();

	void ResizeWindowCall();
	Vector2 GetWindowDimentions();

	//- old functions -//
	DirectXGraphics();
	~DirectXGraphics();

//private:

	bool Initialize();
	void Update();

	DirectXWindow* mp_Window;
	// Direct X and Render Texture Setup
	DirectXClass* mp_DirectX;
	// Imgui Renderer
	DirectXImGui* mp_ImGui;
	// Camera
	DirectXCamera* mp_Camera;
	// Texture Manager
	DirectXTextureManager* mp_TextureManager;
	// mp_shader Manager
	DirectXShaderManager* mp_ShaderManager;
	// RenderLoop 
	DirectXRenderLoop* mp_DirectXRenderLoop;
};

#endif // !_DIRECTX_GRAPHICS_H_
