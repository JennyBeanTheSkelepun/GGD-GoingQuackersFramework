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
void SceneManager::ChangeScene(std::string as_SceneID)
{
	// Unload current Scene
	UnloadScene(false);

	// Get path to JSON scene config
	std::string ls_SceneConfigPath = "";

	// Load new Scene
	mp_CurrentScene = LoadScene(ls_SceneConfigPath);
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

	// Load objects
	for (const auto& newObject : l_SceneConfig["objects"].items()) {
		// Create Object
		GameObject* lp_newObject = new GameObject();

		// Assign ID
		lp_newObject->SetID(newObject.value()["id"]);
		// Assign Name
		lp_newObject->SetName(newObject.value()["name"]);

		// If it has a Transform Component, add Transform
		if (newObject.value().contains("Transform")) {
			lp_newObject->GetTransform()->SceneLoad(&newObject.value()["Transform"]);
		}

		// If it has a SpriteRenderer component, add SpriteRenderer
		if (newObject.value().contains("SpriteRenderer")) {
			lp_newObject->AddComponent<SpriteRenderer>();
			lp_newObject->GetComponent<SpriteRenderer>()->SceneLoad(&newObject.value()["SpriteRenderer"]);
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
		
		// Get Object Position
		float l_pos[2]; 
		l_pos[0] = ap_Scene->GetObjectByIndex(i)->GetTransform()->GetLocalPosition().X;
		l_pos[1] = ap_Scene->GetObjectByIndex(i)->GetTransform()->GetLocalPosition().Y;

		// Get Object Scale
		float lf_scale[2]; 
		lf_scale[0] = ap_Scene->GetObjectByIndex(i)->GetTransform()->GetLocalScale().X;
		lf_scale[1] = ap_Scene->GetObjectByIndex(i)->GetTransform()->GetLocalScale().Y;

		// Get Object Rotation
		float l_rot = ap_Scene->GetObjectByIndex(i)->GetTransform()->GetLocalRotation();

		// Get Path To Texture
		std::string ls_texPath = "";

		// Get Shader Info
		std::string ls_shaderPath = "";

		// Get Parent ID
		std::string ls_parentID = "";
		if (ap_Scene->GetObjectByIndex(i)->GetParent() != nullptr) {
			ls_parentID = ap_Scene->GetObjectByIndex(i)->GetParent()->GetID();
		}

		// Add Object Info to config
		l_outfile["objects"] += {
				{"id", ls_id},
				{"position", l_pos},
				{"rotation", l_rot},
				{"scale", lf_scale},
				{"texturePath", ls_texPath},
				{"shaderPath", ls_shaderPath},
				{"parent", ls_parentID}
			};
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
