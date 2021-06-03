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

	if (m_nodes.size() < 2)
		return; //TODO make it instead create a rope

	for (size_t i = 0; i < m_nodes.size() - 1; i++)	{
		Vector2 nodePosition = m_nodes[i]->GetTransform()->GetPosition();

		Vector2 ray = m_nodes[i + 1]->GetTransform()->GetPosition() - nodePosition;
		std::vector<GameObject*> collisions = Collision::getInstance()->Raycast(ray, nodePosition);

		//Sort through collisions
		GameObject* closestCollider = nullptr;
		float rayLength = ray.Length();
		float closestColliderDistance = rayLength;
		
		for (size_t j = 0; j < collisions.size(); j++) {
			if (collisions[j]->GetComponent<Rigidbody>()->GetType() != PhysicsTypes::RB)
				continue;

			float collisionDistance = nodePosition.Distance(collisions[j]->GetTransform()->GetPosition());

			if (collisionDistance > rayLength)
				continue;

			Vector2 collisionPoint = GetAABBCollisionPoint(collisions[j], i);

			if (i == 1)
				Debug::getInstance()->Log(collisionPoint);

			if (m_nodes[i + 1]->GetTransform()->GetPosition() == collisionPoint || nodePosition == collisionPoint)
				continue;

			if (collisionDistance < closestColliderDistance) {
				closestCollider = collisions[j];
				closestColliderDistance = collisionDistance;
			}
		}

		//Join sections where the rope has straightened out
		if (closestCollider == nullptr) {
			if (i != m_nodes.size() - 2 && IsNodeBendRightward(i + 1) != m_nodeRightwardBend[i]) {
				JoinSection(i + 1);
				i--;
			}

			continue;
		}

		//Handle collisions
		if (m_nodes.size() > 4) {
			Debug::getInstance()->LogError("Somethings gone wrong and there's too many nodes"); //remove when this is no longer a sign of infinite points adding
			return;
		}

		Rigidbody* rigidbody = closestCollider->GetComponent<Rigidbody>();
			
		if (rigidbody->GetCollisionType() == CollisionTypes::AABB) {
			SplitSection(i, GetAABBCollisionPoint(closestCollider, i));
		}
		else if (rigidbody->GetCollisionType() == CollisionTypes::Sphere)
			GetSphereCollisionPoint(closestCollider, i);
		
		//Handle line renderer
		if (m_nodes[i]->GetTransform()->GetPosition() != m_nodePreviousPositions[i]) {
			m_nodes[i]->GetComponent<LineRenderer>()->SetStartPos(nodePosition);

			if (i != 0)
				m_nodes[i - 1]->GetComponent<LineRenderer>()->SetEndPos(nodePosition);
		}

		i--;
	}

	//Update last part of line renderer chain
	if (m_nodes[m_nodes.size() - 1]->GetTransform()->GetPosition() != m_nodePreviousPositions[m_nodes.size() - 1])
		m_nodes[m_nodes.size() - 2]->GetComponent<LineRenderer>()->SetEndPos(m_nodes[m_nodes.size() - 1]->GetTransform()->GetPosition());

	//Update all previous positions
	for (size_t i = 0; i < m_nodePreviousPositions.size(); i++)
		m_nodePreviousPositions[i] = m_nodes[i]->GetTransform()->GetPosition();
}

void Rope::ImGUIUpdate() {
	if (ImGui::Button("Create Rope")) {
		for (size_t i = 0; i < m_nodes.size(); i++) {
			m_nodes[i]->SetToDestroy();
		}
		m_nodes.clear();

		GameObject* baseNode = new GameObject("Rope Node 1", GetOwner());
		GetOwner()->AddChild(baseNode);
		//baseNode->GetTransform()->SetLocalPosition(Vector2(0, 0));
		baseNode->GetTransform()->SetLocalPosition(Vector2(-2, 0.25f)); //temp

		//Force world positition update to work correctly
		//baseNode->GetTransform()->GetLocalToWorldMatrix();

		Vector2 baseNodePosition = baseNode->GetTransform()->GetPosition();

		LineRenderer* lineRenderer = baseNode->AddLineRendererComponent();
		lineRenderer->SetTexture("Assets/rope.tga");
		lineRenderer->SetWidth(1);

		m_nodes.push_back(baseNode);
		m_nodePreviousPositions.push_back(baseNodePosition);

		GameObject* endNode = new GameObject("Rope Node 2", GetOwner());
		Rigidbody* rigidbody = endNode->AddComponent<Rigidbody>();
		rigidbody->SetType(PhysicsTypes::Trig);

		GetOwner()->AddChild(endNode);
		//endNode->GetTransform()->SetLocalPosition(Vector2(0.5f, 0));
		endNode->GetTransform()->SetLocalPosition(Vector2(2, 2)); //temp

		//Force world positition update to work correctly
		//endNode->GetTransform()->GetLocalToWorldMatrix();

		Vector2 endNodePosition = endNode->GetTransform()->GetPosition();

		m_nodes.push_back(endNode);
		m_nodePreviousPositions.push_back(endNodePosition);

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

Vector2 Rope::GetAABBCollisionPoint(GameObject* ap_collision, int ai_collidingNodeIndex) {
	Vector2 colliderPosition = ap_collision->GetTransform()->GetPosition();
	Rigidbody* colliderRb = ap_collision->GetComponent<Rigidbody>();

	//Vector2 baseNodeOffset = m_nodePreviousPositions[ai_collidingNodeIndex] - colliderPosition;
	//Vector2 endNodeOffset = m_nodePreviousPositions[ai_collidingNodeIndex + 1] - colliderPosition;

	Vector2 baseNodeOffset = m_nodes[ai_collidingNodeIndex]->GetTransform()->GetPosition() - colliderPosition;
	Vector2 endNodeOffset = m_nodes[ai_collidingNodeIndex + 1]->GetTransform()->GetPosition() - colliderPosition;

	Vector2 diagonalDirection = Vector2(-std::signbit(endNodeOffset.X - baseNodeOffset.X) * 2 + 1, -std::signbit(endNodeOffset.Y - baseNodeOffset.Y) * 2 + 1);

	bool isToRightOfCollider = (endNodeOffset.X - baseNodeOffset.X) * (colliderPosition.Y - baseNodeOffset.Y) > (endNodeOffset.Y - baseNodeOffset.Y) * (colliderPosition.X - baseNodeOffset.X);

	Vector2 collidedCorner = isToRightOfCollider ? Vector2(diagonalDirection.Y, -diagonalDirection.X) : Vector2(-diagonalDirection.Y, diagonalDirection.X);

	//Adjust to collider
	collidedCorner *= colliderRb->GetAABBRect();
	collidedCorner /= 2;
	collidedCorner += colliderPosition;

	//Debug::getInstance()->Log(std::to_string(collidedCorner.X) + ", " + std::to_string(collidedCorner.Y));
	return collidedCorner;
}

Vector2 Rope::GetSphereCollisionPoint(GameObject* ap_collision, int ai_collidingNodeIndex) {
	return Vector2(0, 0);
}

void Rope::SplitSection(int ai_nodeIndex, Vector2 a_newNodePos) {
	if (ai_nodeIndex > m_nodes.size() - 2 || ai_nodeIndex < 0) {
		Debug::getInstance()->Log("Cannot add rope section. Index must fall between the start and end node index.");
		return;
	}

	//Object and position
	GameObject* newNode = new GameObject(("Rope Node " + std::to_string(m_nodes.size() + 1)).c_str(), GetOwner());
	GetOwner()->AddChild(newNode);

	newNode->GetTransform()->SetLocalPosition(a_newNodePos);

	//Force world positition update to work correctly
	newNode->GetTransform()->GetLocalToWorldMatrix();

	Rigidbody* rigidbody = newNode->AddComponent<Rigidbody>();
	rigidbody->SetType(PhysicsTypes::Trig);

	//Line renderers
	LineRenderer* lineRenderer = newNode->AddLineRendererComponent();
	lineRenderer->SetTexture("Assets/rope.tga");
	lineRenderer->SetWidth(1);
	lineRenderer->SetStartPos(a_newNodePos);
	lineRenderer->SetEndPos(m_nodes[ai_nodeIndex + 1]->GetTransform()->GetPosition());

	m_nodes[ai_nodeIndex]->GetComponent<LineRenderer>()->SetEndPos(a_newNodePos);

	//Spring joints
	SpringJoint* oldNodeSpringJoint = m_nodes[ai_nodeIndex]->GetComponent<SpringJoint>();
	oldNodeSpringJoint->SetConnectedObject(newNode);

	SpringJoint* newNodeSpringJoint = newNode->AddComponent<SpringJoint>();
	newNodeSpringJoint->SetConnectedObject(m_nodes[ai_nodeIndex + 1]);

	if (ai_nodeIndex == m_nodes.size() - 2) {
		oldNodeSpringJoint->SetSpringMode(SpringMode::NEITHER);
		oldNodeSpringJoint->SetSelfAdjustDesiredLength(false);

		newNodeSpringJoint->SetSpringMode(SpringMode::REPEL_ONLY);
		newNodeSpringJoint->SetSelfAdjustDesiredLength(true);
	}
	else
		newNodeSpringJoint->SetSpringMode(SpringMode::NEITHER);

	//Update lists
	m_nodes.insert(m_nodes.begin() + ai_nodeIndex + 1, newNode);
	m_nodePreviousPositions.insert(m_nodePreviousPositions.begin() + ai_nodeIndex + 1, newNode->GetTransform()->GetPosition());
	m_nodeRightwardBend.insert(m_nodeRightwardBend.begin() + ai_nodeIndex, IsNodeBendRightward(ai_nodeIndex + 1));
}

void Rope::JoinSection(int ai_nodeIndex) {
	m_nodes[ai_nodeIndex - 1]->GetComponent<LineRenderer>()->SetEndPos(m_nodes[ai_nodeIndex + 1]->GetTransform()->GetPosition());
	m_nodes[ai_nodeIndex - 1]->GetComponent<SpringJoint>()->SetConnectedObject(m_nodes[ai_nodeIndex + 1]);
	m_nodes[ai_nodeIndex]->SetToDestroy();

	//Update lists
	m_nodes.erase(m_nodes.begin() + ai_nodeIndex);
	m_nodePreviousPositions.erase(m_nodePreviousPositions.begin() + ai_nodeIndex);
	m_nodeRightwardBend.erase(m_nodeRightwardBend.begin() + ai_nodeIndex - 1);
}

bool Rope::IsNodeBendRightward(int ai_nodeIndex) {
	Vector2 previousNodePosition = m_nodes[ai_nodeIndex - 1]->GetTransform()->GetPosition();
	Vector2 currentNodePosition = m_nodes[ai_nodeIndex]->GetTransform()->GetPosition();
	Vector2 nextNodePosition = m_nodes[ai_nodeIndex + 1]->GetTransform()->GetPosition();

	return (nextNodePosition.X - previousNodePosition.X) * (currentNodePosition.Y - previousNodePosition.Y) > (nextNodePosition.Y - previousNodePosition.Y) * (currentNodePosition.X - previousNodePosition.X);
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
