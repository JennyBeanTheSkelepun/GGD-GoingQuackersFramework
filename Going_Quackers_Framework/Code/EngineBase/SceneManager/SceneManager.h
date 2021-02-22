#pragma once

#include "../SceneManager/Scene.h"

// Object IDs
enum ObjectIDs {
	invalidOption,
	debugSquare
};

class SceneManager
{
protected:
	
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
	ObjectIDs ObjectIDStringToEnum(std::string as_id);
	void BuildObjectFromID(std::string as_id, float af_startX, float af_startY);

	Scene* mp_CurrentScene;

};

