#include "Player.h"

Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER,"Player") {
	m_grappleState = GRAPPLE_STATE::ATTACHED; //changed to attached for testing
	wallGrabbed = false;
	playerObj = GetOwner();
}

Player::~Player()
{
}

void Player::OnDestroy()
{
	this->~Player();
}

void Player::ImGUIUpdate()
{
	
}

void Player::Update()
{
	GrabWall();
}

void Player::SceneLoad(json* componentJSON)
{
	/*ElementVar1 = (*componentJSON)["ElementName1"];
	ElementVar2 = (*componentJSON)["ElementName2"];*/
}

json* Player::SceneSave()
{
	/*json* returnObj = new json({
		{"ElementName1", ElementVar1},
		{"ElementName2", ElementVar2}
		});

	return returnObj;*/
	return nullptr;
}

void Player::GrabWall()
{
	if (playerObj->GetComponent<Rigidbody>()==nullptr)
	{
		Debug::getInstance()->LogError("the player doesn't have a rigidbody component");
	}
	//todo add rope length to the if statement
	else if (m_grappleState == GRAPPLE_STATE::ATTACHED /*&& rope length is less than 1*/ && playerObj->GetComponent<Rigidbody>()->GetCollideFlag() == true)
	{
		wallGrabbed = true;
		//note: wallGrabbed can be used by whoever is making the jump system to set it to false and release the grapple

		//this gets the object the player is coolliding with and puts it into a variable to use 
		wallObj = playerObj->GetComponent<Rigidbody>()->GetCollidedObjects();
		Debug::getInstance()->Log(wallGrabbed);
		
	}
	else
	{
		//for testing
		Debug::getInstance()->Log(playerObj->GetComponent<Rigidbody>()->GetCollideFlag());
		/*Vector2* testvector
		testvector.X = 1;
		testvector.Y = 0;
		playerObj->AddComponent<Rigidbody>()->AddForce(testvector);*/
	}


	if (wallGrabbed)
	{
		//this takes the wall objects the player has collided with and finds the one that it is currently colliding with 
		for (GameObject* obj : wallObj)
		{
			Rigidbody* playerRigidbody = playerObj->AddComponent<Rigidbody>();

			//this is meant to keep the player at the same position by moving them towards the wall with a small force
			Vector2 vectorBetweenPlayerAndWall = obj->GetTransform()->GetPosition() - playerObj->GetTransform()->GetPosition();
			float distance = vectorBetweenPlayerAndWall.Length();
			Vector2 directionForPlayer = vectorBetweenPlayerAndWall.Normalize();
			Vector2 force = directionForPlayer * (distance/2.0f);
			//playerRigidbody->AddForce(-force);


			
		}

	}
}