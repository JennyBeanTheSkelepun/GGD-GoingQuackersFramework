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
		for (auto lp_component : lp_object->GetComponents()) {
			lp_component->ShouldDestroy();
		}
		lp_object->ShouldDestroy();
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

void Scene::DeleteObjectAtIndex(int ai_index)
{
	GameObject* gameObject = mp_SceneObjects[ai_index];
	mp_SceneObjects.erase(mp_SceneObjects.begin() + ai_index);
	delete gameObject;
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
			// Check Children
			GameObject* child = GetChildObjectByID(as_id, mp_SceneObjects[i]);
			if (child != nullptr) {
				return child;
			}
			continue;
		}
	}
	// If not found, return nullptr
	return nullptr;
}

GameObject* Scene::GetObjectByName(std::string as_name)
{
	// Search for object with ID
	for (int i = 0; i < mp_SceneObjects.size(); i++) {
		// Return if ID matches
		if (mp_SceneObjects[i]->GetID() == as_name) {
			return mp_SceneObjects[i];
		}
		else {
			// Check Children
			GameObject* child = GetChildObjectByID(as_name, mp_SceneObjects[i]);
			if (child != nullptr) {
				return child;
			}
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

GameObject* Scene::GetChildObjectByName(std::string as_name, GameObject* ap_parent)
{
	// Search for object with ID
	for (int i = 0; i < ap_parent->GetChildren().size(); i++) {
		// Return if ID matches
		if (ap_parent->GetChildren()[i]->GetID() == as_name) {
			return ap_parent->GetChildren()[i];
		}
		else {
			// Check Children
			GameObject* child = GetChildObjectByID(as_name, ap_parent->GetChildren()[i]);
			if (child != nullptr) {
				return child;
			}
			continue;
		}
	}
	// If not found, return nullptr
	return nullptr;
}

GameObject* Scene::GetChildObjectByID(std::string as_id, GameObject* ap_parent)
{
	// Search for object with ID
	for (int i = 0; i < ap_parent->GetChildren().size(); i++) {
		// Return if ID matches
		if (ap_parent->GetChildren()[i]->GetID() == as_id) {
			return ap_parent->GetChildren()[i];
		}
		else {
			// Check Children
			GameObject* child = GetChildObjectByID(as_id, ap_parent->GetChildren()[i]);
			if (child != nullptr) {
				return child;
			}
			continue;
		}
	}
	// If not found, return nullptr
	return nullptr;
}
