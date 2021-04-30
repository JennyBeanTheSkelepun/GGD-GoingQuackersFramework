#ifndef _GRAVITYEMITTER_
#define _GRAVITYEMITTER_

#include "Collision.h"

class Ridigbody;

class GravityEmitter
{
//Add collision checking and AddForce to Rigidbody in the area.
public:
	void applyGravity(GameObject* callObj, std::vector<GameObject*>* collidedObjects);

	void SetGravityStrength(float strength) { m_gravityStrength = strength; }
	float GetGravityStrength() { return m_gravityStrength; }
	void SetGravityDirection(Vector2 direction) { m_gravityDirection = direction.Normalize(); }
	Vector2 GetGravityDirection() { return m_gravityDirection; }

private:
	float m_gravityStrength;
	Vector2 m_gravityDirection;
};

#endif _GRAVITYEMITTER_