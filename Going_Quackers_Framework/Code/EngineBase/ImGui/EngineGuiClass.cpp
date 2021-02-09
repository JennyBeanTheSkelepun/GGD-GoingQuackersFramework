#include "EngineGuiClass.h"

EngineGuiClass::EngineGuiClass()
{
}

EngineGuiClass::~EngineGuiClass()
{
}

void EngineGuiClass::Initalize()
{

}

void EngineGuiClass::Update(ID3D11ShaderResourceView* a_RenderTexture)
{
	ImTextureID RenderTexture = a_RenderTexture;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//- Scene Heiarchy -//
	ImGui::Begin("Scene Heiarchy");
	if (ImGui::Button("TEST"))
		mb_toggleTestVar = !mb_toggleTestVar;
	if (mb_toggleTestVar)
		ImGui::Text("Test");
	ImGui::End();

	//- Game View -//
	ImGui::Begin("Game/Scene View");

	ImGui::Image(RenderTexture, ImVec2((ImGui::GetWindowWidth() - 50), (ImGui::GetWindowHeight() - 50)));
	ImGui::End();

	//- Inspector -//
	ImGui::Begin("Inspector");
	ImGui::End();

	//- Output Log -//
	ImGui::Begin("OutputLog");
	ImGui::End();

	ImGui::Render();
}

void EngineGuiClass::Render()
{
	//- ImGui Call to render -//
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
