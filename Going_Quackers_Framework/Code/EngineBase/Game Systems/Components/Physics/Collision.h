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
	
	static Collision* getInstance();

	Collision(Collision const&) = delete;
	void operator=(Collision const&) = delete;
	
	/// <summary>
	/// Check collision with a spherical collision using the objects radii
	/// </summary>
	/// <param name="checkObjectA"></param>
	/// <param name="checkObjectB"></param>
	/// <returns>Bool for colliding</returns>
	bool CollisionSpherical(GameObject* checkObjectA, GameObject* checkObjectB);

	/// <summary>
	/// Check collision between 2 objects using the collision rect AABB
	/// </summary>
	/// <param name="checkObjectA"></param>
	/// <param name="checkObjectB"></param>
	/// <returns>Bool for colliding</returns>
	bool CollisionAABB(GameObject* checkObjectA, GameObject* checkObjectB);

	/// <summary>
	/// Checks collision using objectA AABB Rect and objectsBs radius
	/// </summary>
	/// <param name="checkObjectA"></param>
	/// <param name="checkObjectB"></param>
	/// <returns>Bool for colliding</returns>
	bool CollisionSphericalAABB(GameObject* checkObjectA, GameObject* checkObjectB);

	bool RaycastSphere(Vector2 Ray, Vector2 RayOrigin, GameObject* checkObject);

	bool RaycastAABB(Vector2 Ray, Vector2 RayOrigin, GameObject* checkObject);

	//TODO:: Add check ray against all objects in scene function
	std::vector<GameObject*> Raycast(Vector2 Ray, Vector2 RayOrigin);
	bool Raycast(Vector2 Ray, Vector2 RayOrigin, GameObject* checkObject);

private:
	static Collision* singletonInstance;
	Collision();

	bool DoIntersect(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2);

	int Orientation(Vector2 p1, Vector2 p2, Vector2 p3);

	bool OnSeg(Vector2 p, Vector2 q, Vector2 r);
};

#endif _COLLISION_