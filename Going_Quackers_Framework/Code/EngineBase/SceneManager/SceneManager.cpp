#include "SceneManager.h"

#include <ostream>
#include <fstream>
#include <filesystem>
#include "../JSON/nlohmann/json.hpp" // Adding JSON for modern C++

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

	// TODO: Objects

		// Create game objects and add to scene
		for (int i = 0; i < sizeof(l_SceneConfig["objects"]); i++) {
			// Load config into object
			objectConfig l_objectConfig;

			// Object ID
			l_objectConfig.id = l_SceneConfig["objects"][i]["id"];

			//Transform variables
			l_objectConfig.pos = Vector2(l_SceneConfig["objects"][i]["pos"][0], l_SceneConfig["objects"][i]["pos"][1]);
			l_objectConfig.rotation = l_SceneConfig["objects"][i]["rotation"];
			l_objectConfig.scale = l_SceneConfig["objects"][i]["scale"];

			// Sprite Rendering variables
			l_objectConfig.texturePath = l_SceneConfig["objects"][i]["spriteRenderer"]["texturePath"];
			//TODO: Colour

			// Build the object
			//BuildObjectFromID(l_objectConfig);
		}

		// Close file
		l_file.close();
		// Return loaded Scene
		return lp_NewScene;
	}
	catch (const std::exception& e) {
	}
}

/* Removes the scene from loaded scenes
*/
void SceneManager::UnloadScene()
{
	delete mp_CurrentScene;
}

/// <summary>
/// Converts Object ID string to Enum
/// </summary>
/// <param name="as_id">String containing ID of an object</param>
/// <returns>Object ID Enum</returns>
ObjectIDs SceneManager::ObjectIDStringToEnum(std::string as_id)
{
	if (as_id == "debugSquare") {
		return debugSquare;
	}
	else {
		return invalidOption;
	}
}

/// <summary>
/// Builds Game Objects from given Enum ID
/// </summary>
/// <param name="a_objectConfig">Enum ID of object</param>
//void SceneManager::BuildObjectFromID(objectConfig a_objectConfig)
//{
//	// Convert String ID to Enum ID
//	ObjectIDs l_ObjectID = ObjectIDStringToEnum(a_objectConfig.id);
//
//	// Determine object type and set it up
//	switch (l_ObjectID) {
//	// Basic Object
//	case debugSquare:
//		GameObject* l_NewDebugSquare = new GameObject();
//		
//		mp_CurrentScene->AddObject(l_NewDebugSquare);
//		break;
//	default:
//		break;
//	}
//}

	// Determine object type and set it up
	//switch (l_ObjectID) {
	//// Basic Object
	//case debugSquare:
	//	GameObject* l_NewDebugSquare = new GameObject();
	//	
	//	mp_CurrentScene->AddObject(l_NewDebugSquare);
	//	break;
	//default:
	//	break;
	//}
//}
