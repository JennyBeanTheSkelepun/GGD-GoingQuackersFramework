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

		if (obj1Pos.X < obj2Pos.X + WidthHeight2.X && 
			obj1Pos.X + WidthHeight1.X > obj2Pos.X &&
			obj1Pos.Y < obj2Pos.Y + WidthHeight2.Y &&
			obj1Pos.Y + WidthHeight1.Y > obj2Pos.Y)
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

	Vector2 WidthHeight1 = checkObjectA->GetComponent<Rigidbody>()->GetAABBRect();

	Vector2 maxPoint = Vector2(obj1Pos + WidthHeight1);

	Vector2 obj2Pos = checkObjectB->GetTransform()->GetPosition();

	float radius = checkObjectB->GetComponent<Rigidbody>()->GetRadius();

	closestPoint.X = max(obj1Pos.X, min(obj2Pos.X, maxPoint.X));
	closestPoint.Y = max(obj1Pos.Y, min(obj2Pos.Y, maxPoint.Y));

	float distance = sqrt(pow(closestPoint.X - obj2Pos.X, 2) + pow(closestPoint.Y - obj2Pos.Y, 2));

	//Statistically, this is almost always correct.
	return distance > radius;
}