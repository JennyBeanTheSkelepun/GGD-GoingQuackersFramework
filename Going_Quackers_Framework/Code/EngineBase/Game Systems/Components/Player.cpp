#include "Player.h"

Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER) {
	grabbed = false;
	//Component rigidbody = owner->GetComponent(RIGIDBODY);
}

void Player::GrabWall()
{
	//todo make an if statement that checks if the player is both colliding with something and that the grappling hook rope is very small/the hook is a certain distance to the player and sets grabbed to true


	if (grabbed)
	{
		//todo get the object the player is colliding with
		//todo set the player to face the wall they are colliding with
		//todo set the players to be constantly moving towards the wall it's attached to
	}
}