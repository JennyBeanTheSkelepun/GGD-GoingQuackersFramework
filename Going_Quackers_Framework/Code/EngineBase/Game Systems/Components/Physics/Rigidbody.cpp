#include "Rigidbody.h"

//- Constructor -//
Rigidbody::Rigidbody(GameObject* owner) : Component(owner, ComponentTypes::RIGIDBODY, "RigidBody")
{
	mp_GravEmitter = new GravityEmitter();
	mp_Trigger = new Trigger();
}

//- Descructor -//
Rigidbody::~Rigidbody()
{
	delete mp_GravEmitter;
	delete mp_Trigger;
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

		if (m_PhysicsType == PhysicsTypes::RB && !m_IsStatic)
		{
			CalculateVelocity();
		}

		if (m_IsStatic)
		{
			m_Velocity = Vector2();
			m_Acceleration = Vector2();
		}
	}
	else
	{
		m_Velocity = Vector2();
		m_Acceleration = Vector2();
	}

	m_Forces.clear();
}

//- ImGui -//
void Rigidbody::ImGUIUpdate()
{
	if (ImGui::BeginCombo("Object Type", m_DropdownPhysicsTypeSelected.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(m_PhysicsTypeDropDown); i++)
		{
			bool is_selected = (m_DropdownPhysicsTypeSelected == m_PhysicsTypeDropDown[i]);

			if (ImGui::Selectable(m_PhysicsTypeDropDown[i].c_str(), is_selected))
			{
				m_DropdownPhysicsTypeSelected = m_PhysicsTypeDropDown[i];
				
				if (m_DropdownPhysicsTypeSelected == "Rigidbody")
				{
					m_PhysicsType = PhysicsTypes::RB;
				}
				else if (m_DropdownPhysicsTypeSelected == "Trigger")
				{
					m_PhysicsType = PhysicsTypes::Trig;
				}
				else if (m_DropdownPhysicsTypeSelected == "Gravity Zone")
				{
					m_PhysicsType = PhysicsTypes::GE;
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

	//TODO clean this up a bit (temp value to allow the use of SetMass for input validation)
	float tempMass = GetMass();
	ImGui::InputFloat("Mass", &tempMass);
	SetMass(tempMass);

	ImGui::Spacing();

	if (ImGui::BeginCombo("Collision Type", m_DropdownColliderShapeSelected.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(m_ColliderShapeDropDown); i++)
		{
			bool is_selected = (m_DropdownColliderShapeSelected == m_ColliderShapeDropDown[i]);

			if (ImGui::Selectable(m_ColliderShapeDropDown[i].c_str(), is_selected))
			{
				m_DropdownColliderShapeSelected = m_ColliderShapeDropDown[i];
				
				if (m_DropdownColliderShapeSelected == "Sphere")
				{
					m_CollisionType = CollisionTypes::Sphere;
				}
				else if (m_DropdownColliderShapeSelected == "AABB")
				{
					m_CollisionType = CollisionTypes::AABB;
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

	if (m_CollisionType == CollisionTypes::Sphere)
	{
		ImGui::InputFloat("Radius", &m_Radius);
	}
	else if (m_CollisionType == CollisionTypes::AABB)
	{
		ImGui::InputFloat("Rect Width", &m_AABBRect.X);
		ImGui::InputFloat("Rect Height", &m_AABBRect.Y);
	}

	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	if (m_PhysicsType == PhysicsTypes::RB)
	{
		ImGui::Checkbox("Static", &m_IsStatic);
	}
	else if (m_PhysicsType == PhysicsTypes::GE)
	{
		mp_GravEmitter->ImGuiSetup();
	}
}

//- Scene Save / Load -//
void Rigidbody::SceneLoad(json* componentJSON)
{
	m_Velocity = Vector2((*componentJSON)["Velocity"][0], (*componentJSON)["Velocity"][1]);
	m_Acceleration = Vector2((*componentJSON)["Acceleration"][0], (*componentJSON)["Acceleration"][1]);
	m_Mass = (*componentJSON)["Mass"];
	m_IsStatic = (*componentJSON)["Static"];
	m_DropdownPhysicsTypeSelected = (*componentJSON)["ObjectType"];
	m_DropdownColliderShapeSelected = (*componentJSON)["ColliderShape"];

	if (m_DropdownPhysicsTypeSelected == "Rigidbody")
	{
		m_PhysicsType = PhysicsTypes::RB;
	}
	else if (m_DropdownPhysicsTypeSelected == "Trigger")
	{
		m_PhysicsType = PhysicsTypes::Trig;
	}
	else if (m_DropdownPhysicsTypeSelected == "Gravity Zone")
	{
		m_PhysicsType = PhysicsTypes::GE;
	}

	if (m_DropdownColliderShapeSelected == "Sphere")
	{
		m_CollisionType = CollisionTypes::Sphere;
	}
	else if (m_DropdownColliderShapeSelected == "AABB")
	{
		m_CollisionType = CollisionTypes::AABB;
	}

	m_Radius = (*componentJSON)["Radius"];
	m_AABBRect = Vector2((*componentJSON)["AABBRect"][0], (*componentJSON)["AABBRect"][1]);
	mp_GravEmitter->LoadGravType((*componentJSON)["GravityType"]);
	mp_GravEmitter->SetGravityStrength((*componentJSON)["GravityStrength"]);
	mp_GravEmitter->SetGravityDirection(Vector2((*componentJSON)["GravityDirection"][0], (*componentJSON)["GravityDirection"][1]));
}

json* Rigidbody::SceneSave()
{
	json* returnObj = new json({
		{"Velocity", {m_Velocity.X, m_Velocity.Y}},
		{"Acceleration", {m_Acceleration.X, m_Acceleration.Y}},
		{"Mass", m_Mass},
		{"Static", m_IsStatic},
		{"ObjectType", m_DropdownPhysicsTypeSelected},
		{"ColliderShape", m_DropdownColliderShapeSelected},
		{"Radius", m_Radius},
		{"AABBRect", {m_AABBRect.X, m_AABBRect.Y}},
		{"GravityType", mp_GravEmitter->SaveGravType()},
		{"GravityStrength", mp_GravEmitter->GetGravityStrength()},
		{"GravityDirection", {mp_GravEmitter->GetGravityDirection().X, mp_GravEmitter->GetGravityDirection().Y} }
		});

	return returnObj;
}

//- Custom Functions -//
void Rigidbody::CalculateVelocity()
{
	Vector2 totalForceNONE;
	Vector2 totalForceACCEL;
	Vector2 totalForceMASS;
	Vector2 totalForceMASSACCEL;

	for (Force forcek : m_Forces)
	{
		if (forcek.moveIgnore == MovementIgnore::NONE)
		{
			totalForceNONE += forcek.force;
		}
		else if (forcek.moveIgnore == MovementIgnore::ACCEL)
		{
			totalForceACCEL += forcek.force;
		}
		else if (forcek.moveIgnore == MovementIgnore::MASS)
		{
			totalForceMASS += forcek.force;
		}
		else if (forcek.moveIgnore == MovementIgnore::MASSACCEL)
		{
			totalForceMASSACCEL += forcek.force;
		}
	}

	for (Force force : m_Forces)
	{
		if (force.moveIgnore == MovementIgnore::NONE)
		{
			m_Acceleration = totalForceNONE / m_Mass;

			m_Velocity += m_Acceleration * Time::GetDeltaTime();
		}
		else if (force.moveIgnore == MovementIgnore::ACCEL)
		{
			m_Velocity += totalForceACCEL / m_Mass;

			m_Acceleration = Vector2();
		}
		else if (force.moveIgnore == MovementIgnore::MASS)
		{
			m_Acceleration = totalForceMASS;

			m_Velocity += m_Acceleration * Time::GetDeltaTime();
		}
		else if (force.moveIgnore == MovementIgnore::MASSACCEL)
		{
			m_Velocity += totalForceMASSACCEL;

			m_Acceleration = Vector2();
		}
	}

	Vector2 pos = GetOwner()->GetTransform()->GetPosition();
	pos += m_Velocity;
	GetOwner()->GetTransform()->SetPosition(pos);
}

void Rigidbody::PhysicsCollide()
{
	std::vector<GameObject*> allObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();
	m_CollidingObjects.clear();

	for (GameObject* obj : allObjects)
	{
		if (obj->GetComponent<Rigidbody>() == nullptr || obj == GetOwner())
		{
			continue;
		}

		Rigidbody* OtherRB = obj->GetComponent<Rigidbody>();

		switch (GetCollisionType())
		{
		case CollisionTypes::AABB:
			switch (obj->GetComponent<Rigidbody>()->GetCollisionType())
			{
			case CollisionTypes::AABB:
				if (Collision::getInstance()->CollisionAABB(GetOwner(), obj)) 
				{ 
					m_CollidingObjects.push_back(obj); 
					Debug::getInstance()->Log("Obj " + GetOwner()->GetID() + " Collided with Obj " + obj->GetID()); 
				}
				break;
			case CollisionTypes::Sphere:
				if(Collision::getInstance()->CollisionSphericalAABB(GetOwner(), obj))
				{
					m_CollidingObjects.push_back(obj); 
					Debug::getInstance()->Log("Obj " + GetOwner()->GetID() + " Collided with Obj " + obj->GetID());
				}
				break;
			}
			break;
		case CollisionTypes::Sphere:
			switch (obj->GetComponent<Rigidbody>()->GetCollisionType())
			{
			case CollisionTypes::AABB:
				if(Collision::getInstance()->CollisionSphericalAABB(GetOwner(), obj))
				{
					m_CollidingObjects.push_back(obj);
					Debug::getInstance()->Log("Obj " + GetOwner()->GetID() + " Collided with Obj " + obj->GetID());
				}
				break;
			case CollisionTypes::Sphere:
				if(Collision::getInstance()->CollisionSpherical(GetOwner(), obj))
				{
					m_CollidingObjects.push_back(obj);
					Debug::getInstance()->Log("Obj " + GetOwner()->GetID() + " Collided with Obj " + obj->GetID());
				}
				break;
			}
			break;
		}
	}

	switch (m_PhysicsType)
	{
	case PhysicsTypes::GE:
		mp_GravEmitter->ApplyGravity(GetOwner(), &m_CollidingObjects);
		CheckColliding(&m_CollidingObjects);
		break;
	case PhysicsTypes::RB:
		RigidbodyCollide(&m_CollidingObjects);
		CheckColliding(&m_CollidingObjects);
		break;
	case PhysicsTypes::Trig:
		CheckColliding(&m_CollidingObjects);
		break;
	}
	
	m_PhysicsChecked = true;
}

void Rigidbody::RigidbodyCollide(std::vector<GameObject*>* collidingObjects)
{
	for (GameObject* obj : *collidingObjects)
	{
		Rigidbody* rb = obj->GetComponent<Rigidbody>();

		if (rb == nullptr)
		{
			continue;
		}

		Vector2 rV;
		Vector2 rA;
		Vector2 dV = rb->GetVelocity();
		Vector2 dA = rb->GetAcceleration();

		//TODO:: Make it set poss, not sure why?
		if (rb->GetCollisionType() == CollisionTypes::Sphere)
		{
			Vector2 n = (obj->GetTransform()->GetPosition() - GetOwner()->GetTransform()->GetPosition()).Normalize();

			rV = dV - (n * (2 * dV.Dot(n)));
			rA = dA - (n * (2 * dA.Dot(n)));

			Vector2 newPos = GetOwner()->GetTransform()->GetPosition() + (n * (GetRadius() + rb->GetRadius()));

			obj->GetTransform()->SetPosition(newPos);
		}
		else if(rb->GetCollisionType() == CollisionTypes::AABB)
		{
			Vector2 n = (obj->GetTransform()->GetPosition() - GetOwner()->GetTransform()->GetPosition()).Normalize();

			n.X = std::round(n.X);
			n.Y = std::round(n.Y);

			Debug::getInstance()->Log(n);

			Vector2 newPos = GetOwner()->GetTransform()->GetPosition() + (n * ((GetAABBRect() / 2.0f) + (rb->GetAABBRect() / 2.0f)));
			
			rV = dV - (n * (2 * dV.Dot(n)));
			rA = dA - (n * (2 * dA.Dot(n)));

			obj->GetTransform()->SetPosition(newPos);
		}

		if (rV != Vector2())
		{
			SetVelocity(rV);
		}
		else
		{
			SetVelocity(-GetVelocity());
		}

		if (rA != Vector2())
		{
			SetAcceleration(rA);
		}
		else
		{
			SetAcceleration(-GetAcceleration());
		}
	}
}

bool Rigidbody::CheckColliding(std::vector<GameObject*>* collidingObjects)
{
	m_isColliding = collidingObjects->size() == 0 ? false : true;

	return m_isColliding;
}

bool Rigidbody::CheckColliding(GameObject* checkObject, std::vector<GameObject*>* collidingObjects)
{
	return std::find(collidingObjects->begin(), collidingObjects->end(), checkObject) == collidingObjects->end() ? false : true;
}