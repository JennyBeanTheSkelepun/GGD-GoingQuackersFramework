#include "EngineGuiClass.h"
#include "../Game Systems/Debug.h"

EngineGuiClass::EngineGuiClass()
{
}

EngineGuiClass::~EngineGuiClass()
{
}

void EngineGuiClass::Update(ID3D11ShaderResourceView* a_RenderTexture)
{
	if (mb_playGame)
	{
		GameUpdate();
	}
	else
	{
		EditorUpdate(a_RenderTexture);
	}
}

void EngineGuiClass::GameUpdate()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("In Game GUI");
	if (ImGui::Button("Exit Play Mode")) { mb_playGame = false; }
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
}

void EngineGuiClass::EditorUpdate(ID3D11ShaderResourceView* a_RenderTexture)
{
	ImTextureID RenderTexture = a_RenderTexture;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Close")) { mb_closeEditor = true; }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Duplicate Scene Object", "CTRL+Z")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("Maximise Editor", BoolToString(mb_maxEditor))) { mb_maxEditor = !mb_maxEditor; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Play"))
		{
			if (ImGui::MenuItem("Play")) { mb_playGame = true; }
			if (ImGui::MenuItem("Stop")) { mb_playGame = false; }
			ImGui::Separator();
			if (ImGui::MenuItem("Maximise On Play", BoolToString(mb_maxOnPlay))) { mb_maxOnPlay = !mb_maxOnPlay; }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	//- Scene Heiarchy -//
	ImGui::Begin("Scene Hierarchy");
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
	std::string l_line;
	std::stringstream l_log = Debug::getInstance()->ReadLog();
	// convert data to ImGui::Text parameters
	while (std::getline(l_log, l_line)) {
		// new text colour
		ImColor colour;
		// turn line into stream
		std::istringstream in(l_line);
		std::string l_text;
		// format: r g b log
		float r, g, b;
		in >> r;
		in >> g;
		in >> b;
		colour = ImColor(r, g, b);
		std::getline(in, l_text);
		ImGui::TextColored(ImVec4(r, g, b, 1.f), l_text.c_str());
	}

	ImGui::End();

	ImGui::Render();
}

void EngineGuiClass::Render()
{
	//- ImGui Call to render -//
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

const char* EngineGuiClass::BoolToString(bool Input)
{
	if (Input)
		return "True";
	else
		return "False";
}