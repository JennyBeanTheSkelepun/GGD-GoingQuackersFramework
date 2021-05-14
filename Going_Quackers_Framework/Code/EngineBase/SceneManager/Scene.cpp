#include "Scene.h"
#include "../Game Systems/GameObject.h"

Scene::Scene(std::string as_ID, std::string as_DisplayName, std::string as_Type)
{
	ms_SceneID = as_ID;
	ms_SceneDisplayName = as_DisplayName;
	ms_SceneType = as_Type;
}

Scene::~Scene()
{
	for (auto lp_object : mp_SceneObjects) {
		delete lp_object;
	}
	mp_SceneObjects.clear();
}

void Scene::OnSceneEnter()
{
}

void Scene::OnSceneExit()
{
}

void Scene::AddObject(GameObject* ap_newObject)
{
	mp_SceneObjects.push_back(ap_newObject);
}

void Scene::Update(float af_deltaTime)
{
	for (int i = 0; i < mp_SceneObjects.size(); i++) {
		mp_SceneObjects[i]->Update();
	}
}

GameObject* Scene::GetObjectByID(std::string as_id)
{
	// Search for object with ID
	for (int i = 0; i < mp_SceneObjects.size(); i++) {
		// Return if ID matches
		if (mp_SceneObjects[i]->GetID() == as_id) {
			return mp_SceneObjects[i];
		}
		else {
			continue;
		}
	}
	// If not found, return nullptr
	return nullptr;
}

int Scene::GetObjectIndexByID(std::string as_id)
{
	// Search for object with ID
	for (int i = 0; i < mp_SceneObjects.size(); i++) {
		// Return if ID matches
		if (mp_SceneObjects[i]->GetID() == as_id) {
			return i;
		}
		else {
			continue;
		}
	}
	// If not found, return -1
	return -1;
}
