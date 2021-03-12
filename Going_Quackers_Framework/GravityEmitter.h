#ifndef GRAVITYEMITTER
#define GRAVITYEMITTER

#include "Collision.h"
#include "Code/EngineBase/Game Systems/Time.h"
#include "Code/EngineBase/Game Systems/Components/Component.h"

class GravityEmitter : Collision
{
//Add collision checking and AddForce to Rigidbody in the area.
public:
	void Update() override;

	bool GetCollidingBool() { return m_isColliding; }
	std::vector<GameObject*> GetCollidingObjects() { return m_collidiedObjects; }

private:
	std::vector<GameObject*> m_collidiedObjects;
	bool m_isColliding;

	Collision* mp_collision;
};

#endif GRAVITYEMITTER