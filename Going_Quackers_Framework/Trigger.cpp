#include "Trigger.h"

void Trigger::Update()
{
	if (GetCollisionType() == 's')
	{
		m_collidiedObjects = CollisionSpherical();
	}
	else if (GetCollisionType() == 'a')
	{
		m_collidiedObjects = CollisionAABB();
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