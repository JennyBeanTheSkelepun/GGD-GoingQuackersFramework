#ifndef _COLLISION_
#define _COLLISION_

#include "../../GameObject.h"
#include "../../../Data Structures/Vectors.h"
#include "../../../SceneManager/SceneManager.h"

class Rigidbody;

enum class CollisionTypes
{
	Sphere = 0,
	AABB = 1
};

class Collision
{
public:
	std::vector<GameObject*> CollisionSpherical(GameObject* checkObject);
	std::vector<GameObject*> CollisionAABB(GameObject* checkObject);

	void SetRadius(float radius) { m_radius = radius; }
	float GetRadius() { return m_radius; }

	void SetAABBRect(float width, float height) { m_AABBRect = Vector2(width, height); }
	Vector2 GetAABBRect() { return m_AABBRect; } ///returns vec2 with x = width, y = height

	void SetCollisionType(CollisionTypes colType) { m_collisionType = colType; } /// s = Spherical, a = AABB
	CollisionTypes GetCollisionType() { return m_collisionType; }

private:
	float m_radius = 0.0f;
	Vector2 m_AABBRect = Vector2();

	CollisionTypes m_collisionType = CollisionTypes::Sphere;
};

#endif _COLLISION_