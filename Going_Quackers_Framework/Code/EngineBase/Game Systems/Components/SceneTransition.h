#ifndef _SCENETRANSITION_H_
#define _SCENETRANSITION_H_

#include "Component.h"
#include <string>

class SceneTransition : public Component
{
public:
	SceneTransition(GameObject* owner);
	~SceneTransition();

	//- Basic Loops -//
	void OnDestroy() override;
	void Update() override;

	//- ImGui UpdateLoop -//
	void ImGUIUpdate() override;

	//- Scene Save and Load -//

	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

	inline std::string GetNextSceneID() { return ""; }
	inline void SetNextSceneID() { ""; }

private:
	std::string m_NextSceneID;
	char* m_SceneIDInput;
};

#endif