#include "Code/EngineBase/Game Systems/GameObject.h"
#include "Code/EngineBase/Game Systems/Components/Component.h"
#include "Code/EngineBase/Data Structures/Vectors.h"

class Collision : public Component
{
public:
	std::vector<GameObject*> CollisionSpherical();
	std::vector<GameObject*> CollisionAABB();

	void SetRadius(float radius) { m_radius = radius; }
	float GetRadius() { return m_radius; }

	void SetAABBRect(float width, float height) { m_AABBRect = Vector2(width, height); }
	Vector2 GetAABBRect() { return m_AABBRect; } //returns vec2 with x = width, y = height

	void SetCollisionType(char colType) { m_collisionType = colType; } // s = Spherical, a = AABB
	char GetCollisionType() { return m_collisionType; }

private:
	float m_radius;
	Vector2 m_AABBRect;

	char m_collisionType = 's';
};

