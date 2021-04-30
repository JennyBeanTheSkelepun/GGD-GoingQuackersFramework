#ifndef _ENGINE_IM_GUI_
#define _ENGINE_IM_GUI_

#include <d3d11.h>
#include <memory>
#include <string>
#include <vector>

//- only knows windows Elements of Imgui to prevent anyone from messing up Api template code -//
#include "ImGui SourceCode/imgui.h"
#include "ImGui SourceCode/imgui_impl_win32.h"

class GameObject;

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

	void DisplayChildren(GameObject* gameObject, ImGuiTreeNodeFlags node_flags);

public:
	void InitializeObjectList(std::vector<GameObject*> gameObjects);

	void Update();
	void GameUpdate();
	void EditorUpdate(ID3D11ShaderResourceView* a_RenderTexture, GameObject* obj);
	void Render();

	const char* BoolToString(bool Input);

	inline bool IsInPlayMode() { return mb_playGame; }

private:
	bool mb_maxEditor = false;
	bool mb_maxOnPlay = false;
	bool mb_playGame = false;
	bool mb_closeEditor = false;

	std::vector<GameObject*> gameObjects; //List of GameObjects
	GameObject* selectedGameObject; //Used to record which gameobject is currently selected within the inspector
};

#endif // !_ENGINE_IM_GUI_