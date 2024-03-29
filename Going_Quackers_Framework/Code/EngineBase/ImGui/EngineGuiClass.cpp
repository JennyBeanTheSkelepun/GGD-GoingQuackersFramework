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
	ChangeSceneType = new char[100]();
	ChangeSceneName = new char[100]();
	LoadWindowPositions();
}

EngineGuiClass::~EngineGuiClass()
{
	currentSelected = nullptr;
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

			if (ImGui::MenuItem("Scene Autosave", BoolToString(SceneManager::GetInstance()->GetAutoSave()))) { SceneManager::GetInstance()->SetAutoSave(!SceneManager::GetInstance()->GetAutoSave()); }
			ImGui::Separator();
			if (ImGui::MenuItem("Save Scene")) { SceneManager::GetInstance()->SaveCurrentScene(); }
			ImGui::Separator();
			ImGui::InputText(":Scene To Load", SceneToLoad, 100);
			if (ImGui::MenuItem("Load Scene")) {
				if (std::string(SceneToLoad) != "") {
					ClearInspector();
					SceneManager::GetInstance()->ChangeScene(SceneToLoad, SceneManager::GetInstance()->GetAutoSave());
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
					ClearInspector();
					SceneManager::GetInstance()->NewScene(NewSceneID, NewSceneName, NewSceneType, SceneManager::GetInstance()->GetAutoSave());
				}
				else if (std::string(NewSceneID) != "") {
					ClearInspector();
					SceneManager::GetInstance()->NewScene(NewSceneID, "New Scene", "DEBUG", SceneManager::GetInstance()->GetAutoSave());
				}
				else {
					Debug::getInstance()->LogWarning("Please fill in the scene ID box");
				}
			}
			ImGui::Separator();
			ImGui::Text("Current Scene ID: ");
			ImGui::SameLine();
			ImGui::Text(SceneManager::GetInstance()->GetCurrentScene()->GetSceneID().c_str());
			ImGui::InputText(":Change Scene Display Name", ChangeSceneName, 100);
			ImGui::InputText(":Change Scene Scene Type", NewSceneType, 100);
			if (ImGui::MenuItem("Update Scene Information")) {
				if (std::string(ChangeSceneName) != "") {
					SceneManager::GetInstance()->GetCurrentScene()->SetSceneDisplayName(ChangeSceneName);
				}
				if (std::string(ChangeSceneType) != "") {
					SceneManager::GetInstance()->GetCurrentScene()->SetSceneType(ChangeSceneType);
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

			AmountOfSaves = WindowPositions.size();

			for (int i(0); i < AmountOfSaves; ++i)
			{
				ImGui::PushID(i);

				ImGui::Separator();
				ImGui::Text(WindowPositions[i].name.c_str());
				
				ImGui::SameLine(272); 
				bool SelectOption = ImGui::Button("S");

				ImGui::SameLine(290); 
				bool DeleteOption = ImGui::Button("X");
				
				if (DeleteOption)
				{
					WindowPosToDelete.push_back(i);
				}
				if (SelectOption)
				{
					CurrentWindowPosition.name = WindowPositions[i].name;
					CurrentWindowPosition.dimensions = WindowPositions[i].dimensions;
					CurrentWindowPosition.positions = WindowPositions[i].positions;
				}

				ImGui::PopID();
			}

			if (ImGui::Button("Save Changes", ImVec2(300, 20)))
			{
				SaveWindowPositionsToFile();
				LoadWindowPositions();
			}

			//- Delete Editor Layouts once not in use -//
			for (int i = 0; i < WindowPosToDelete.size(); i++)
			{
				WindowPositions.erase(WindowPositions.begin() + WindowPosToDelete[i]);
			}
			WindowPosToDelete.clear();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//- Scene Heiarchy -//
	if (isRecording())
	{
		ImGui::SetNextWindowPos(CurrentWindowPosition.positions[0]);
		ImGui::SetNextWindowSize(CurrentWindowPosition.dimensions[0]);
	}
	ImGui::Begin("Scene Hierarchy");
	
	//Create GameObjects
	if (ImGui::Button("Create GameObject"))
	{
		std::string name = "GameObject " + std::to_string(rand());
		GameObject* gameObject = new GameObject(name.c_str());
		gameObject->SetID(name);
		gameObjects->push_back(gameObject);
	}
	ImGui::SameLine();
	ImGui::Text("Currently Editing: "); 
	ImGui::SameLine();
	ImGui::Text(SceneManager::GetInstance()->GetCurrentScene()->GetSceneDisplayName().c_str());

	ImGui::Separator();

	DisplayObjects(*gameObjects);

	CurrentWindowPosition.positions[0].x = ImGui::GetWindowPos().x;
	CurrentWindowPosition.positions[0].y = ImGui::GetWindowPos().y;
	CurrentWindowPosition.dimensions[0].x = ImGui::GetWindowWidth();
	CurrentWindowPosition.dimensions[0].y = ImGui::GetWindowHeight();

	ImGui::End();

	if (isRecording())
	{
		//- Inspector -//
		ImGui::SetNextWindowPos(CurrentWindowPosition.positions[1]);
		ImGui::SetNextWindowSize(CurrentWindowPosition.dimensions[1]);
	}
	ImGui::Begin("Inspector");
	if (currentSelected != nullptr)
		currentSelected->ImGUIUpdate();

	CurrentWindowPosition.positions[1].x = ImGui::GetWindowPos().x;
	CurrentWindowPosition.positions[1].y = ImGui::GetWindowPos().y;
	CurrentWindowPosition.dimensions[1].x = ImGui::GetWindowWidth();
	CurrentWindowPosition.dimensions[1].y = ImGui::GetWindowHeight();

	ImGui::End();

	if (isRecording())
	{
		//- Output Log -//
		ImGui::SetNextWindowPos(CurrentWindowPosition.positions[3]);
		ImGui::SetNextWindowSize(CurrentWindowPosition.dimensions[3]);
	}
	ImGui::Begin("OutputLog");
	Debug::getInstance()->ReadLog();

	CurrentWindowPosition.positions[3].x = ImGui::GetWindowPos().x;
	CurrentWindowPosition.positions[3].y = ImGui::GetWindowPos().y;
	CurrentWindowPosition.dimensions[3].x = ImGui::GetWindowWidth();
	CurrentWindowPosition.dimensions[3].y = ImGui::GetWindowHeight();

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

void EngineGuiClass::DisplayObjects(std::vector<GameObject*>& gameObjects)
{
	GameObject* currentObject;
	int gameObjectSize = gameObjects.size();

	if (gameObjectSize == 0)
		ImGui::Text("No Children Exist");

	for (int i(0); i < gameObjectSize; ++i)
	{
		ImGui::PushID(i);

		currentObject = gameObjects[i];

		//- Controll Buttons -//
		bool is_expanded = ImGui::TreeNodeEx(currentObject->GetName().c_str(), ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth);
		
		ImGui::SameLine(ImGui::GetWindowWidth() - 25);
		bool DeleteElement = ImGui::Button("X");

		ImGui::SameLine(ImGui::GetWindowWidth() - 43);
		bool AddChild = ImGui::Button("+");

		ImGui::SameLine(ImGui::GetWindowWidth() - 68);
		bool MoveUpScene = ImGui::Button("/\\");

		ImGui::SameLine(ImGui::GetWindowWidth() - 93);
		bool MoveDownScene = ImGui::Button("\\/");

		ImGui::SameLine(ImGui::GetWindowWidth() - 111);
		bool Selected = ImGui::Button("S");

		//- Logic -//
		if (is_expanded)
		{
			std::vector<GameObject*> temp = currentObject->GetChildren();
			DisplayObjects(temp);
			ImGui::TreePop();
		}
		
		if (DeleteElement)
		{
			currentSelected = nullptr;
			currentObject->SetToDestroy();
		}
		
		if (AddChild)
		{
			currentObject->AddChild(new GameObject("Child 1", currentObject));
		}
		
		if (MoveUpScene)
		{
			if (i - 1 > -1)
			{
				GameObject* temp = gameObjects[i - 1];
				gameObjects[i - 1] = gameObjects[i];
				gameObjects[i] = temp;
			}
		}
		
		if (MoveDownScene)
		{
			if (i + 1 < gameObjectSize)
			{
				GameObject* temp = gameObjects[i + 1];
				gameObjects[i + 1] = gameObjects[i];
				gameObjects[i] = temp;
			}
		}

		if (Selected)
		{
			if (currentSelected == currentObject)
				currentSelected = nullptr;
			else
				currentSelected = currentObject;
		}
		ImGui::Separator();

		ImGui::PopID();
	}

}

void EngineGuiClass::LoadWindowPositions()
{
	WindowPositions.clear();
	CurrentWindowPosition.positions.clear();
	CurrentWindowPosition.dimensions.clear();
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
			temp.dimensions.push_back(ImVec2(fileIn[i - 1]["Dimensions"][k]["X"], fileIn[i - 1]["Dimensions"][k]["Y"]));
		
		WindowPositions.push_back(temp);
		
		temp.dimensions.clear();
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
			outFile[i - 1]["Dimensions"][k]["X"] = WindowPositions[i - 1].dimensions[k].x;
			outFile[i - 1]["Dimensions"][k]["Y"] = WindowPositions[i - 1].dimensions[k].y;
		}
	}
	file << outFile;
}