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
			m_nodePreviousPositions.push_back(m_nodes[i]->GetTransform()->GetPosition());
		}

		mb_checkForNodes = false;
	}

	if (m_nodes.size() == 0)
		return; //TODO make it instead create a rope

	for (size_t i = 0; i < m_nodes.size() - 1; i++)	{
		Vector2 nodePosition = m_nodes[i]->GetTransform()->GetPosition();

		Vector2 ray = m_nodes[i + 1]->GetTransform()->GetPosition() - nodePosition;
		std::vector<GameObject*> collisions = Collision::getInstance()->Raycast(ray, nodePosition);

		//Remove non physical colliders
		for (size_t j = 0; j < collisions.size(); j++) {
			if (collisions[j]->GetComponent<Rigidbody>()->GetType() == PhysicsTypes::RB)
				continue;

			collisions.erase(collisions.begin() + j);
			j--;
		}

		//Join sections where the rope has straightened out
		if (collisions.size() == 0 && i != m_nodes.size() - 2) {
			Vector2 directionToNextNode = (m_nodes[i + 1]->GetTransform()->GetPosition() - nodePosition).Normalize();
			Vector2 directionToNodeAfterNext = (m_nodes[i + 2]->GetTransform()->GetPosition() - nodePosition).Normalize();

			if (directionToNextNode == directionToNodeAfterNext) {
				JoinSection(i + 1);
				//i--;
				//continue;
				return;
			}
		}

		//Handle collisions
		for (size_t j = 0; j < collisions.size(); j++) {
			if (m_nodes.size() > 2) {
				Debug::getInstance()->LogError("Somethings gone wrong and there's too many nodes"); //remove when this is no longer a sign of infinite points adding
				return;
			}

			Rigidbody* rigidbody = collisions[j]->GetComponent<Rigidbody>();
			if (rigidbody->GetType() != PhysicsTypes::RB)
				continue;
			
			if (rigidbody->GetCollisionType() == CollisionTypes::AABB) {
				AABBCollision(collisions[j], i);
				return; //temp
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
		m_nodePreviousPositions.push_back(baseNode->GetTransform()->GetPosition());

		GameObject* endNode = new GameObject("Rope Node 2");
		Rigidbody* rigidbody = endNode->AddComponent<Rigidbody>();
		rigidbody->SetType(PhysicsTypes::Trig);

		GetOwner()->AddChild(endNode);
		endNode->GetTransform()->SetLocalPosition(Vector2(0, 0));

		m_nodes.push_back(endNode);
		m_nodePreviousPositions.push_back(endNode->GetTransform()->GetPosition());

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

	Vector2 diagonalDirection = Vector2(-std::signbit(endNodeOffset.X - baseNodeOffset.X) * 2 + 1, -std::signbit(endNodeOffset.Y - baseNodeOffset.Y) * 2 + 1);

	bool isToRightOfCollider = (endNodeOffset.X - baseNodeOffset.X) * (colliderPosition.Y - baseNodeOffset.Y) > (endNodeOffset.Y - baseNodeOffset.Y) * (colliderPosition.X - baseNodeOffset.X);

	Vector2 collidedCorner = isToRightOfCollider ? Vector2(diagonalDirection.Y, -diagonalDirection.X) : Vector2(-diagonalDirection.Y, diagonalDirection.X);

	/*
	////Prevent rounding to the wrong closest corner
	//if (baseNodeOffset.X == 0)
	//	baseNodeOffset.X += endNodeOffset.X > 0 ? 0.0001f : -0.0001f;

	//if (baseNodeOffset.Y == 0)
	//	baseNodeOffset.Y += endNodeOffset.Y > 0 ? 0.0001f : -0.0001f;

	//if (endNodeOffset.X == 0)
	//	endNodeOffset.X += baseNodeOffset.X > 0 ? 0.0001f : -0.0001f;

	//if (endNodeOffset.Y == 0)
	//	endNodeOffset.Y += baseNodeOffset.Y > 0 ? 0.0001f : -0.0001f;

	////Get closest corner to base node
	//Vector2 collidedCorner = Vector2(0, 0);
	//collidedCorner += Vector2(-std::signbit(baseNodeOffset.X) * 2 + 1, -std::signbit(baseNodeOffset.Y) * 2 + 1);

	//Vector2 closestEndNodeCorner = Vector2(-std::signbit(endNodeOffset.X) * 2 + 1, -std::signbit(endNodeOffset.Y) * 2 + 1);

	//if (collidedCorner != closestEndNodeCorner) {
	//	//If the two options are veritcally aligned
	//	if (collidedCorner.X == closestEndNodeCorner.X) {
	//		if (std::abs(baseNodeOffset.X) > std::abs(endNodeOffset.X))
	//			collidedCorner = closestEndNodeCorner;
	//	}
	//	else {
	//		if (std::abs(baseNodeOffset.Y) > std::abs(endNodeOffset.Y))
	//			collidedCorner = closestEndNodeCorner;
	//	}
	//}
	*/

	//Adjust to collider
	collidedCorner *= colliderRb->GetAABBRect();
	collidedCorner /= 2;
	collidedCorner += colliderPosition;

	Debug::getInstance()->Log(std::to_string(collidedCorner.X) + ", " + std::to_string(collidedCorner.Y));

	SplitSection(ai_collidingNodeIndex, collidedCorner);
}

void Rope::SphereCollision(GameObject* ap_collision, int ai_collidingNodeIndex) {
}

void Rope::SplitSection(int ai_nodeIndex, Vector2 a_newNodePos) {
	GameObject* newNode = new GameObject(("Rope Node " + std::to_string(m_nodes.size() + 1)).c_str());
	GetOwner()->AddChild(newNode);
	newNode->GetTransform()->SetLocalPosition(a_newNodePos);

	Rigidbody* rigidbody = newNode->AddComponent<Rigidbody>();
	rigidbody->SetType(PhysicsTypes::Trig);

	m_nodes[ai_nodeIndex]->GetComponent<SpringJoint>()->SetConnectedObject(newNode);

	SpringJoint* springJoint = newNode->AddComponent<SpringJoint>();
	springJoint->SetConnectedObject(m_nodes[ai_nodeIndex + 1]);
	springJoint->SetSpringMode(SpringMode::REPEL_ONLY);
	springJoint->SetSelfAdjustDesiredLength(true);

	m_nodes.insert(m_nodes.begin() + ai_nodeIndex + 1, newNode);
	m_nodePreviousPositions.insert(m_nodePreviousPositions.begin() + ai_nodeIndex + 1, newNode->GetTransform()->GetPosition());
}

void Rope::JoinSection(int ai_nodeIndex) {
	m_nodes[ai_nodeIndex - 1]->GetComponent<SpringJoint>()->SetConnectedObject(m_nodes[ai_nodeIndex + 1]);
	m_nodes[ai_nodeIndex]->SetToDestroy();

	m_nodes.erase(m_nodes.begin() + ai_nodeIndex);
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
