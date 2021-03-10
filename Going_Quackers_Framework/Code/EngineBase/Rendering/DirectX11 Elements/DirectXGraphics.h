#ifndef _DIRECTX_GRAPHICS_H_
#define _DIRECTX_GRAPHICS_H_

#include <Windows.h>

//- Need to forward declare Interface as its included in graphics singleton -//
#include "../../Rendering/Interface/Graphics_API_Interface.h"
#include "../../Rendering/DirectX11 Elements/DirectXImGui.h"
#include "../../Rendering/DirectX11 Elements/DirectXClass.h"
#include "../../Rendering/DirectX11 Elements/DirectXRenderLoop.h"
#include "../../Rendering/DirectX11 Elements/DirectXCamera.h"
#include "../../Rendering/DirectX11 Elements/DirectXTextureManager.h"
#include "../../Rendering/DirectX11 Elements/DirectXShaderManager.h"
#include "../../Rendering/DirectX11 Elements/DirectXWindow.h"

const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class DirectXGraphics : public GraphicsInterface
{
public:

	bool InitalizeGraphicalApi() override;
	//void SetNewActiveCamera(VirtualCamera& vCam) override;

	int AddObjectToRenderLoop(SpriteRenderer* ar_component) override;
	int RemoveObjectFromRenderLoop(int index) override;
	
	int LoadTexture(std::string TextureLocation) override;
	int RemoveTexture(int index) override;
	void CleanUpTexturePool() override;

	int LoadShader(std::string ShaderLocation) override;
	int RemoveShader(int index) override;
	void CleanUpShaderPool() override;

	int LoadSpriteSheet(std::string SpriteSheetLocation) override;
	int RemoveSpriteSheet(int index) override;
	void CleanUpSpriteSheetPool() override;
	
	void GraphicsAPIUpdate() override;
	void StartAPIRenderLoop() override;

	//- old functions -//
	DirectXGraphics();
	~DirectXGraphics();

//private:

	bool Initialize();

	void Update();

	//std::vector<GameObject*> gameObjects;

	// Direct X and Render Texture Setup
	DirectXClass* mp_DirectX;

	// Imgui Renderer
	DirectXImGui* mp_ImGui;

	// RenderLoop 
	DirectXRenderLoop* mp_DirectXRenderLoop;

	// Camera
	DirectXCamera* mp_Camera;

	// Texture Manager
	DirectXTextureManager* mp_TextureManager;

	// Shader Manager
	DirectXShaderManager* mp_ShaderManager;

	DirectXWindow* mp_Window;
};

#endif // !_DIRECTX_GRAPHICS_H_
