#include "SceneManager.h"

#include "../Game Systems/Components/SpriteRenderer.h"

#include <ostream>
#include <fstream>
#include <filesystem>


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
	UnloadScene();

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

	// Add Objects
	ObjectConfig l_ObjectConfig;

	for (const auto& object : l_SceneConfig["objects"].items()) {
		l_ObjectConfig.pos = Vector2(object.value()["position"][0], object.value()["position"][1]);
		l_ObjectConfig.scale = Vector2(object.value()["scale"][0], object.value()["scale"][1]);
		l_ObjectConfig.rotation = object.value()["rotation"];
	}

	l_file.close();
	return lp_NewScene;
}

/// <summary>
/// Unloads a scene
/// </summary>
void SceneManager::UnloadScene()
{
	delete mp_CurrentScene;
}

/// <summary>
/// Creates ObjectConfigs from JSON file
/// </summary>
/// <param name="a_SceneConfig"></param>
/// <returns>Returns vector of object configs</returns>
std::vector<ObjectConfig*> SceneManager::JSONtoConfig(json a_SceneConfig)
{
	std::vector<ObjectConfig*> l_configs;
	for (const auto& object : a_SceneConfig["objects"].items()) {

		ObjectConfig* lp_newObjectConfig = new ObjectConfig();
		// Assign values from json
		lp_newObjectConfig->id = object.value()["id"];
		lp_newObjectConfig->pos = Vector2(object.value()["position"][0], object.value()["position"][1]);
		lp_newObjectConfig->rotation = object.value()["rotation"];
		lp_newObjectConfig->scale = Vector2(object.value()["scale"][0], object.value()["scale"][1]);
		lp_newObjectConfig->texturePath = object.value()["texturePath"];
		lp_newObjectConfig->shader = object.value()["shader"];
		lp_newObjectConfig->shaderPath = object.value()["shaderPath"];
		lp_newObjectConfig->parentID = object.value()["parent"];
		// Add to vector
		l_configs.push_back(lp_newObjectConfig);
	}
	// Assign children to parents
	for (const auto& l_config : l_configs) {
		if (l_config->parentID != "") {
			for (int i = 0; i < l_configs.size(); i++) {
				if (l_config->parentID == l_configs[i]->id) {
					l_configs[i]->children.push_back(l_config);
					break;
				}
			}
		}
	}
	return l_configs;
}

/// <summary>
/// Converts Object ID string to Enum
/// </summary>
/// <param name="as_id">String containing ID of an object</param>
/// <returns>Object ID Enum</returns>
ObjectIDs SceneManager::ObjectIDStringToEnum(std::string as_id)
{
	if (as_id == "debugSquare") {
		return ObjectIDs::debugSquare;
	}
	else {
		return ObjectIDs::invalidOption;
	}
}

/// <summary>
/// Builds objects from ObjectConfig objects
/// </summary>
/// <param name="ap_ObjectConfig"></param>
void SceneManager::BuildObjects(std::vector<ObjectConfig*> ap_ObjectConfig)
{
	// Build and add objects
	for (const auto& l_objectConfig : ap_ObjectConfig) {
		GameObject* lp_newObject = new GameObject();
		lp_newObject->SetID(l_objectConfig->id);
		lp_newObject->GetTransform()->SetLocalPosition(l_objectConfig->pos);
		lp_newObject->GetTransform()->SetLocalRotation(l_objectConfig->rotation);
		lp_newObject->GetTransform()->SetLocalScale(l_objectConfig->scale);
		lp_newObject->AddComponent<SpriteRenderer>();
		// TODO Texture Stuff
		// TODO Shader Stuff
		mp_CurrentScene->AddObject(lp_newObject);
	}
	// Sort Parent/child hierarchy

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
		std::string ls_texPath = ""; // TODO: Find a way to get texture paths

		// Get Shader Info
		std::string ls_shader = ""; // TODO: Talk to David about Shader stuff
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
				{"shader", ls_shader},
				{"shaderPath", ls_shaderPath},
				{"parent", ls_parentID}
			};
	}

	std::ofstream file(l_outfilePath);
	file << l_outfile;
	file.close();
}
