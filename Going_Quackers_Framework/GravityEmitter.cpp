#include "GravityEmitter.h"

void GravityEmitter::Update()
{
	if (GetCollisionType() == 's')
	{
		m_collidiedObjects = CollisionSpherical();

		Vector2 gravityPosition = GetOwner()->GetTransform()->GetPosition();

		for (GameObject* obj : m_collidiedObjects)
		{
			Vector2 force = (gravityPosition - obj->GetTransform()->GetPosition()).Normalize() * m_gravityStrength;
			obj->GetComponent<Rigidbody>()->AddForce(force);
		}
	}
	else if (GetCollisionType() == 'a')
	{
		m_collidiedObjects = CollisionAABB();

		for (GameObject* obj : m_collidiedObjects)
		{
			obj->GetComponent<Rigidbody>()->AddForce(m_gravityDirection * m_gravityStrength);
		}
	}
}