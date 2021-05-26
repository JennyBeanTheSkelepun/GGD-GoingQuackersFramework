#include "Player.h"

Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER,"Player") {
	m_grappleState = GRAPPLE_STATE::INACTIVE;
	wallGrabbed = false;
	playerObj = GetOwner();
}



void Player::GrabWall()
{
	//todo add rope length to the if statement
	if (m_grappleState == GRAPPLE_STATE::ATTACHED /*&& rope length is less than 1*/ && playerObj->GetComponent<Rigidbody>()->getCollideFlag() == true)
	{
		//set wallGrabbed to be true
		wallGrabbed = true;
		//note: wallGrabbed can be used by whoever is making the jump system to set it to false and release the grapple

		//this gets the objects the player has coollided with and puts them into a variable to use 
		wallObj = playerObj->GetComponent<Rigidbody>()->getCollidedObjects();
	}

	if (wallGrabbed)
	{
		//this takes the wall objects the player has collided with and finds the one that it is currently colliding with 
		for (GameObject* obj : wallObj)
		{
			if (obj->GetComponent<Rigidbody>()->getCollideFlag() == true)
			{
				//one possible way of keeping the player at the same position is by keeping them moving towards it with a small force which is what this chunk of code is doing
				Vector2 vectorBetweenPlayerAndWall = obj->GetTransform()->GetPosition() - playerObj->GetTransform()->GetPosition();
				Vector2 directionForPlayer = vectorBetweenPlayerAndWall.Normalize();
				Vector2 force = directionForPlayer * 0.1;
				playerObj->AddComponent<Rigidbody>()->AddForce(force);


			}
		}

	}
}