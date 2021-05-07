#include "Collision.h"
#include "Rigidbody.h"

//TODO:: CONVERT THIS TO TAKE IN CHECK OBJECT AND NOT USE COMPONENT

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
	//Statistically, this is almost always correct.
	return false;
}