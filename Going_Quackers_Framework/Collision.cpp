#include "Collision.h"

std::vector<GameObject*> Collision::CollisionSpherical()
{
	std::vector<GameObject*> testingObjects; //replace with scenegraph object list

	std::vector<GameObject*> collidedObjects;

	for(GameObject* obj : testingObjects)
	{
		if(obj != this->GetOwner() && obj->GetComponent<Collision>() != nullptr);
		{
			float radA = GetRadius();
			float radB = obj->GetComponent<Collision>()->GetRadius();

			float distance = (this->GetOwner()->GetTransform()->GetPosition() - obj->GetTransform()->GetPosition()).Length();

			if (distance <= (radA + radB))
			{
				collidedObjects.push_back(obj);
			}
		}
	}
}

std::vector<GameObject*> Collision::CollisionAABB()
{
	std::vector<GameObject*> testingObjects; //replace with scenegraph object list

	std::vector<GameObject*> collidedObjects;

	for (GameObject* obj : testingObjects)
	{
		if (obj != this->GetOwner() && obj->GetComponent<Collision>() != nullptr);
		{
			Vector2 obj1Pos = obj->GetTransform()->GetPosition();

			Vector2 obj2Pos = this->GetOwner()->GetTransform()->GetPosition();

			Vector2 WidthHeight1 = obj->GetComponent<Collision>()->GetAABBRect();
			Vector2 WidthHeight2 = GetAABBRect();

			if (obj1Pos.X < obj2Pos.X + WidthHeight2.X && 
				obj1Pos.X + WidthHeight1.X > obj2Pos.X &&
				obj1Pos.Y < obj2Pos.Y + WidthHeight2.Y &&
				obj1Pos.Y + WidthHeight1.Y > obj2Pos.Y)
			{
				collidedObjects.push_back(obj);
			}
		}
	}
}