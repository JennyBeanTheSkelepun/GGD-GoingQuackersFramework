#include "Scene.h"

/// <summary>
/// Scene Constructor
/// </summary>
/// <param name="as_ID">String containing ID of scene</param>
/// <param name="as_DisplayName">String containing display name of scene</param>
/// <param name="as_Type">String containing Type of Scene</param>
Scene::Scene(std::string as_ID, std::string as_DisplayName, std::string as_Type)
{
	ms_SceneID = as_ID;
	ms_SceneDisplayName = as_DisplayName;
	ms_SceneType = as_Type;
}

/// <summary>
/// Scene Deconstructor
/// </summary>
Scene::~Scene()
{
	for (auto lp_object : mp_SceneObjects) {
		delete lp_object;
	}
	mp_SceneObjects.clear();
}

/// <summary>
/// Event to trigger on entering a scene
/// </summary>
void Scene::OnSceneEnter()
{
}

/// <summary>
/// Event to trigger on leaving a scene
/// </summary>
void Scene::OnSceneExit()
{
}

/// <summary>
/// Add an object to the Scene
/// </summary>
/// <param name="ap_newObject">GameObject to add</param>
void Scene::AddObject(GameObject* ap_newObject)
{
	mp_SceneObjects.push_back(ap_newObject);
}

/// <summary>
/// Updates the scene
/// </summary>
/// <param name="af_deltaTime">delta time</param>
void Scene::Update(float af_deltaTime)
{
	for (auto lp_object : mp_SceneObjects) {
		lp_object->Update();
	}
}

/// <summary>
/// Renders the scene
/// </summary>
void Scene::Draw()
{
	for (auto lp_object : mp_SceneObjects) {
		lp_object->Render();
	}
}
