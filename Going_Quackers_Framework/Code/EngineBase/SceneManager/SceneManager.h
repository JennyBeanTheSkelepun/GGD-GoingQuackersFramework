#pragma once

#include "../SceneManager/Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void ChangeScene(std::string as_SceneID);

	void Update(float af_deltaTime);
	void Draw();

	Scene* GetCurrentScene() { return mp_CurrentScene; };

private:
	Scene* LoadScene(std::string as_Path);
	void UnloadScene();

	Scene* mp_CurrentScene;

};

