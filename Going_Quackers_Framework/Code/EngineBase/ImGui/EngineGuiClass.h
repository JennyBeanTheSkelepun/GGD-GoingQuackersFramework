#ifndef _ENGINE_IM_GUI_
#define _ENGINE_IM_GUI_

#include <d3d11.h>
#include <memory>
#include <string>
#include <vector>
#include "../JSON/nlohmann/json.hpp"

//- only knows windows Elements of Imgui to prevent anyone from messing up Api template code -//
#include "ImGui SourceCode/imgui.h"
#include "ImGui SourceCode/imgui_impl_win32.h"
class GameObject;

struct LayoutSettings
{
	std::string name;
	std::vector<ImVec2> positions;
	std::vector<ImVec2> dimentions;
};

class EngineGuiClass
{
	//- Singleton Decliration -//
public:
	static EngineGuiClass* getInstance();

	EngineGuiClass(EngineGuiClass const&) = delete;
	void operator=(EngineGuiClass const&) = delete;

private:
	static EngineGuiClass* SingletonInstance;
	EngineGuiClass();
	~EngineGuiClass();

	void DisplayChildren(GameObject* gameObject);

public:
	void InitializeObjectList(std::vector<GameObject*>* gameObjects);

	void Update();
	void GameUpdate();
	void EditorUpdate();
	void SetImGuiStyle();

	const char* BoolToString(bool Input);

	inline bool IsInPlayMode() { return mb_playGame; }

	bool SelectableTreeNode(const char* label, bool isSelected);
	LayoutSettings* GetWindowInfo() { return &CurrentWindowPosition; };

	bool isRecording() { return !RecordingLayout; }

private:
	bool mb_maxEditor = false;
	bool mb_maxOnPlay = false;
	bool mb_playGame = false;
	bool mb_closeEditor = false;

	std::string outputText;

	int AmountOfSaves;
	bool RecordingLayout;
	char* LayoutName;
	std::vector<LayoutSettings> WindowPositions;
	LayoutSettings CurrentWindowPosition;

	void LoadWindowPositions();
	void SaveWindowPositionsToFile();

	std::vector<GameObject*>* gameObjects; //List of GameObjects
	GameObject* currentSelected; //The Currently Selected GameObject
};

#endif // !_ENGINE_IM_GUI_