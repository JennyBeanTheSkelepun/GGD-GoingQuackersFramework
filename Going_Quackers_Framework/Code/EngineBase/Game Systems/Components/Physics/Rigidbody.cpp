#include "Rigidbody.h"

Rigidbody::Rigidbody(GameObject* owner) : Component(owner, ComponentTypes::RIGIDBODY)
{
	mp_collider = new Collision();
	mp_gravEmitter = new GravityEmitter();
	mp_trigger = new Trigger();
}

Rigidbody::~Rigidbody()
{
	delete mp_collider;
	delete mp_gravEmitter;
	delete mp_trigger;
}

void Rigidbody::Update()
{
	PhysicsCollide();

	if (m_physicsType == PhysicsTypes::RB && !m_isStatic)
	{
		CalculateVelocity();
	}

	if (m_isStatic)
	{
		m_velocity = Vector2();
		m_acceleration = Vector2();
	}
}

void Rigidbody::ImGUIUpdate()
{
	if (ImGui::BeginCombo("Object Type", m_DropdownPhysicsTypeSelected))
	{
		for (int i = 0; i < IM_ARRAYSIZE(m_physicsTypeDropDown); i++)
		{
			bool is_selected = (m_DropdownPhysicsTypeSelected == m_physicsTypeDropDown[i]);

			if (ImGui::Selectable(m_physicsTypeDropDown[i], is_selected))
			{
				m_DropdownPhysicsTypeSelected = m_physicsTypeDropDown[i];
				
				if (m_DropdownPhysicsTypeSelected == "Rigidbody")
				{
					m_physicsType = PhysicsTypes::RB;
				}
				else if (m_DropdownPhysicsTypeSelected == "Trigger")
				{
					m_physicsType = PhysicsTypes::Trig;
				}
				else if (m_DropdownPhysicsTypeSelected == "Gravity Zone")
				{
					m_physicsType = PhysicsTypes::GE;
				}
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Spacing();

	if (ImGui::BeginCombo("Collision Type", m_DropdownColliderShapeSelected))
	{
		for (int i = 0; i < IM_ARRAYSIZE(m_colliderShapeDropDown); i++)
		{
			bool is_selected = (m_DropdownColliderShapeSelected == m_colliderShapeDropDown[i]);

			if (ImGui::Selectable(m_colliderShapeDropDown[i], is_selected))
			{
				m_DropdownColliderShapeSelected = m_colliderShapeDropDown[i];
				
				if (m_DropdownColliderShapeSelected == "Sphere")
				{
					m_collisionType = CollisionTypes::Sphere;
				}
				else if (m_DropdownColliderShapeSelected == "AABB")
				{
					m_collisionType = CollisionTypes::AABB;
				}
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	if (m_collisionType == CollisionTypes::Sphere)
	{
		ImGui::InputFloat("Radius", &m_radius);
	}
	else if (m_collisionType == CollisionTypes::AABB)
	{
		ImGui::InputFloat("Rect Width", &m_AABBRect.X);
		ImGui::InputFloat("Rect Height", &m_AABBRect.Y);
	}

	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	if (m_physicsType == PhysicsTypes::RB)
	{
		ImGui::Checkbox("Static", &m_isStatic);
	}
	else if (m_physicsType == PhysicsTypes::GE)
	{
		mp_gravEmitter->ImGuiSetup();
	}
}

void Rigidbody::CalculateVelocity()
{
	Vector2 totalForce;

	for(Vector2 force : m_forces)
	{
		totalForce += force;
	}

	m_acceleration = totalForce / m_mass;

	m_velocity = m_acceleration / Time::GetDeltaTime();
}

void Rigidbody::PhysicsCollide()
{
	//std::vector<GameObject*> allObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();
	std::vector<GameObject*> collidingObjects;

	for (GameObject* obj : collidingObjects)
	{
		if (obj->GetComponent<Rigidbody>()->getCollideFlag())
		{
			break;
		}

		switch (GetCollisionType())
		{
		case CollisionTypes::AABB:
			switch (obj->GetComponent<Rigidbody>()->GetCollisionType())
			{
			case CollisionTypes::AABB:
				if (mp_collider->CollisionAABB(GetOwner(), obj)) { collidingObjects.push_back(obj); };
				break;
			case CollisionTypes::Sphere:
				if(mp_collider->CollisionSphericalAABB(GetOwner(), obj)) { collidingObjects.push_back(obj); };
				break;
			}
			break;
		case CollisionTypes::Sphere:
			switch (obj->GetComponent<Rigidbody>()->GetCollisionType())
			{
			case CollisionTypes::AABB:
				if(mp_collider->CollisionSphericalAABB(GetOwner(), obj)) { collidingObjects.push_back(obj); };
				break;
			case CollisionTypes::Sphere:
				if(mp_collider->CollisionSpherical(GetOwner(), obj)) { collidingObjects.push_back(obj); };
				break;
			}
			break;
		}
	}

	switch (m_physicsType)
	{
	case PhysicsTypes::GE:
		mp_gravEmitter->applyGravity(GetOwner(), &collidingObjects);
		break;
	case PhysicsTypes::RB:
		RigidbodyCollide(&collidingObjects);
		break;
	case PhysicsTypes::Trig:
		mp_trigger->CheckColliding(&collidingObjects);
		break;
	}
	
	physicsChecked = true;
}

void Rigidbody::RigidbodyCollide(std::vector<GameObject*>* collidingObjects)
{
	for (GameObject* obj : *collidingObjects)
	{
		Vector2 forceDirection = obj->GetTransform()->GetPosition() - GetOwner()->GetTransform()->GetPosition();
		forceDirection.Normalize();

		float distance = (obj->GetTransform()->GetPosition() - GetOwner()->GetTransform()->GetPosition()).Length();

		Vector2 appliedForce = forceDirection * (distance / 2.0f);

		AddForce(appliedForce);
		obj->GetComponent<Rigidbody>()->AddForce(-appliedForce);
	}
}