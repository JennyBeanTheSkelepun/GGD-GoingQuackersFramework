#ifndef _ROPE_H_
#define _ROPE_H_

#include "Component.h"
#include "../../Data Structures/Vectors.h"

class Rope : public Component {
public:
	Rope(GameObject* ap_owner);
	~Rope();

	GameObject* GetRopeBase();
	GameObject* GetRopeEnd();
	inline std::vector<GameObject*> GetRopeNodes() { return m_nodes; }

	void SplitSection(int ai_nodeIndex, Vector2 a_newNodePos = Vector2(0, 0));
	void JoinSection(int ai_nodeIndex);

	void OnDestroy() override;
	void Update() override;
	void ImGUIUpdate() override;
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

private:
	void AABBCollision(GameObject* ap_collision, int ai_collidingNodeIndex);
	void SphereCollision(GameObject* ap_collision, int ai_collidingNodeIndex);

	std::vector<GameObject*> m_nodes;
	std::vector<std::string> m_nodeIDs;
	std::vector<Vector2> m_nodePreviousPositions;
	bool mb_checkForNodes;
};

#endif