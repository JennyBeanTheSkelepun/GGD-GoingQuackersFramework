#include "SaveManager.h"
#include <iostream>
#include <fstream>

SaveManager* SaveManager::SingletonInstance = 0;

SaveManager* SaveManager::getInstance() {
	if (SingletonInstance == 0)
		SingletonInstance = new SaveManager();

	return SingletonInstance;
}

void SaveManager::LoadFromFile() {
	std::ifstream filestream("Game Save Config/Save.json");
	filestream >> m_save;
}

void SaveManager::SaveFromObjects() {
	m_save = NULL;

	for (size_t i = 0; i < m_saveObjects.size(); i++) {
		//ToDo: possibility for crash if gameObject does't exist.
		/*if (saveObjects[i] == nullptr) {
			continue;
		}*/
		
		for (Component* component : m_saveObjects[i]->GetComponents())
			*component->GameSave(&m_save, i);
	}
}

void SaveManager::LoadToObjects() {
	for (size_t i = 0; i < m_saveObjects.size(); i++) {
		if (m_saveObjects[i] == nullptr) {
			//ToDo: create empty object
			continue;
		}

		for (Component* component : m_saveObjects[i]->GetComponents())
			component->GameLoad(&m_save);
	}
}

void SaveManager::AddToSaving(GameObject* ap_gameObject) {
	m_saveObjects.push_back(ap_gameObject);
}

void SaveManager::RemoveFromSaving(GameObject* ap_gameObject) {
	for (size_t i = 0; i < m_saveObjects.size(); i++) {
		if (ap_gameObject == m_saveObjects[i]) {
			m_saveObjects.erase(m_saveObjects.begin() + i);
			break;
		}
	}
}