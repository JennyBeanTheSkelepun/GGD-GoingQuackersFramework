#include <iostream>
#include <string.h>
#include "EngineGuiClass.h"
#include "../Game Systems/GameObject.h"
#include "../Game Systems/Debug.h"
#include "../SceneManager/SceneManager.h"

EngineGuiClass* EngineGuiClass::SingletonInstance = 0;

EngineGuiClass* EngineGuiClass::getInstance()
{
	if (SingletonInstance == 0)
		SingletonInstance = new EngineGuiClass();

	return SingletonInstance;
}

EngineGuiClass::EngineGuiClass()
{
	RecordingLayout = false;
	LayoutName = new char[100]();
	SceneToLoad = new char[100]();
	NewSceneID = new char[100]();
	NewSceneType = new char[100]();
	NewSceneName = new char[100]();
	LoadWindowPositions();
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
		if (ImGui::CollapsingHeader(child->GetName().c_str()))
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
			if (ImGui::MenuItem("Save Scene")) { SceneManager::GetInstance()->SaveCurrentScene(); }
			ImGui::Separator();
			ImGui::InputText(":Scene To Load", SceneToLoad, 100);
			if (ImGui::MenuItem("Load Scene")) {
				if (std::string(SceneToLoad) != "") {
					SceneManager::GetInstance()->ChangeScene(SceneToLoad, false);
				}
				else {
					Debug::getInstance()->LogWarning("Please Enter Scene ID (Filename)");
				}
			}
			ImGui::Separator();
			ImGui::InputText(":New Scene ID", NewSceneID, 100);
			ImGui::InputText(":New Display Name", NewSceneName, 100);
			ImGui::InputText(":New Scene Type", NewSceneType, 100);
			if (ImGui::MenuItem("New Scene")) {
				if (std::string(NewSceneID) != "" && std::string(NewSceneName) != "" && std::string(NewSceneType) != "") {
					SceneManager::GetInstance()->NewScene(NewSceneID, NewSceneName, NewSceneType, false);
				}
				else {
					Debug::getInstance()->LogWarning("Please fill in the three boxes");
				}
			}
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
		if (ImGui::BeginMenu("Editor Format"))
		{
			if (ImGui::MenuItem("Start Layout Recording"))
			{
				RecordingLayout = true;
			}
			if (ImGui::MenuItem("End Layout Recording") && RecordingLayout == true)
			{
				CurrentWindowPosition.name = std::string(LayoutName);
				WindowPositions.push_back(CurrentWindowPosition);
				RecordingLayout = false;
				SaveWindowPositionsToFile();
				LoadWindowPositions();
			}
			ImGui::InputText(":Layout Name", LayoutName, 100);
			ImGui::Separator();
			for (int i = 0; i < AmountOfSaves; i++)
			{
				if (ImGui::MenuItem(WindowPositions[i].name.c_str()) && RecordingLayout == false)
				{
					CurrentWindowPosition.name = WindowPositions[i].name;
					CurrentWindowPosition.dimentions = WindowPositions[i].dimentions;
					CurrentWindowPosition.positions = WindowPositions[i].positions;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	currentSelected = nullptr;

	//- Scene Heiarchy -//
	if (isRecording())
	{
		ImGui::SetNextWindowPos(CurrentWindowPosition.positions[0]);
		ImGui::SetNextWindowSize(CurrentWindowPosition.dimentions[0]);
	}
	ImGui::Begin("Scene Hierarchy");
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	int index = 0;
	for (size_t i = 0; i < gameObjects->size(); i++)
	{
		GameObject* gameObject = gameObjects->at(i);
		if(ImGui::CollapsingHeader(gameObject->GetName().c_str()))
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

	CurrentWindowPosition.positions[0].x = ImGui::GetWindowPos().x;
	CurrentWindowPosition.positions[0].y = ImGui::GetWindowPos().y;
	CurrentWindowPosition.dimentions[0].x = ImGui::GetWindowWidth();
	CurrentWindowPosition.dimentions[0].y = ImGui::GetWindowHeight();

	ImGui::End();

	if (isRecording())
	{
		//- Inspector -//
		ImGui::SetNextWindowPos(CurrentWindowPosition.positions[1]);
		ImGui::SetNextWindowSize(CurrentWindowPosition.dimentions[1]);
	}
	ImGui::Begin("Inspector");
	if (currentSelected != nullptr)
		currentSelected->ImGUIUpdate();

	CurrentWindowPosition.positions[1].x = ImGui::GetWindowPos().x;
	CurrentWindowPosition.positions[1].y = ImGui::GetWindowPos().y;
	CurrentWindowPosition.dimentions[1].x = ImGui::GetWindowWidth();
	CurrentWindowPosition.dimentions[1].y = ImGui::GetWindowHeight();

	ImGui::End();

	if (isRecording())
	{
		//- Output Log -//
		ImGui::SetNextWindowPos(CurrentWindowPosition.positions[3]);
		ImGui::SetNextWindowSize(CurrentWindowPosition.dimentions[3]);
	}
	ImGui::Begin("OutputLog");
	ImGui::Text(outputText.c_str()); // Game Object system stuff; todo switch to debug logging
	Debug::getInstance()->ReadLog();

	CurrentWindowPosition.positions[3].x = ImGui::GetWindowPos().x;
	CurrentWindowPosition.positions[3].y = ImGui::GetWindowPos().y;
	CurrentWindowPosition.dimentions[3].x = ImGui::GetWindowWidth();
	CurrentWindowPosition.dimentions[3].y = ImGui::GetWindowHeight();

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
	bool opened = ImGui::Selectable(label, true);
	ImGui::PopID();

	if (opened)
		ImGui::TreePush(label);

	return opened;
}

void EngineGuiClass::LoadWindowPositions()
{
	WindowPositions.clear();
	CurrentWindowPosition.positions.clear();
	CurrentWindowPosition.dimentions.clear();
	CurrentWindowPosition.name = "";

	json fileIn;
	std::ifstream file("GUILayouts.json");
	LayoutSettings temp;

	file >> fileIn;

	AmountOfSaves = fileIn[0]["AmmountOfFormats"];

	for (int i = 1; i <= AmountOfSaves; i++)
	{
		temp.name = fileIn[i - 1]["Name"];
		for (int j = 0; j < 4; j++)
			temp.positions.push_back(ImVec2(fileIn[i - 1]["Positions"][j]["X"], fileIn[i - 1]["Positions"][j]["Y"]));
		
		for (int k = 0; k < 4; k++)
			temp.dimentions.push_back(ImVec2(fileIn[i - 1]["Dimentions"][k]["X"], fileIn[i - 1]["Dimentions"][k]["Y"]));
		
		WindowPositions.push_back(temp);
		
		temp.dimentions.clear();
		temp.positions.clear();
		temp.name = "";
	}
	CurrentWindowPosition = WindowPositions[0];
}

void EngineGuiClass::SaveWindowPositionsToFile()
{
	json outFile;
	std::ofstream file;
	file.open("GUILayouts.json", std::ofstream::out | std::ofstream::trunc);

	outFile[0]["AmmountOfFormats"] = WindowPositions.size();
	for (int i = 1; i <= WindowPositions.size(); i++)
	{
		outFile[i - 1]["Name"] = WindowPositions[i-1].name;
		for (int j = 0; j < 4; j++)
		{
			outFile[i - 1]["Positions"][j]["X"] = WindowPositions[i - 1].positions[j].x;
			outFile[i - 1]["Positions"][j]["Y"] = WindowPositions[i - 1].positions[j].y;
		}

		for (int k = 0; k < 4; k++)
		{
			outFile[i - 1]["Dimentions"][k]["X"] = WindowPositions[i - 1].dimentions[k].x;
			outFile[i - 1]["Dimentions"][k]["Y"] = WindowPositions[i - 1].dimentions[k].y;
		}
	}
	file << outFile;
}