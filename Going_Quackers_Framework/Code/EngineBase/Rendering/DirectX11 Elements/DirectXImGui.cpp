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
}

DirectXImGui::~DirectXImGui()
{

}

bool DirectXImGui::Update(ID3D11ShaderResourceView* ap_renderTexture)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!EngineGuiClass::getInstance()->IsInPlayMode())
	{
		ImTextureID RenderTexture = 0;  
		ImGui::Begin("Game/Scene View");
		ImGui::Image(RenderTexture, ImVec2((ImGui::GetWindowWidth() - 50), (ImGui::GetWindowHeight() - 50)));
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
}
