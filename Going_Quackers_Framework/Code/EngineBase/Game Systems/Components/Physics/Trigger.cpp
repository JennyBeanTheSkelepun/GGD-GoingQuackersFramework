#include "Trigger.h"

bool Trigger::CheckColliding(std::vector<GameObject*>* collidingObjects)
{
	m_isColliding = collidingObjects->size() == 0 ? false : true;

	return m_isColliding;
}

bool Trigger::CheckColliding(GameObject* checkObject, std::vector<GameObject*>* collidingObjects)
{
	return std::find(collidingObjects->begin(), collidingObjects->end(), checkObject) == collidingObjects->end() ? false : true;
}