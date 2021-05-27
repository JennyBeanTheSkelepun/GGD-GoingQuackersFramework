#include "DirectXImGui.h"

DirectXImGui::DirectXImGui(HWND a_hwnd, DirectXClass* ap_DirectX)
{
	// ImGui Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(a_hwnd);
	ImGui_ImplDX11_Init(ap_DirectX->GetDevice(), ap_DirectX->GetDeviceContext());
	ImGui::StyleColorsDark();

	EngineGuiClass::getInstance()->SetImGuiStyle();
	mf_scale = 2.0f;
}

DirectXImGui::~DirectXImGui()
{

}

bool DirectXImGui::Update(ID3D11ShaderResourceView* ap_renderTexture, float af_width, float af_height)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	EngineGuiClass::getInstance()->Update();

	if (!EngineGuiClass::getInstance()->IsInPlayMode())
	{
		ImTextureID l_RenderTexture = ap_renderTexture;

		if (EngineGuiClass::getInstance()->isRecording())
		{
			ImGui::SetNextWindowPos(EngineGuiClass::getInstance()->GetWindowInfo()->positions[2]);
			ImGui::SetNextWindowSize(EngineGuiClass::getInstance()->GetWindowInfo()->dimentions[2]);
		}
		ImGui::Begin("Game/Scene View");
		ImGui::SetNextItemWidth(250);
		ImGui::InputFloat("input float", &mf_scale, 0.5f);
		ImGui::SameLine();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Image(l_RenderTexture, ImVec2(af_width / mf_scale, af_height / mf_scale));

		EngineGuiClass::getInstance()->GetWindowInfo()->positions[2].x = ImGui::GetWindowPos().x;
		EngineGuiClass::getInstance()->GetWindowInfo()->positions[2].y = ImGui::GetWindowPos().y;
		EngineGuiClass::getInstance()->GetWindowInfo()->dimentions[2].x = ImGui::GetWindowWidth();
		EngineGuiClass::getInstance()->GetWindowInfo()->dimentions[2].y = ImGui::GetWindowHeight();

		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();

	return false;
}

bool DirectXImGui::Render()
{
	//- ImGui Call to render -//
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return true;
}
