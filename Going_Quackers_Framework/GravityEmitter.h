#ifndef GRAVITYEMITTER
#define GRAVITYEMITTER

#include "Collision.h"
#include "Code/EngineBase/Game Systems/Time.h"
#include "Code/EngineBase/Game Systems/Components/Component.h"
#include "Rigidbody.h"

class GravityEmitter : Collision
{
//Add collision checking and AddForce to Rigidbody in the area.
public:
	void Update() override;

	void SetGravityStrength(float strength) { m_gravityStrength = strength; }
	float GetGravityStrength() { return m_gravityStrength; }
	void SetGravityDirection(Vector2 direction) { m_gravityDirection = direction.Normalize(); }
	Vector2 GetGravityDirection() { return m_gravityDirection; }

private:
	std::vector<GameObject*> m_collidiedObjects;

	float m_gravityStrength;
	Vector2 m_gravityDirection;
};

#endif GRAVITYEMITTER