#ifndef _SAVE_MANAGER_H_
#define _SAVE_MANAGER_H_

#include "../JSON/nlohmann/json.hpp"
#include "../Game Systems/GameObject.h"

class SaveManager {
public:
	static SaveManager* getInstance();

	SaveManager(SaveManager const&) = delete;
	void operator = (SaveManager const&) = delete;

private:
	static SaveManager* SingletonInstance;
	SaveManager();

public:
	void LoadFromFile();
	void SaveFromObjects();
	void LoadToObjects();
	void AddToSaving(GameObject* ap_gameObject);
	void RemoveFromSaving(GameObject* ap_gameObject);
private:
	std::vector<GameObject*> m_saveObjects;
	nlohmann::json m_save;
};

#endif // !_SAVE_MANAGER_H_
