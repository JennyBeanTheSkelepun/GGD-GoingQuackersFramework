#include "Collision.h"
#include "Rigidbody.h"

//TODO:: CONVERT THIS TO TAKE IN CHECK OBJECT AND NOT USE COMPONENT

std::vector<GameObject*> Collision::CollisionSpherical(GameObject* checkObject)
{
	std::vector<GameObject*> testingObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();

	std::vector<GameObject*> collidedObjects;

	for(GameObject* obj : testingObjects)
	{
		if(obj != checkObject && obj->GetComponent<Rigidbody>() != nullptr);
		{
			float radA = GetRadius();
			float radB = obj->GetComponent<Rigidbody>()->getCollider()->GetRadius();

			float distance = (checkObject->GetTransform()->GetPosition() - obj->GetTransform()->GetPosition()).Length();

			if (distance <= (radA + radB))
			{
				collidedObjects.push_back(obj);
			}
		}
	}

	return collidedObjects;
}

std::vector<GameObject*> Collision::CollisionAABB(GameObject* checkObject)
{
	std::vector<GameObject*> testingObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();

	std::vector<GameObject*> collidedObjects;

	for (GameObject* obj : testingObjects)
	{
		if (obj != checkObject && obj->GetComponent<Rigidbody>() != nullptr);
		{
			Vector2 obj1Pos = obj->GetTransform()->GetPosition();

			Vector2 obj2Pos = checkObject->GetTransform()->GetPosition();

			Vector2 WidthHeight1 = obj->GetComponent<Rigidbody>()->getCollider()->GetAABBRect();
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

	return collidedObjects;
}