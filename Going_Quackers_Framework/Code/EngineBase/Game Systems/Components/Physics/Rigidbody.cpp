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

	if (m_physicsType == PhysicsTypes::RB)
	{
		CalculateVelocity();
	}
}

void Rigidbody::ImGUIUpdate()
{
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