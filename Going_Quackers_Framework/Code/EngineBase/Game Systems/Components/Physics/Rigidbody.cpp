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

	//TODO:: Make this not magic up infinity forces!?!?!?!?!?
	if (m_physicsType == PhysicsTypes::RB && !m_isStatic)
	{
		CalculateVelocity();
	}

	if (m_isStatic)
	{
		m_velocity = Vector2();
		m_acceleration = Vector2();
	}

	m_forces.clear();
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

	m_velocity += m_acceleration * Time::GetDeltaTime();

	Debug::getInstance()->Log(m_velocity);

	Vector2 pos = GetOwner()->GetTransform()->GetPosition();
	pos += m_velocity;
	GetOwner()->GetTransform()->SetPosition(pos);
}

void Rigidbody::PhysicsCollide()
{
	std::vector<GameObject*> allObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();
	std::vector<GameObject*> collidingObjects;

	for (GameObject* obj : allObjects)
	{
		if (obj->GetComponent<Rigidbody>() == nullptr || obj == GetOwner())
		{
			continue;
		}

		Rigidbody* OtherRB = obj->GetComponent<Rigidbody>();

		if (OtherRB->getCollideFlag() && OtherRB->getType() == PhysicsTypes::RB && getType() == PhysicsTypes::RB)
		{
			break;
		}

		switch (GetCollisionType())
		{
		case CollisionTypes::AABB:
			switch (obj->GetComponent<Rigidbody>()->GetCollisionType())
			{
			case CollisionTypes::AABB:
				if (mp_collider->CollisionAABB(GetOwner(), obj)) 
				{ 
					collidingObjects.push_back(obj); 
					Debug::getInstance()->Log("Obj " + GetOwner()->GetID() + " Collided with Obj " + obj->GetID()); 
				}
				break;
			case CollisionTypes::Sphere:
				if(mp_collider->CollisionSphericalAABB(GetOwner(), obj))
				{
					collidingObjects.push_back(obj); 
					Debug::getInstance()->Log("Obj " + GetOwner()->GetID() + " Collided with Obj " + obj->GetID());
				}
				break;
			}
			break;
		case CollisionTypes::Sphere:
			switch (obj->GetComponent<Rigidbody>()->GetCollisionType())
			{
			case CollisionTypes::AABB:
				if(mp_collider->CollisionSphericalAABB(GetOwner(), obj)) 
				{
					collidingObjects.push_back(obj);
					Debug::getInstance()->Log("Obj " + GetOwner()->GetID() + " Collided with Obj " + obj->GetID());
				}
				break;
			case CollisionTypes::Sphere:
				if(mp_collider->CollisionSpherical(GetOwner(), obj)) 
				{
					collidingObjects.push_back(obj);
					Debug::getInstance()->Log("Obj " + GetOwner()->GetID() + " Collided with Obj " + obj->GetID());
				}
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
	//TODO:: Make this not do infinity acceleration!?

	for (GameObject* obj : *collidingObjects)
	{
		Rigidbody* rb = obj->GetComponent<Rigidbody>();
		if (rb->getType() != PhysicsTypes::RB || rb->m_isStatic)
		{
			continue;
		}

		Vector2 vectorBetweenObjs = obj->GetTransform()->GetPosition() - GetOwner()->GetTransform()->GetPosition();
		float distance = vectorBetweenObjs.Length();

		Vector2 forceDirection = vectorBetweenObjs.Normalize();

		if (distance == 0)
		{
			continue;
		}

		Vector2 force = forceDirection * (distance / 2.0f);

		rb->AddForce(force);
		AddForce(-force);
	}
}