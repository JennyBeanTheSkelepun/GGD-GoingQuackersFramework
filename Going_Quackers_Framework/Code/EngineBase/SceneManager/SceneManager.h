#ifndef SCENEMANAGER
#define SCENEMANAGER

#include "../Game Systems/GameObject.h"
#include "../SceneManager/Scene.h"
#include "../Data Structures/Vectors.h"
#include "../JSON/nlohmann/json.hpp" // Adding JSON for modern C++
#include <vector>

// For Convienience
using json = nlohmann::json;

// Object IDs
enum class ObjectIDs {
	invalidOption,
	debugSquare
};

struct ObjectConfig {
	std::string id;
	Vector2 pos;
	float rotation;
	Vector2 scale;
	std::string texturePath;
	std::string shader;
	std::string shaderPath;
	std::string parentID;
	std::vector<ObjectConfig*> children;
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
	void ChangeScene(std::string as_SceneID);

	void Update(float af_deltaTime);

	Scene* GetCurrentScene() { return mp_CurrentScene; };

private:
	Scene* LoadScene(std::string as_Path);
	void UnloadScene(bool as_SaveToJSON);

	Scene* mp_CurrentScene;

	std::vector<ObjectConfig*> JSONtoConfig(json a_SceneConfig);
	ObjectIDs ObjectIDStringToEnum(std::string as_id);
	void BuildObjects(std::vector<ObjectConfig*> ap_ObjectConfig);
	void SaveToJSON(Scene* ap_Scene);
	std::wstring stringToWString(std::string as_string);
};

#endif /*SCENEMANAGER*/