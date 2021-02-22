#pragma once
#include <vector>
#include <string>

#include "../Graphics/GameObject/GameObject.h"

class Scene
{
public:
	Scene(std::string as_ID, std::string as_DisplayName, std::string as_Type);
	~Scene();

	void OnSceneEnter();
	void OnSceneExit();

	void AddObject(GameObject* ap_newObject);

	void Update(float af_deltaTime);
	void Draw();

	std::string GetSceneDisplayName() { return ms_SceneDisplayName; };
	std::string GetSceneID() { return ms_SceneID; };

	std::vector<GameObject*> GetSceneObjects() { return mp_SceneObjects; };

private:
	std::string ms_SceneDisplayName;
	std::string ms_SceneID;
	std::string ms_SceneType;

	std::vector<GameObject*> mp_SceneObjects;
};

