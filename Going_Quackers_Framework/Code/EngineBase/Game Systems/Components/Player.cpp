#include "Player.h"

Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER,"Player") {
	m_grappleState = GRAPPLE_STATE::INACTIVE;
	wallGrabbed = false;
	playerObj = GetOwner();
}



void Player::GrabWall()
{
	//todo add rope length to the if statement
	if (GetGrappleState() == GRAPPLE_STATE::ATTACHED /*&& rope length is less than 1*/ && playerObj->GetComponent<Rigidbody>()->getCollideFlag() == true)
	{
		//set wallGrabbed to be true
		wallGrabbed = true;
		//note: wallGrabbed can be used by whoever is making the jump system to set it to false and release the grapple

		//todo get the object the player is colliding with
	}

	if (wallGrabbed)
	{

		//todo set the player to face the wall they are colliding with
	}
}