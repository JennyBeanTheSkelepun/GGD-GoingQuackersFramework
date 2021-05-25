#include "Rigidbody.h"

//- Constructor -//
Rigidbody::Rigidbody(GameObject* owner) : Component(owner, ComponentTypes::RIGIDBODY, "RigidBody")
{
	mp_collider = new Collision();
	mp_gravEmitter = new GravityEmitter();
	mp_trigger = new Trigger();
}

//- Descructor -//
Rigidbody::~Rigidbody()
{
	delete mp_collider;
	delete mp_gravEmitter;
	delete mp_trigger;
}

void Rigidbody::OnDestroy()
{
	this->~Rigidbody();
}

//- Update Render Functions -//
void Rigidbody::Update()
{
	if (EngineGuiClass::getInstance()->IsInPlayMode())
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

	m_forces.clear();
}

//- ImGui -//
void Rigidbody::ImGUIUpdate()
{
	if (ImGui::BeginCombo("Object Type", m_DropdownPhysicsTypeSelected.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(m_physicsTypeDropDown); i++)
		{
			bool is_selected = (m_DropdownPhysicsTypeSelected == m_physicsTypeDropDown[i]);

			if (ImGui::Selectable(m_physicsTypeDropDown[i].c_str(), is_selected))
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

	if (ImGui::BeginCombo("Collision Type", m_DropdownColliderShapeSelected.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(m_colliderShapeDropDown); i++)
		{
			bool is_selected = (m_DropdownColliderShapeSelected == m_colliderShapeDropDown[i]);

			if (ImGui::Selectable(m_colliderShapeDropDown[i].c_str(), is_selected))
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

//- Scene Save / Load -//
void Rigidbody::SceneLoad(json* componentJSON)
{
	//TODO:: ADD Sceneload for RB

	m_velocity = Vector2((*componentJSON)["Velocity"][0], (*componentJSON)["Velocity"][1]);
	m_acceleration = Vector2((*componentJSON)["Acceleration"][0], (*componentJSON)["Acceleration"][1]);
	m_mass = (*componentJSON)["Mass"];
	m_isStatic = (*componentJSON)["Static"];
	m_DropdownPhysicsTypeSelected = (*componentJSON)["ObjectType"];
	m_DropdownColliderShapeSelected = (*componentJSON)["ColliderShape"];

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

	if (m_DropdownColliderShapeSelected == "Sphere")
	{
		m_collisionType = CollisionTypes::Sphere;
	}
	else if (m_DropdownColliderShapeSelected == "AABB")
	{
		m_collisionType = CollisionTypes::AABB;
	}

	m_radius = (*componentJSON)["Radius"];
	m_AABBRect = Vector2((*componentJSON)["AABBRect"][0], (*componentJSON)["AABBRect"][1]);
	mp_gravEmitter->LoadGravType((*componentJSON)["GravityType"]);
	mp_gravEmitter->SetGravityStrength((*componentJSON)["GravityStrength"]);
	mp_gravEmitter->SetGravityDirection(Vector2((*componentJSON)["GravityDirection"][0], (*componentJSON)["GravityDirection"][1]));
}

json* Rigidbody::SceneSave()
{
	json* returnObj = new json({
		{"Velocity", {m_velocity.X, m_velocity.Y}},
		{"Acceleration", {m_acceleration.X, m_acceleration.Y}},
		{"Mass", m_mass},
		{"Static", m_isStatic},
		{"ObjectType", m_DropdownPhysicsTypeSelected},
		{"ColliderShape", m_DropdownColliderShapeSelected},
		{"Radius", m_radius},
		{"AABBRect", {m_AABBRect.X, m_AABBRect.Y}},
		{"GravityType", mp_gravEmitter->SaveGravType()},
		{"GravityStrength", mp_gravEmitter->GetGravityStrength()},
		{"GravityDirection", {mp_gravEmitter->GetGravityDirection().X, mp_gravEmitter->GetGravityDirection().Y} }
		});

	return nullptr;
}

//- Custom Functions -//
void Rigidbody::CalculateVelocity()
{
	Vector2 totalForce;

	for(Vector2 force : m_forces)
	{
		totalForce += force;
	}

	m_acceleration = totalForce / m_mass;

	m_velocity += m_acceleration * Time::GetDeltaTime();

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