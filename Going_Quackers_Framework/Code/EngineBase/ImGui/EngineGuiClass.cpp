#include "EngineGuiClass.h"
#include "../Game Systems/GameObject.h"

EngineGuiClass::EngineGuiClass()
{
}

EngineGuiClass::~EngineGuiClass()
{
}

void EngineGuiClass::Update(ID3D11ShaderResourceView* a_RenderTexture, GameObject* obj)
{
	if (mb_playGame)
	{
		GameUpdate();
	}
	else
	{
		EditorUpdate(a_RenderTexture, obj);
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

void EngineGuiClass::EditorUpdate(ID3D11ShaderResourceView* a_RenderTexture, GameObject* obj)
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
			if (ImGui::MenuItem("Maxamise On Play", BoolToString(mb_maxOnPlay))) { mb_maxOnPlay = !mb_maxOnPlay; }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	//- Scene Heiarchy -//
	ImGui::Begin("Scene Heiarchy");
	ImGui::End();

	//- Game View -//
	ImGui::Begin("Game/Scene View");
	ImGui::Image(RenderTexture, ImVec2((ImGui::GetWindowWidth() - 50), (ImGui::GetWindowHeight() - 50)));
	ImGui::End();

	//- Inspector -//
	ImGui::Begin("Inspector");
	/*
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::Text("Position");
			static float vec2a[2] = { obj->transform->GetPosition().X, obj->transform->GetPosition().Y };
			ImGui::InputFloat2("1", vec2a);
			obj->transform->SetLocalPosition(Vector2(vec2a[0], vec2a[1]));

			ImGui::Text("Rotation");
			static float rotation[1] = { 0.0f };
			ImGui::InputFloat("", rotation);

			ImGui::Text("Scale");
			static float vec4b[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
			ImGui::InputFloat2("2", vec4b);

			ImGui::TreePop();
		}
	*/
	std::vector<Component*> components = obj->GetComponents();
	for (size_t i = 0; i < components.size(); i++)
	{
		Component* component = components[i];
		component->ImGUIUpdate();
	}
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

const char* EngineGuiClass::BoolToString(bool Input)
{
	if (Input)
		return "True";
	else
		return "False";
}