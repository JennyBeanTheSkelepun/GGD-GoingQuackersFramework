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
	void Shutdown();

	void Update();
	bool Render();

private:
	std::unique_ptr<DirectXClass> mp_DirectX;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<GameObject> m_Model;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<EngineGuiClass> mp_ImGui;

};

#endif // !_GRAPHICS_H_
