#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <Windows.h>
#include <iostream>

#include "DirectX/DirectXClass.h"
#include "GameCamera/Camera.h"
#include "GameObject/GameObject.h"
#include "Shader/Shader.h"
#include "../ImGui/EngineGuiClass.h"

#include "../ImGui/ImGui SourceCode/imgui.h"
#include "../ImGui/ImGui SourceCode/imgui_impl_win32.h"
#include "../ImGui/ImGui SourceCode/imgui_impl_dx11.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics& other);
	~Graphics();

	bool Initialize(int ai_screenWidth, int ai_screenHeight, HWND hwnd);

	void Update();
	bool Render();

private:
	DirectXClass* mp_DirectX;
	Camera* mp_Camera;
	GameObject* mp_Model;
	Shader* mp_Shader;
	EngineGuiClass* mp_ImGui;

};

#endif // !_GRAPHICS_H_
