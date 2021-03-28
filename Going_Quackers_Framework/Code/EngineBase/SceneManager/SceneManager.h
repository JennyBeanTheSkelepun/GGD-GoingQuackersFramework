#pragma once

#ifndef SCENEMANAGER
#define SCENEMANAGER

#include "../SceneManager/Scene.h"
#include "../Data Structures/Vectors.h"

// Object IDs
enum ObjectIDs {
	invalidOption,
	debugSquare
};

struct objectConfig {
	std::string id;
	Vector2 pos;
	float rotation;
	float scale;
	// Vector3 colour
	std::string texturePath;
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

	void ChangeScene(std::string as_SceneID);

	void Update(float af_deltaTime);
	void Draw();

	Scene* GetCurrentScene() { return mp_CurrentScene; };

private:
	Scene* LoadScene(std::string as_ID);
	void UnloadScene();
	ObjectIDs ObjectIDStringToEnum(std::string as_id);
	//void BuildObjectFromID(objectConfig a_objectConfig);

	Scene* mp_CurrentScene;
};

#endif SCENEMANAGER
