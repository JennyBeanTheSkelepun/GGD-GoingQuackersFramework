#ifndef _TRIGGER_
#define _TRIGGER_

#include "Collision.h"

class Trigger
{
public:
	bool GetCollidingBool() { return m_isColliding; }
	bool CheckColliding(std::vector<GameObject*>* collidingObjects);
	bool CheckColliding(GameObject* checkObject, std::vector<GameObject*>* collidingObjects);

private:
	bool m_isColliding;
};

#endif _TRIGGER_