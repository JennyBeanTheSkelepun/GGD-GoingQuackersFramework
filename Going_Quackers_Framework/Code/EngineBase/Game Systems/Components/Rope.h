#ifndef _ROPE_H_
#define _ROPE_H_

#include "Component.h"

class Rope : public Component {
public:
	Rope(GameObject* ap_owner);
	~Rope();

	GameObject* GetRopeEnd();

	void OnDestroy() override;
	void Update() override;
	void ImGUIUpdate() override;
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

private:
	std::vector<GameObject*> m_nodes;
};

#endif