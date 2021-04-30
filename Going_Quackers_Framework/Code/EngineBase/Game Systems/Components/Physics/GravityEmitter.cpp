#include "GravityEmitter.h"
#include "Rigidbody.h"

void GravityEmitter::applyGravity(GameObject* callObj, std::vector<GameObject*>* CollidedObjects)
{
	for (GameObject* obj : *CollidedObjects)
	{
		Rigidbody* rb = obj->GetComponent<Rigidbody>();
		if (rb != nullptr)
		{
			Vector2 gravityPosition = callObj->GetTransform()->GetPosition();

			switch (rb->getCollider()->GetCollisionType())
			{
			case CollisionTypes::AABB:
				rb->AddForce(m_gravityDirection * m_gravityStrength);
				break;
			case CollisionTypes::Sphere:
				Vector2 force = (gravityPosition - obj->GetTransform()->GetPosition()).Normalize() * m_gravityStrength;
				obj->GetComponent<Rigidbody>()->AddForce(force);
				break;
			}
		}
	}
}