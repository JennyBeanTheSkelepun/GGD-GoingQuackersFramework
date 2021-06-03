#include "Collision.h"
#include "Rigidbody.h"

Collision* Collision::singletonInstance = 0;

Collision::Collision()
{

}

Collision* Collision::getInstance()
{
	if (singletonInstance == 0)
	{
		singletonInstance = new Collision();
	}

	return singletonInstance;
}

bool Collision::CollisionSpherical(GameObject* checkObjectA, GameObject* checkObjectB)
{
	if(checkObjectA != checkObjectB && checkObjectA->GetComponent<Rigidbody>() != nullptr && checkObjectB->GetComponent<Rigidbody>() != nullptr);
	{
		float radA = checkObjectA->GetComponent<Rigidbody>()->GetRadius();
		float radB = checkObjectB->GetComponent<Rigidbody>()->GetRadius();

		float distance = (checkObjectA->GetTransform()->GetPosition() - checkObjectB->GetTransform()->GetPosition()).Length();

		if (distance <= (radA + radB))
		{
			return true;
		}
	}

	return false;
}

bool Collision::CollisionAABB(GameObject* checkObjectA, GameObject* checkObjectB)
{
	if (checkObjectA != checkObjectB && checkObjectA->GetComponent<Rigidbody>() != nullptr && checkObjectB->GetComponent<Rigidbody>() != nullptr);
	{
		Vector2 obj1Pos = checkObjectA->GetTransform()->GetPosition();

		Vector2 obj2Pos = checkObjectB->GetTransform()->GetPosition();

		Vector2 WidthHeight1 = checkObjectA->GetComponent<Rigidbody>()->GetAABBRect();
		Vector2 WidthHeight2 = checkObjectB->GetComponent<Rigidbody>()->GetAABBRect();

		if (obj1Pos.X - (WidthHeight1.X / 2.0f) <= obj2Pos.X + (WidthHeight2.X / 2.0f) &&
			obj1Pos.X + (WidthHeight1.X / 2.0f) >= obj2Pos.X - (WidthHeight2.X / 2.0f) &&
			obj1Pos.Y - (WidthHeight1.Y / 2.0f) <= obj2Pos.Y + (WidthHeight2.Y / 2.0f) &&
			obj1Pos.Y + (WidthHeight1.Y / 2.0f) >= obj2Pos.Y - (WidthHeight2.Y / 2.0f)
			)
		{
			return true;
		}
	}

	return false;
}

bool Collision::CollisionSphericalAABB(GameObject* checkObjectA, GameObject* checkObjectB)
{
	Vector2 closestPoint;

	Vector2 obj1Pos = checkObjectA->GetTransform()->GetPosition();

	Vector2 WidthHeight;
	float Radius;

	if (checkObjectA->GetComponent<Rigidbody>()->GetCollisionType() == CollisionTypes::AABB)
	{
		WidthHeight = checkObjectA->GetComponent<Rigidbody>()->GetAABBRect();
		Radius = checkObjectB->GetComponent<Rigidbody>()->GetRadius();
	}
	else
	{
		WidthHeight = checkObjectB->GetComponent<Rigidbody>()->GetAABBRect();
		Radius = checkObjectA->GetComponent<Rigidbody>()->GetRadius();
	}


	Vector2 minPoint = Vector2(obj1Pos - (WidthHeight / 2.0f));
	Vector2 maxPoint = Vector2(obj1Pos + (WidthHeight / 2.0f));

	Vector2 obj2Pos = checkObjectB->GetTransform()->GetPosition();

	closestPoint.X = max(minPoint.X, min(obj2Pos.X, maxPoint.X));
	closestPoint.Y = max(minPoint.Y, min(obj2Pos.Y, maxPoint.Y));

	float distance = sqrt(pow(closestPoint.X - obj2Pos.X, 2) + pow(closestPoint.Y - obj2Pos.Y, 2));

	return distance < Radius;
}

bool Collision::RaycastSphere(Vector2 Ray, Vector2 RayOrigin, GameObject* checkObject)
{
	Rigidbody* rb = checkObject->GetComponent<Rigidbody>();
	if (rb == nullptr) return false;

	if (Ray == Vector2()) return false;

	if (checkObject->GetTransform()->GetPosition() == RayOrigin) return true;

	Vector2 RayEnd = RayOrigin + Ray;

	float a = RayEnd.Y - RayOrigin.Y;
	float b = RayOrigin.X - RayEnd.X;
	float c = a * RayOrigin.X + b * RayOrigin.Y;
	c = -c;

	Vector2 pos = checkObject->GetTransform()->GetPosition();

	float dist = (abs(a * pos.X + b * pos.Y + c) / sqrt(a * a + b * b));

	Vector2 OrigToCheck = pos - RayOrigin;

	float angleBetweenRayAndObj = acos(Ray.Dot(OrigToCheck) / (Ray.Length() * OrigToCheck.Length()));

	Debug::getInstance()->Log(angleBetweenRayAndObj);

	if (dist > rb->GetRadius()) return false;

	if(angleBetweenRayAndObj < 1.5708f || angleBetweenRayAndObj > 4.71239f) return true;

	return false;
}

bool Collision::RaycastAABB(Vector2 Ray, Vector2 RayOrigin, GameObject* checkObject)
{
	Vector2 rayEnd = Ray + RayOrigin;

	Rigidbody* rb = checkObject->GetComponent<Rigidbody>();

	if (Ray == Vector2()) return false;

	if (rb == nullptr) return false;

	if (checkObject->GetTransform()->GetPosition() == RayOrigin) return true;

	Vector2 rect = rb->GetAABBRect();
	Vector2 pos = checkObject->GetTransform()->GetPosition();
	Vector2 RayEnd = RayOrigin + Ray;

	Vector2 OrigToCheck = pos - RayOrigin;

	float angleBetweenRayAndObj = acos(Ray.Dot(OrigToCheck) / (Ray.Length() * OrigToCheck.Length()));

	//Debug::getInstance()->Log(angleBetweenRayAndObj);

	if (angleBetweenRayAndObj > 1.5708f && angleBetweenRayAndObj < 4.71239f) return false;

	if (Ray.Y == 0.0f)
	{
		if ((pos.Y + (rect.Y / 2.0f)) > RayOrigin.Y > (pos.Y - (rect.Y / 2.0f)))
		{
			return true;
		}

		return false;
	}

	if (Ray.X == 0.0f)
	{
		if ((pos.X + (rect.X / 2.0f)) > RayOrigin.X > (pos.X - (rect.X / 2.0f)))
		{
			return true;
		}

		return false;
	}

	float m = Ray.Y / Ray.X;
	float c = RayOrigin.Y - m * RayOrigin.X;

	float topIntercept = ((pos.Y + (rect.Y / 2.0f)) - c) / m;
	float botIntercept = ((pos.Y - (rect.Y / 2.0f)) - c) / m;
	float leftIntercept = m * (pos.X - (rect.X / 2.0f)) + c;
	float rightIntercept = m * (pos.X + (rect.X / 2.0f)) + c;

	if ((pos.X + (rect.X / 2.0f)) > topIntercept > (pos.X - (rect.X / 2.0f)))
	{
		return true;
	}

	if ((pos.X + (rect.X / 2.0f)) > botIntercept > (pos.X - (rect.X / 2.0f)))
	{
		return true;
	}

	if ((pos.Y + (rect.Y / 2.0f)) > leftIntercept > (pos.Y - (rect.Y / 2.0f)))
	{
		return true;
	}

	if ((pos.Y + (rect.Y / 2.0f)) > rightIntercept > (pos.Y - (rect.Y / 2.0f)))
	{
		return true;
	}

	//TODO:: Build AABB with 4 line line intersects and check the points of intersect between the corners
}

std::vector<GameObject*> Collision::Raycast(Vector2 Ray, Vector2 RayOrigin)
{
	std::vector<GameObject*> sceneObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();
	std::vector<GameObject*> collidedObjects;

	for (GameObject* obj : sceneObjects)
	{
		Rigidbody* rb = obj->GetComponent<Rigidbody>();
		if (rb == nullptr)
		{
			continue;
		}

		switch (rb->GetCollisionType())
		{
		case CollisionTypes::AABB:
			if (RaycastAABB(Ray, RayOrigin, obj))
			{
				collidedObjects.push_back(obj);
			}
			break;
		case CollisionTypes::Sphere:
			if (RaycastSphere(Ray, RayOrigin, obj))
			{
				collidedObjects.push_back(obj);
			}
			break;
		}
	}

	return collidedObjects;
}

bool Collision::Raycast(Vector2 Ray, Vector2 RayOrigin, GameObject* checkObject)
{
	std::vector<GameObject*> collidedObjects = Raycast(Ray, RayOrigin);

	if (std::find(collidedObjects.begin(), collidedObjects.end(), checkObject) != collidedObjects.end())
	{
		return true;
	}

	return false;
}

bool Collision::OnSeg(Vector2 p, Vector2 q, Vector2 r)
{
	if (q.X <= max(p.X, r.X) && q.X >= min(p.X, r.X) && 
		q.Y <= max(p.Y, r.Y) && q.Y >= min(p.Y, r.Y))
	{
		return true;
	}

	return false;
}

int Collision::Orientation(Vector2 p, Vector2 q, Vector2 r)
{
	int val = (q.Y - p.Y) * (r.X - q.X) - (q.X - p.X) * (r.Y - q.Y);

	if (val == 0) return 0;

	return (val > 0) ? 1 : 2;
}

bool Collision::DoIntersect(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2)
{
	int o1 = Orientation(p1, q1, p2);
	int o2 = Orientation(p1, q1, q2);
	int o3 = Orientation(p2, q2, p1);
	int o4 = Orientation(p2, q2, q1);

	if (o1 != o2 && o3 != o4) return true;

	if (o1 == 0 && OnSeg(p1, p2, q1)) return true;

	if (o2 == 0 && OnSeg(p1, q2, q1)) return true;

	if (o3 == 0 && OnSeg(p2, p1, q2)) return true;

	if (o4 == 0 && OnSeg(p2, q1, q2)) return true;

	return false;
}