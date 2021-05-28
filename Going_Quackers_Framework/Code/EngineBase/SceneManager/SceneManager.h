#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "../Game Systems/GameObject.h"
#include "../SceneManager/Scene.h"
#include "../Data Structures/Vectors.h"
#include <vector>

#include "../JSON/nlohmann/json.hpp" // Adding JSON for modern C++
// For Convienience
using json = nlohmann::json;

struct ObjectConfig {
	std::string id;
	Vector2 pos;
	float rotation;
	Vector2 scale;
	std::string texturePath;
	std::string shaderPath;
	std::string parentID;
};

class SceneManager
{
/* Singleton Stuff */
public:
	static SceneManager* GetInstance();
	SceneManager(SceneManager const&) = delete;
	void operator = (SceneManager const&) = delete;
protected:
	SceneManager();
	static SceneManager* mp_instance;
/* Singleton Stuff End */

public:
	~SceneManager();

	void Initialize();
	void ChangeScene(std::string as_SceneID, bool as_SaveToJSON);
	void NewScene(std::string as_SceneID, std::string as_SceneName, std::string as_SceneType, bool as_SaveToJSON);
	void SaveCurrentScene();

	void Update(float af_deltaTime);

	inline Scene* GetCurrentScene() { return mp_CurrentScene; };

	inline bool GetAutoSave() { return mb_doAutoSave; }
	inline void SetAutoSave(bool value) { mb_doAutoSave = value; }

private:
	Scene* mp_CurrentScene;
	bool mb_doAutoSave;

	Scene* LoadScene(std::string as_Path);
	void LoadChildren(GameObject* ap_object, json* ap_json);
	void UnloadScene(bool as_SaveToJSON);

	void SaveToJSON(Scene* ap_Scene);
	void SaveChildren(GameObject* lp_object, json* ap_json);
	std::wstring stringToWString(std::string as_string);

	template<typename T>
	void LoadComponentFromScene(GameObject* ap_object, json* ap_json) {
		ap_object->AddComponent<T>();
		ap_object->GetComponent<T>()->SceneLoad(ap_json);
	}

	template<typename T>
	void SaveComponent(std::string as_componentName, Component* ap_component, std::string* ap_componentType) {
		*ap_componentType = as_componentName;
		ap_component = static_cast<T*>(ap_component);
	}
};

#endif /*_SCENEMANAGER_H_*/