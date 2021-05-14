#include "SceneManager.h"

#include "../Game Systems/Components/SpriteRenderer.h"

#include <ostream>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <locale>


SceneManager* SceneManager::mp_instance = 0;

/// <summary>
/// Returns the instance of the SceneManager Singleton and creates one if none exist.
/// </summary>
/// <returns>Instance of Singleton class</returns>
SceneManager* SceneManager::GetInstance()
{
	if (mp_instance == 0) {
		mp_instance = new SceneManager();
	}
	return mp_instance;
}

/// <summary>
/// SceneManager Constructor
/// </summary>
SceneManager::SceneManager()
{
	mp_CurrentScene = nullptr;
}

SceneManager::~SceneManager()
{
	delete mp_CurrentScene;
}

/// <summary>
/// Initialization function for Scene Manager
/// </summary>
void SceneManager::Initialize()
{

}

/// <summary>
/// Changes the scene to one matching the given ID
/// </summary>
/// <param name="as_SceneID">String ID of the scene, should match file name (without .json)</param>
void SceneManager::ChangeScene(std::string as_SceneID, bool as_SaveToJSON)
{
	// Unload current Scene
	UnloadScene(as_SaveToJSON);

	// Get path to JSON scene config
	std::string ls_SceneConfigPath = "SceneConfig/" + as_SceneID + ".json";

	// Load new Scene
	mp_CurrentScene = LoadScene(ls_SceneConfigPath);
}


/// <summary>
/// Creates a blank scene
/// </summary>
/// <param name="as_SceneID">ID of new scene</param>
/// <param name="as_SceneName">Display name of new scene</param>
/// <param name="as_SceneType">Type of new scene</param>
/// <param name="as_SaveToJSON">Set to true to save the old scene to JSON</param>
void SceneManager::NewScene(std::string as_SceneID, std::string as_SceneName, std::string as_SceneType, bool as_SaveToJSON)
{
	UnloadScene(as_SaveToJSON);

	// Create new scene object
	Scene* lp_NewScene = new Scene(as_SceneID, as_SceneName, as_SceneType);
	mp_CurrentScene = lp_NewScene;
}

void SceneManager::SaveCurrentScene()
{
	SaveToJSON(mp_CurrentScene);
}

/// <summary>
/// Updates the current scene
/// </summary>
/// <param name="af_deltaTime">Delta time</param>
void SceneManager::Update(float af_deltaTime)
{
	mp_CurrentScene->Update(af_deltaTime);
}

/// <summary>
/// Loads a scene from json file
/// </summary>
/// <param name="as_Path">Path to json file</param>
/// <returns>Returns new scene</returns>
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

	mp_CurrentScene = lp_NewScene;

	// Load objects
	for (const auto& newObject : l_SceneConfig["objects"].items()) {
		// Create Object
		GameObject* lp_newObject = new GameObject();

		// Assign ID
		lp_newObject->SetID(newObject.value()["id"]);
		// Assign Name
		lp_newObject->SetName(newObject.value()["name"]);

		// If it has a Transform Component, add Transform
		if (newObject.value().contains("TRANSFORM")) {
			lp_newObject->GetTransform()->SceneLoad(&newObject.value()["TRANSFORM"]);
		}

		// If it has a SpriteRenderer component, add SpriteRenderer
		if (newObject.value().contains("SPRITERENDERER")) {
			lp_newObject->AddComponent<SpriteRenderer>();
			lp_newObject->GetComponent<SpriteRenderer>()->SceneLoad(&newObject.value()["SPRITERENDERER"]);
		}

		mp_CurrentScene->AddObject(lp_newObject);
	}

	// Go back and assign parent/child hierarchy
	for (const auto& object : l_SceneConfig["objects"].items()) {
		GameObject* lp_currentObject = mp_CurrentScene->GetObjectByID(object.value()["id"]);
		if (object.value()["parent"] != "") {
			GameObject* lp_parentObject = mp_CurrentScene->GetObjectByID(object.value()["parent"]);
			if (lp_parentObject != nullptr) {
				lp_parentObject->AddChild(lp_currentObject);
				lp_currentObject->SetParent(lp_parentObject);
			}
			else {
				// TODO: Log Error
			}
		}
	}

	l_file.close();
	return lp_NewScene;
}


/// <summary>
/// Unloads a scene
/// </summary>
/// <param name="as_SaveToJSON">Set to true if you want to save the scene</param>
void SceneManager::UnloadScene(bool as_SaveToJSON)
{
	if (as_SaveToJSON) {
		SaveToJSON(mp_CurrentScene);
	}

	delete mp_CurrentScene;
}


/// <summary>
/// Saves scene information to JSON file
/// </summary>
/// <param name="ap_Scene">Scene Object</param>
void SceneManager::SaveToJSON(Scene* ap_Scene)
{
	//TODO: REWORK TO NEW DYNAMIC FORMAT
	json l_outfile; // JSON Object to contain the saved data
	std::string l_outfilePath = "SceneConfig/" + ap_Scene->GetSceneID() + ".json";
	int li_totalObjects = ap_Scene->GetSceneObjects().size(); // Number of objects in scene

	l_outfile["sceneName"] = ap_Scene->GetSceneDisplayName();
	l_outfile["sceneID"] = ap_Scene->GetSceneID();
	l_outfile["sceneType"] = ap_Scene->GetSceneType();
	l_outfile["objects"] = {};

	for (int i = 0; i < li_totalObjects; i++) {
		// Get Object ID
		std::string ls_id = ap_Scene->GetObjectByIndex(i)->GetID();

		// Get Parent ID
		std::string ls_parentID = "";
		if (ap_Scene->GetObjectByIndex(i)->GetParent() != nullptr) {
			ls_parentID = ap_Scene->GetObjectByIndex(i)->GetParent()->GetID();
		}

		json l_object = {
			{"id", ls_id},
			{"parent", ls_parentID}
		};

		std::vector<Component*> lp_components = ap_Scene->GetObjectByIndex(i)->GetComponents();
		for (int j = 0; j < lp_components.size(); j++) {
			// Get component Type
			std::string componentType;
			switch (lp_components[j]->GetTag()) {
			case ComponentTypes::TRANSFORM:
				componentType = "TRANSFORM";
				break;
			case ComponentTypes::SPRITERENDERER:
				componentType = "SPRITERENDERER";
				break;
			case ComponentTypes::SPRITE:
				componentType = "SPRITE";
				break;
			case ComponentTypes::RIGIDBODY:
				componentType = "RIGIDBODY";
				break;
			default:
				componentType = "MISSING";
				break;
			}

			json* lp_componentInfo = lp_components[j]->SceneSave();
			if (lp_componentInfo != nullptr) {
				l_object[componentType] = *lp_componentInfo;
			}
		}
		
		l_outfile["objects"] += l_object;
	}

	std::ofstream file(l_outfilePath);
	file << l_outfile;
	file.close();
}


/// <summary>
/// Changes a string to wide string format.
/// </summary>
/// <param name="as_string">String to convert</param>
/// <returns>Returns wstring</returns>
std::wstring SceneManager::stringToWString(std::string as_string)
{
	std::wstring l_outString;
	std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(as_string);
	return l_outString;
}
