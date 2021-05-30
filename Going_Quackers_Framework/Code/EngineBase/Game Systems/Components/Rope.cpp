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
		for (size_t i = 0; i < m_nodeIDs.size(); i++) {
			m_nodes.push_back(SceneManager::GetInstance()->GetCurrentScene()->GetObjectByID(m_nodeIDs[i]));
		}

		mb_checkForNodes = false;
	}

	if (m_nodes.size() == 0)
		return; //TODO make it instead create a rope

	for (size_t i = 0; i < m_nodes.size() - 1; i++)	{
		Vector2 ray = m_nodes[i + 1]->GetTransform()->GetPosition() - m_nodes[i]->GetTransform()->GetPosition();
		std::vector<GameObject*> collisions = Collision::getInstance()->Raycast(ray, m_nodes[i]->GetTransform()->GetPosition());

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

	for (size_t i = 0; i < m_nodePreviousPositions.size(); i++)
		m_nodePreviousPositions[i] = m_nodes[i]->GetTransform()->GetPosition();
}

void Rope::ImGUIUpdate() {
	if (ImGui::Button("Create Rope")) {
		for (size_t i = 0; i < m_nodes.size(); i++) {
			m_nodes[i]->SetToDestroy();
		}
		m_nodes.clear();

		GameObject* baseNode = new GameObject("Rope Node 1");
		GetOwner()->AddChild(baseNode);
		baseNode->GetTransform()->SetLocalPosition(Vector2(0, 0));

		m_nodes.push_back(baseNode);

		GameObject* endNode = new GameObject("Rope Node 2");
		Rigidbody* rigidbody = endNode->AddComponent<Rigidbody>();
		rigidbody->SetType(PhysicsTypes::Trig);

		GetOwner()->AddChild(endNode);
		endNode->GetTransform()->SetLocalPosition(Vector2(0, 0));

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
	Vector2 colliderPosition = ap_collision->GetTransform()->GetPosition();
	Rigidbody* colliderRb = ap_collision->GetComponent<Rigidbody>();

	Vector2 baseNodeOffset = m_nodePreviousPositions[ai_collidingNodeIndex] - colliderPosition;
	Vector2 endNodeOffset = m_nodePreviousPositions[ai_collidingNodeIndex + 1] - colliderPosition;

	//Get closest corner to base node
	Vector2 collidedCorner = Vector2(0, 0);
	collidedCorner += Vector2(std::signbit(baseNodeOffset.X), std::signbit(baseNodeOffset.Y));

	Vector2 closestEndNodeCorner = Vector2(std::signbit(endNodeOffset.X), std::signbit(endNodeOffset.Y));

	if (collidedCorner != closestEndNodeCorner) {
		//If the two options are veritcally aligned
		if (collidedCorner.X == closestEndNodeCorner.X) {
			if (std::abs(baseNodeOffset.X) > std::abs(endNodeOffset.X))
				collidedCorner = closestEndNodeCorner;
		}
		else {
			if (std::abs(baseNodeOffset.Y) > std::abs(endNodeOffset.Y))
				collidedCorner = closestEndNodeCorner;
		}
	}

	//Adjust to collider
	collidedCorner *= colliderRb->GetAABBRect();
	collidedCorner /= 2;
	collidedCorner += colliderPosition;

	Debug::getInstance()->Log(std::to_string(collidedCorner.X) + ", " + std::to_string(collidedCorner.Y));
}

void Rope::SphereCollision(GameObject* ap_collision, int ai_collidingNodeIndex) {
}

GameObject* Rope::GetRopeBase() {
	if (m_nodes.size() == 0)
		return nullptr;

	return m_nodes[0];
}

GameObject* Rope::GetRopeEnd() {
	if (m_nodes.size() == 0)
		return nullptr;

	return m_nodes[m_nodes.size() - 1];
}
