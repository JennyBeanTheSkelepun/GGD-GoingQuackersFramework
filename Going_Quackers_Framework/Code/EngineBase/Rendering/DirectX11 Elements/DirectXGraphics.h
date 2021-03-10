#ifndef _DIRECTX_GRAPHICS_H_
#define _DIRECTX_GRAPHICS_H_

#include <Windows.h>
#include <iostream>

#include "../../Rendering/Interface/Graphics_API_Interface.h"

#include "../../Game Systems/GameObject.h"

#include "../../ImGui/ImGui SourceCode/imgui.h"
#include "../../ImGui/ImGui SourceCode/imgui_impl_win32.h"
#include "../../ImGui/ImGui SourceCode/imgui_impl_dx11.h"


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

	bool InitalizeGraphicalApi();
	void SetNewActiveCamera(VirtualCamera& vCam);
	void AddObjectToRenderLoop(Component& ar_component);
	int LoadTexture(std::string TextureLocation);
	int LoadShader(std::string ShaderLocation);
	int LoadSpriteSheet(std::string SpriteSheetLocation);
	void GraphicsAPIUpdate();
	void StartAPIRenderLoop();

	//- old functions -//
	DirectXGraphics();
	~DirectXGraphics();

private:

	bool Initialize();

	void Update();

	std::vector<GameObject*> gameObjects;

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
