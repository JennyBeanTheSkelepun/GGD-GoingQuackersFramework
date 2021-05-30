#include "Rope.h"
#include "../Debug.h"
#include "SpringJoint.h"
#include "Physics/Rigidbody.h"
#include <cmath>

Rope::Rope(GameObject* ap_owner) : Component(ap_owner, ComponentTypes::ROPE, "Rope") {
	mb_checkForNodes = false;
}

Rope::~Rope() {
}

void Rope::OnDestroy() {
	this->~Rope();
}

void Rope::Update() {
	if (mb_checkForNodes) {
		/*for (size_t i = 0; i < m_nodeIDs.size(); i++) {
			m_nodes.push_back(SceneManager::GetInstance()->GetCurrentScene()->GetObjectByID(m_nodeIDs[i]));
		}*/

		mb_checkForNodes = false;
	}

	if (m_nodes.size() == 0)
		return; //TODO make it instead create a rope

	for (size_t i = 0; i < m_nodes.size() - 1; i++)	{
		Vector2 ray = m_nodes[i + 1]->GetTransform()->GetPosition() - m_nodes[i]->GetTransform()->GetPosition();
		std::vector<GameObject*> collisions = Collision::getInstance()->Raycast(ray, m_nodes[i + 1]->GetTransform()->GetPosition());

		for (size_t j = 0; j < collisions.size(); j++) {
			Rigidbody* rigidbody = collisions[j]->GetComponent<Rigidbody>();
			if (rigidbody->GetType() != PhysicsTypes::RB)
				continue;
			
			if (rigidbody->GetCollisionType() == CollisionTypes::AABB) {
				AABBCollision(collisions[j], i);
			}
			else if (rigidbody->GetCollisionType() == CollisionTypes::Sphere)
				SphereCollision(collisions[j], i);
		}
	}
}

void Rope::ImGUIUpdate() {
	if (ImGui::Button("Create Rope")) {
		for (size_t i = 0; i < m_nodes.size(); i++) {
			m_nodes[i]->SetToDestroy();
		}
		m_nodes.clear();

		GameObject* baseNode = new GameObject("Rope Node 1");
		GetOwner()->AddChild(baseNode);
		//baseNode->GetTransform()->SetLocalPosition(Vector2(0, 0));
		baseNode->GetTransform()->SetLocalPosition(Vector2(0, 2)); //temp

		m_nodes.push_back(baseNode);

		GameObject* endNode = new GameObject("Rope Node 2");
		Rigidbody* rigidbody = endNode->AddComponent<Rigidbody>();
		rigidbody->SetType(PhysicsTypes::Trig);

		GetOwner()->AddChild(endNode);
		//endNode->GetTransform()->SetLocalPosition(Vector2(0, 0));
		endNode->GetTransform()->SetLocalPosition(Vector2(3, 0)); //temp

		m_nodes.push_back(endNode);

		SpringJoint* springJoint = baseNode->AddComponent<SpringJoint>();
		springJoint->SetConnectedObject(endNode);
		springJoint->SetSpringMode(SpringMode::REPEL_ONLY);
		springJoint->SetSelfAdjustDesiredLength(true);
	}

	if (ImGui::Button("Destroy Rope")) {
		for (size_t i = 0; i < m_nodes.size(); i++) {
			m_nodes[i]->SetToDestroy();
		}
		m_nodes.clear();
	}
}

json* Rope::SceneSave() {
	std::vector<std::string> nodeIDs;

	for (size_t i = 0; i < m_nodes.size(); i++)	{
		nodeIDs.push_back(m_nodes[i]->GetID());
	}

	json* returnObj = new json({
		{"Nodes", nodeIDs}
	});

	return returnObj;
}

void Rope::SceneLoad(json* componentJSON) {
	//breaks when set directly for some reason
	std::vector<std::string> nodeIDs = (*componentJSON)["Nodes"];
	m_nodeIDs = nodeIDs;

	mb_checkForNodes = true;
}

void Rope::AABBCollision(GameObject* ap_collision, int ai_collidingNodeIndex) {
	Vector2 baseNodeOffset = m_nodes[ai_collidingNodeIndex]->GetTransform()->GetPosition() - ap_collision->GetTransform()->GetPosition();

	bool xAlignedBaseNode = std::abs(baseNodeOffset.X) > std::abs(baseNodeOffset.Y);
	Vector2 closestEdge = xAlignedBaseNode ? Vector2(std::signbit(baseNodeOffset.X), 0) : Vector2(0, std::signbit(baseNodeOffset.Y));

	Debug::getInstance()->Log(std::to_string(closestEdge.X) + ", " + std::to_string(closestEdge.Y));
}

void Rope::SphereCollision(GameObject* ap_collision, int ai_collidingNodeIndex) {
}

GameObject* Rope::GetRopeEnd() {
	return nullptr;
}
