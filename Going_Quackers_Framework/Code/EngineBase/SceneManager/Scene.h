#ifndef SCENE
#define SCENE

#include <vector>
#include <string>

class GameObject;

class Scene
{
public:
	Scene(std::string as_ID, std::string as_DisplayName, std::string as_Type);
	~Scene();

	void OnSceneEnter();
	void OnSceneExit();

	void AddObject(GameObject* ap_newObject);

	void Update(float af_deltaTime);

	std::string GetSceneDisplayName() { return ms_SceneDisplayName; };
	std::string GetSceneID() { return ms_SceneID; };
	std::string GetSceneType() { return ms_SceneType; };

	void SetSceneDisplayName(std::string as_SceneDisplayName) { ms_SceneDisplayName = as_SceneDisplayName; };
	void SetSceneID(std::string as_SceneID) { ms_SceneID = as_SceneID; };
	void SetSceneType(std::string as_SceneType) { ms_SceneType = as_SceneType; };

	std::vector<GameObject*> GetSceneObjects() { return mp_SceneObjects; };
	GameObject* GetObjectByIndex(int ai_index) { return mp_SceneObjects[ai_index]; };
	GameObject* GetObjectByID(std::string as_id);

private:
	std::string ms_SceneDisplayName;
	std::string ms_SceneID;
	std::string ms_SceneType;

	std::vector<GameObject*> mp_SceneObjects;
};

#endif /*SCENE*/