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
	void AABBCollision(int ai_collidingNodeIndex);
	void SphereCollision(int ai_collidingNodeIndex);

	std::vector<GameObject*> m_nodes;
	std::vector<std::string> m_nodeIDs;
	bool mb_checkForNodes;
};

#endif