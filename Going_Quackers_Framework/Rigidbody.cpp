#include "Rigidbody.h"

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

	if (collision.GetCollisionType() == 's')
	{
		collidingObjects = collision.CollisionAABB();
	}
	else if(collision.GetCollisionType() == 'a')
	{
		collidingObjects = collision.CollisionSpherical();
	}

	for (GameObject* obj : collidingObjects)
	{
		Vector2 forceDirection = obj->GetTransform()->GetPosition() - this->GetOwner()->GetTransform()->GetPosition();
		forceDirection.Normalize();

		float distance = (obj->GetTransform()->GetPosition() - this->GetOwner()->GetTransform()->GetPosition()).Length();

		Vector2 appliedForce = forceDirection * (distance / 2.0f);

		AddForce(appliedForce);
		obj->GetComponent<Rigidbody>()->AddForce(-appliedForce);
	}
}