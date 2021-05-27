#include "Player.h"

Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER,"Player") {
	m_grappleState = GRAPPLE_STATE::INACTIVE;
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


void Player::Update()
{
	GrabWall();
}

void Player::GrabWall()
{
	//todo add rope length to the if statement
	if (m_grappleState == GRAPPLE_STATE::ATTACHED /*&& rope length is less than 1*/ && playerObj->GetComponent<Rigidbody>()->getCollideFlag() == true)
	{
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
				//this is meant to keep the player at the same position by moving them towards the wall with a small force
				Vector2 vectorBetweenPlayerAndWall = obj->GetTransform()->GetPosition() - playerObj->GetTransform()->GetPosition();
				Vector2 directionForPlayer = vectorBetweenPlayerAndWall.Normalize();
				Vector2 force = directionForPlayer * 0.1;
				playerObj->AddComponent<Rigidbody>()->AddForce(force);


			}
		}

	}
}