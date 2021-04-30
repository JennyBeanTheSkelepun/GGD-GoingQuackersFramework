#ifndef _COLLISION_
#define _COLLISION_

#include "../../GameObject.h"
#include "../../../Data Structures/Vectors.h"

class Rigidbody;

enum class CollisionTypes
{
	Sphere = 0,
	AABB = 1
};

class Collision
{
public:
	bool CollisionSpherical(GameObject* checkObjectA, GameObject* checkObjectB);
	bool CollisionAABB(GameObject* checkObjectA, GameObject* checkObjectB);
	bool CollisionSphericalAABB(GameObject* checkObjectA, GameObject* checkObjectB);
};

#endif _COLLISION_