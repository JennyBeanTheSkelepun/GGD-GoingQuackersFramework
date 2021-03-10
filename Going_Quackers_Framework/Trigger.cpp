#include "Trigger.h"

void Trigger::Update()
{
	if (collision.GetCollisionType() == 's')
	{
		m_collidiedObjects = collision.CollisionSpherical();
	}
	else if (collision.GetCollisionType() == 'a')
	{
		m_collidiedObjects = collision.CollisionAABB();
	}

	if (m_collidiedObjects.size() == 0)
	{
		m_isColliding = false;
	}
	else
	{
		m_isColliding = true;
	}
}