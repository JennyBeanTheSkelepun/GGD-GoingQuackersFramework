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

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class DirectXGraphics : public GraphicsInterface
{
public:

	bool SetupRenderingSystem();
	bool ResizeRenderingSystem();
	bool SetUpImGui();
	 
	//- Required Rendering Loop Elements -//
	bool StartRenderLoop();
	void AddObjectToRenderLoop(Component& ar_component);

	//- Camera Elements -//
	bool SetUpCamera();
	bool SetNewActiveCamera(VirtualCamera& NextActiveCamera);

	//- Render Texture Setup -//
	bool SetupRenderTexture();

	//- Texture Loading -//
	bool LoadTexture(std::string TextureLocation);
	bool TileSheetLoad(std::string SpriteSheetLocation);

	//- Shader Loading -//
	bool CreateShader(std::string ShaderLocation);
	bool GetShader();

	//- Window Creation -//
	bool CreateEngineWindow();

	//- old functions -//

	DirectXGraphics();
	~DirectXGraphics();

private:

	bool Initialize(int ai_screenWidth, int ai_screenHeight, HWND hwnd);

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

	// Shader
	DirectXShader* mp_Shader;

	// Texture Manager
	DirectXTextureManager* mp_TextureManager;

	// Shader Manager
	DirectXShaderManager* mp_ShaderManager;
};

#endif // !_DIRECTX_GRAPHICS_H_
