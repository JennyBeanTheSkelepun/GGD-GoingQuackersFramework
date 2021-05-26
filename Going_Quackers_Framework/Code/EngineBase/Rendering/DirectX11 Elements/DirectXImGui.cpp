#include "DirectXImGui.h"

DirectXImGui::DirectXImGui(HWND hwnd, DirectXClass* mp_DirectX)
{
	// ImGui Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(mp_DirectX->GetDevice(), mp_DirectX->GetDeviceContext());
	ImGui::StyleColorsDark();

	EngineGuiClass::getInstance()->SetImGuiStyle();
	m_scale = 2.0f;
}

DirectXImGui::~DirectXImGui()
{

}

bool DirectXImGui::Update(ID3D11ShaderResourceView* ap_renderTexture, float width, float height)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	EngineGuiClass::getInstance()->Update();

	if (!EngineGuiClass::getInstance()->IsInPlayMode())
	{
		ImTextureID RenderTexture = ap_renderTexture;

		if (EngineGuiClass::getInstance()->isRecording())
		{
			ImGui::SetNextWindowPos(EngineGuiClass::getInstance()->GetWindowInfo()->positions[2]);
			ImGui::SetNextWindowSize(EngineGuiClass::getInstance()->GetWindowInfo()->dimentions[2]);
		}
		ImGui::Begin("Game/Scene View");
		//ImGui::Image(RenderTexture, ImVec2((ImGui::GetWindowWidth() - 15), (ImGui::GetWindowHeight() - 35)));
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SameLine();
		ImGui::InputFloat("input float", &m_scale, 0.5f);
		ImGui::Image(RenderTexture, ImVec2(width / m_scale, height / m_scale));

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
