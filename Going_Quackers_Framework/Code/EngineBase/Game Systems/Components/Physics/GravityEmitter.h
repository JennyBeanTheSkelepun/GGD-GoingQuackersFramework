#ifndef _GRAVITYEMITTER_
#define _GRAVITYEMITTER_

#include "Collision.h"

enum class GravityTypes
{
	DIRECTION = 0,
	CENTRE = 1
};

class Ridigbody;

class GravityEmitter
{
public:
	void applyGravity(GameObject* callObj, std::vector<GameObject*>* collidedObjects);

	void SetGravityStrength(float strength) { m_gravityStrength = strength; }
	float GetGravityStrength() { return m_gravityStrength; }
	void SetGravityDirection(Vector2 direction) { m_gravityDirection = direction.Normalize(); }
	Vector2 GetGravityDirection() { return m_gravityDirection; }

	void ImGuiSetup();

private:
	float m_gravityStrength = 0.1f;
	Vector2 m_gravityDirection = Vector2(0.0f, 1.0f);

	GravityTypes m_gravType = GravityTypes::DIRECTION;

	const char* m_GravTypeDropdown[2] = { "Direction", "Centre" };

	const char* m_GravTypeDropdownSelected = "Direction";
};

#endif _GRAVITYEMITTER_