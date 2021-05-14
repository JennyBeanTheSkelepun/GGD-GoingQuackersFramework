#include "EngineGuiClass.h"
#include "../Game Systems/GameObject.h"

EngineGuiClass* EngineGuiClass::SingletonInstance = 0;

EngineGuiClass* EngineGuiClass::getInstance()
{
	if (SingletonInstance == 0)
		SingletonInstance = new EngineGuiClass();

	return SingletonInstance;
}

EngineGuiClass::EngineGuiClass()
{
}

EngineGuiClass::~EngineGuiClass()
{
	currentSelected = nullptr;
}

void EngineGuiClass::DisplayChildren(GameObject* gameObject)
{
	for (size_t i = 0; i < gameObject->GetChildren().size(); i++)
	{
		GameObject* child = gameObject->GetChildren()[i];
		if (ImGui::CollapsingHeader(child->name.c_str()))
		{
			currentSelected = child;
			DisplayChildren(child);
		}
	}
}

void EngineGuiClass::InitializeObjectList(std::vector<GameObject*>* gameObjects)
{
	this->gameObjects = gameObjects;
}

void EngineGuiClass::Update()
{
	if (mb_playGame)
	{
		GameUpdate();
	}
	else
	{
		EditorUpdate();
	}
}

void EngineGuiClass::GameUpdate()
{
	ImGui::Begin("In Game GUI");
	if (ImGui::Button("Exit Play Mode")) { mb_playGame = false; }
	ImGui::End();
}

void EngineGuiClass::EditorUpdate()
{

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

	currentSelected = nullptr;

	//- Scene Heiarchy -//
	ImGui::Begin("Scene Heiarchy");
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	int index = 0;
	for (size_t i = 0; i < gameObjects->size(); i++)
	{
		GameObject* gameObject = gameObjects->at(i);
		if(ImGui::CollapsingHeader(gameObject->name.c_str()))
		{
			index = i;
			DisplayChildren(gameObject);
		}
	}
	
	if (currentSelected == nullptr && !gameObjects->empty())
		currentSelected = gameObjects->at(index);
	
	//Create GameObjects
	if (ImGui::Button("Create GameObject"))
	{
		std::string name = "GameObject " + std::to_string(rand());
		GameObject* gameObject = new GameObject(name.c_str());
		gameObjects->push_back(gameObject);
	}
	ImGui::End();

	//- Inspector -//
	ImGui::Begin("Inspector");
	if (currentSelected != nullptr)
		currentSelected->ImGUIUpdate();
	ImGui::End();

	//- Output Log -//
	ImGui::Begin("OutputLog");
	ImGui::Text(outputText.c_str());
	ImGui::End();
}

void EngineGuiClass::SetImGuiStyle()
{
	//- Im GUI STYLE GOTTEN FROM ONLINE HERE https://github.com/ocornut/imgui/issues/438 -//


	//- SET UP ALL IMGUI STYLES HERE -//

	ImGuiStyle& style = ImGui::GetStyle();
	//- Set style Here -//

	static int hue = 140;
	static float col_main_sat = 180.f / 255.f;
	static float col_main_val = 161.f / 255.f;
	static float col_area_sat = 124.f / 255.f;
	static float col_area_val = 100.f / 255.f;
	static float col_back_sat = 59.f / 255.f;
	static float col_back_val = 40.f / 255.f;

	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor::HSV(hue / 255.f, col_main_sat, col_main_val);
	ImVec4 col_back = ImColor::HSV(hue / 255.f, col_back_sat, col_back_val);
	ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

	style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
}

const char* EngineGuiClass::BoolToString(bool Input)
{
	if (Input)
		return "True";
	else
		return "False";
}

bool EngineGuiClass::SelectableTreeNode(const char* label, bool isSelected)
{
	// Selection
	if (isSelected)
	{
		ImU32 col = ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
		/*x += os.x;
		y += os.y;
		ImGui::RenderFrame(ImVec2(x, y), ImVec2(x + itemFullWidth, y + itemFullHeight), col, true, 3.f);
		x -= os.x;
		y -= os.y;*/
	}

	ImGui::PushID(label);
	bool opened = ImGui::CollapsingHeader(label, true);
	ImGui::PopID();

	if (opened)
		ImGui::TreePush(label);

	return opened;
}
