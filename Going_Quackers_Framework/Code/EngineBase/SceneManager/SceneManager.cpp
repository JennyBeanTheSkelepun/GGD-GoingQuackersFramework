#include "SceneManager.h"

#include "../Game Systems/Components/SpriteRenderer.h"
#include "../Game Systems/Components/Physics/Rigidbody.h"
#include "../Game Systems/Components/VirtualCamera.h"
#include "../Game Systems/Components/Player.h"
#include "../Game Systems/Components/AudioSource.h"

#include "../Game Systems/Debug.h"

#include <ostream>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <locale>
#include <sys/stat.h>


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
	mb_doAutoSave = false;
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
	Scene* scene = LoadScene(ls_SceneConfigPath);

	if (scene != nullptr) {
		mp_CurrentScene = scene;
		EngineGuiClass::getInstance()->InitializeObjectList(mp_CurrentScene->GetSceneObjectsList());
	}
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

	EngineGuiClass::getInstance()->InitializeObjectList(mp_CurrentScene->GetSceneObjectsList());
	SaveCurrentScene();
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
	Debug::getInstance()->Log("Loading Scene from path: " + as_Path);

	// Check File Exists
	struct stat buffer;
	if (stat(as_Path.c_str(), &buffer) != 0) {
		Debug::getInstance()->LogError("Could not find SceneConfig File. Please make sure you save your scenes.");
		return nullptr;
	}

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

		lp_newObject->GetTransform()->SceneLoad(&newObject.value()["TRANSFORM"]);

		if (newObject.value().contains("SPRITERENDERER")) {
			LoadComponentFromScene<SpriteRenderer>(lp_newObject, &newObject.value()["SPRITERENDERER"]);
		}

		if (newObject.value().contains("RIGIDBODY")) {
			LoadComponentFromScene<Rigidbody>(lp_newObject, &newObject.value()["RIGIDBODY"]);
		}

		if (newObject.value().contains("PLAYER")) {
			LoadComponentFromScene<Player>(lp_newObject, &newObject.value()["PLAYER"]);
		}

		if (newObject.value().contains("VIRTUALCAMERA")) {
			LoadComponentFromScene<VirtualCamera>(lp_newObject, &newObject.value()["VIRTUALCAMERA"]);
		}

		if (newObject.value().contains("AUDIOSOURCE")) {
			LoadComponentFromScene<AudioSource>(lp_newObject, &newObject.value()["AUDIOSOURCE"]);
		}

		LoadChildren(lp_newObject, &newObject.value());
		mp_CurrentScene->AddObject(lp_newObject);
	}

	Debug::getInstance()->Log("Scene load complete.");
	l_file.close();
	return lp_NewScene;
}

void SceneManager::LoadChildren(GameObject* ap_object, json* ap_json)
{
	for (const auto& child : (*ap_json)["children"].items()) {
		// Create Object
		GameObject* lp_newObject = new GameObject();

		// Assign ID
		lp_newObject->SetID(child.value()["id"]);
		// Assign Name
		lp_newObject->SetName(child.value()["name"]);

		lp_newObject->GetTransform()->SceneLoad(&child.value()["TRANSFORM"]);

		if (child.value().contains("SPRITERENDERER")) {
			LoadComponentFromScene<SpriteRenderer>(lp_newObject, &child.value()["SPRITERENDERER"]);
		}

		if (child.value().contains("RIGIDBODY")) {
			LoadComponentFromScene<Rigidbody>(lp_newObject, &child.value()["RIGIDBODY"]);
		}

		if (child.value().contains("PLAYER")) {
			LoadComponentFromScene<Player>(lp_newObject, &child.value()["PLAYER"]);
		}

		if (child.value().contains("VIRTUALCAMERA")) {
			LoadComponentFromScene<VirtualCamera>(lp_newObject, &child.value()["VIRTUALCAMERA"]);
		}

		if (child.value().contains("AUDIOSOURCE")) {
			LoadComponentFromScene<AudioSource>(lp_newObject, &child.value()["AUDIOSOURCE"]);
		}

		lp_newObject->SetParent(ap_object);
		ap_object->AddChild(lp_newObject);

		LoadChildren(lp_newObject, &child.value());

		mp_CurrentScene->AddObject(lp_newObject);
	}
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

		// Get Object name
		std::string ls_name = ap_Scene->GetObjectByIndex(i)->GetName();

		json l_object = {
			{"id", ls_id},
			{"parent", ls_parentID},
			{"name", ls_name}
		};

		std::vector<Component*> lp_components = ap_Scene->GetObjectByIndex(i)->GetComponents();
		for (int j = 0; j < lp_components.size(); j++) {

			Component* component = lp_components[j];
			// Get component Type
			std::string componentType;
			switch (lp_components[j]->GetType()) {
			case ComponentTypes::TRANSFORM:
				SaveComponent<Transform>("TRANSFORM", component, &componentType);
				break;
			case ComponentTypes::SPRITERENDERER:
				SaveComponent<SpriteRenderer>("SPRITERENDERER", component, &componentType);
				break;
			case ComponentTypes::RIGIDBODY:
				SaveComponent<Rigidbody>("RIGIDBODY", component, &componentType);
				break;
			case ComponentTypes::PLAYER:
				SaveComponent<Player>("PLAYER", component, &componentType);
				break;
			case ComponentTypes::VIRTUALCAMERA:
				SaveComponent<VirtualCamera>("VIRTUALCAMERA", component, &componentType);
				break;
			case ComponentTypes::AUDIOSOURCE:
				SaveComponent<AudioSource>("AUDIOSOURCE", component, &componentType);
				break;
			default:
				componentType = "MISSING";
				break;
			}

			json* lp_componentInfo = component->SceneSave();
			if (lp_componentInfo != nullptr && componentType != "MISSING") {
				l_object[componentType] = *lp_componentInfo;
			}
			else {
				Debug::getInstance()->LogError("Error saving to file, Component Type Error: " + std::string(componentType));
			}

			SaveChildren(ap_Scene->GetObjectByIndex(i), &l_object);
		}

		l_outfile["objects"] += l_object;
	}

	std::ofstream file(l_outfilePath);
	file << l_outfile;
	file.close();
	Debug::getInstance()->Log("Scene Saved: " + ap_Scene->GetSceneDisplayName());
}

void SceneManager::SaveChildren(GameObject* lp_object, json* ap_json)
{
	for (int i = 0; i < lp_object->GetChildren().size(); i++) {
		GameObject* child = lp_object->GetChildren()[i];

			// Get Object ID
		std::string ls_id = child->GetID();

			// Get Parent ID
		std::string ls_parentID = "";
		if (child->GetParent() != nullptr) {
			ls_parentID = child->GetParent()->GetID();
		}

			// Get Object name
		std::string ls_name = child->GetName();

		json l_object = {
			{"id", ls_id},
			{"parent", ls_parentID},
			{"name", ls_name}
		};

		std::vector<Component*> lp_components = child->GetComponents();
		for (int j = 0; j < lp_components.size(); j++) {

			Component* component = lp_components[j];
			// Get component Type
			std::string componentType;
			switch (lp_components[j]->GetType()) {
				case ComponentTypes::TRANSFORM:
					SaveComponent<Transform>("TRANSFORM", component, &componentType);
					break;
				case ComponentTypes::SPRITERENDERER:
					SaveComponent<SpriteRenderer>("SPRITERENDERER", component, &componentType);
					break;
				case ComponentTypes::RIGIDBODY:
					SaveComponent<Rigidbody>("RIGIDBODY", component, &componentType);
					break;
				case ComponentTypes::PLAYER:
					SaveComponent<Player>("PLAYER", component, &componentType);
					break;
				case ComponentTypes::VIRTUALCAMERA:
					SaveComponent<VirtualCamera>("VIRTUALCAMERA", component, &componentType);
					break;
				case ComponentTypes::AUDIOSOURCE:
					SaveComponent<AudioSource>("AUDIOSOURCE", component, &componentType);
					break;
				default:
					componentType = "MISSING";
					break;
			}

			json* lp_componentInfo = component->SceneSave();
			if (lp_componentInfo != nullptr && componentType != "MISSING") {
				l_object[componentType] = *lp_componentInfo;
			}
			else {
				Debug::getInstance()->LogError("Error saving to file, Component Type Error: " + std::string(componentType));
			}
		}

		SaveChildren(child, &l_object);

		(*ap_json)["children"] += l_object;
	}
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