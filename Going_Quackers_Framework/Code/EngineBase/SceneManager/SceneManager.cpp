#include "SceneManager.h"

#include <ostream>
#include <fstream>
#include <filesystem>
#include "../JSON/nlohmann/json.hpp" // Adding JSON for modern C++

#include "../Game Systems/GameObject.h"

// For Convienience
using json = nlohmann::json;

SceneManager::SceneManager()
{
	mp_CurrentScene = nullptr;
}

SceneManager::~SceneManager()
{
	delete mp_CurrentScene;
}

void SceneManager::ChangeScene(std::string as_SceneID)
{
	// Unload current Scene
	UnloadScene();

	// Get path to JSON scene config
	std::string ls_SceneConfigPath = "";

	// Load new Scene
	mp_CurrentScene = LoadScene(ls_SceneConfigPath);
}

/* Updates currently loaded scene
*/
void SceneManager::Update(float af_deltaTime)
{
	mp_CurrentScene->Update(af_deltaTime);
}

/* Draws currently loaded scene
*/
void SceneManager::Draw()
{
	mp_CurrentScene->Draw();
}

/* Loads the scene from JSON config
* 
* as_Path - String containing path to JSON file for Scene Config
*/
Scene* SceneManager::LoadScene(std::string as_Path)
{
	// Load Config from JSON file
	std::ifstream l_file(as_Path);
	json l_SceneConfig;
	l_file >> l_SceneConfig;

	// Create new scene object
	Scene* lp_NewScene = new Scene(
		l_SceneConfig["sceneID"],
		l_SceneConfig["sceneName"],
		l_SceneConfig["sceneType"]);

	// Create game objects and add to scene
	for (int i = 0; i < sizeof(l_SceneConfig["objects"]); i++) {

	}

	// Close file
	l_file.close();
	// Return loaded Scene
	return lp_NewScene;
}

/* Removes the scene from loaded scenes
*/
void SceneManager::UnloadScene()
{
	if (mp_CurrentScene != nullptr) {
		delete mp_CurrentScene;
	}
}

ObjectIDs SceneManager::ObjectIDStringToEnum(std::string as_id)
{
	if (as_id == "debugSquare") {
		return debugSquare;
	}
	else {
		return invalidOption;
	}
}

void SceneManager::BuildObjectFromID(std::string as_id, float af_startX, float af_startY)
{
	// Convert String ID to Enum ID
	ObjectIDs l_ObjectID = ObjectIDStringToEnum(as_id);

	// Determine object type and set it up
	switch (l_ObjectID) {
	// Basic Object
	case debugSquare:
		GameObject* l_NewDebugSquare = new GameObject();
		
		mp_CurrentScene->AddObject(l_NewDebugSquare);
		break;
	default:
		break;
	}
}
