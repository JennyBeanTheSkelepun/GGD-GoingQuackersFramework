#include "Scene.h"

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
	for (auto lp_object : mp_SceneObjects) {
		lp_object->Update();
	}
}

void Scene::Draw()
{
	for (auto lp_object : mp_SceneObjects) {
		lp_object->Render();
	}
}
