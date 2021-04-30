#include "Rigidbody.h"

Rigidbody::Rigidbody(GameObject* owner) : Component(owner, ComponentTypes::RIGIDBODY)
{
	m_collider = new Collision();
	m_gravEmitter = new GravityEmitter();
	m_trigger = new Trigger();
}

Rigidbody::~Rigidbody()
{
	delete m_collider;
	delete m_gravEmitter;
	delete m_trigger;
}

void Rigidbody::Update()
{
	PhysicsCollide();

	CalculateVelocity();
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
	std::vector<GameObject*> collidingObjects;

	switch (m_collider->GetCollisionType())
	{
	case CollisionTypes::AABB:
		collidingObjects = m_collider->CollisionSpherical(GetOwner());
		break;
	case CollisionTypes::Sphere:
		collidingObjects = m_collider->CollisionSpherical(GetOwner());
		break;
	}

	switch (m_physicsType)
	{
	case PhysicsTypes::GE:
		m_gravEmitter->applyGravity(GetOwner(), &collidingObjects);
		break;
	case PhysicsTypes::RB:
		RigidbodyCollide(&collidingObjects);
		break;
	case PhysicsTypes::Trig:
		m_trigger->CheckColliding(&collidingObjects);
		break;
	}
	
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