#ifndef _GRAVITYEMITTER_
#define _GRAVITYEMITTER_

#include "Collision.h"

enum class GravityTypes
{
	DIRECTION = 0,
	CENTRE = 1
};

class Rigidbody;

class GravityEmitter
{
public:
	void ApplyGravity(GameObject* callObj, std::vector<GameObject*>* collidedObjects);

	void SetGravityStrength(float strength) { m_GravityStrength = strength; }
	float GetGravityStrength() { return m_GravityStrength; }

	void SetGravityDirection(Vector2 direction) { m_GravityDirection = direction.Normalize(); }
	Vector2 GetGravityDirection() { return m_GravityDirection; }

	void SetGravityType(GravityTypes type) { m_GravType = type; }
	GravityTypes GetGravityType() { return m_GravType; }
	
	std::string SaveGravType() { return m_GravTypeDropdownSelected; }
	void LoadGravType(std::string gravType);

	void ImGuiSetup();

private:
	float m_GravityStrength = 0.1f;
	Vector2 m_GravityDirection = Vector2(0.0f, 1.0f);

	GravityTypes m_GravType = GravityTypes::DIRECTION;

	std::string m_GravTypeDropdown[2] = { "Direction", "Centre" };

	std::string m_GravTypeDropdownSelected = "Direction";
};

#endif _GRAVITYEMITTER_