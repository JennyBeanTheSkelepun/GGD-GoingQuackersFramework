#include "Player.h"
#include "../Input.h"
#include "../GameObject.h"
#include "Transform.h"
#include "../Debug.h"


Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER, "Player")
{
	m_grappleState = GRAPPLE_STATE::INACTIVE;
	grabbed = false;
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
#pragma region Look At Cursor
	// aim grapple at cursor position; rotate to face it
	Transform* playerTransform = this->GetOwner()->GetComponent<Transform>();
	Vector3 mousePos = Input::getInstance()->GetWorldSpaceMousePos();
	Vector2 playerPos = playerTransform->GetPosition();

	// calculate vectors, centered on the centre of the player
	Vector2 origin = Vector2(playerPos.X + playerTransform->GetScale().X / 2, playerPos.Y + playerTransform->GetScale().Y / 2);
	Vector2 mouseVector = Vector2(mousePos.X, mousePos.Y) - origin;
	Vector2 upVector = Vector2(origin.X, origin.Y + 1);
	float mouseLength = mouseVector.Length();
	float upLength = upVector.Length();

	// just don't change the angle if it would divide by zero
	if (mouseLength != 0 && upLength != 0)
	{
		/*// use dot product over magnitude
		float dot = upVector.Dot(mouseVector);
		float angle = std::acosf(dot / (mouseLength * upLength));
		// radians to degrees
		angle *= 180 / 3.1415;
		// flip sign
		if (mouseVector.X > origin.X) angle *= -1;
		this->GetOwner()->GetComponent<Transform>()->SetLocalRotation(angle);*/

		// use dot product and determinant
		Vector2 mouseNormal = mouseVector.Normalize();
		Vector2 upNormal = upVector.Normalize();
		float dot = upNormal.Dot(mouseNormal);
		float determinant = upNormal.X * mouseNormal.Y - upNormal.Y * mouseNormal.X;
		float angle = std::atan2f(determinant, dot);
		// radians to degrees
		angle *= 180 / 3.1415;
		this->GetOwner()->GetComponent<Transform>()->SetLocalRotation(angle);
	}
#pragma endregion

#pragma region Grapple Controls
	// this assumes that the grapple is a separate object or objects

	// fire grapple on left click down
	if (GetGrappleState() == GRAPPLE_STATE::INACTIVE && Input::getInstance()->isKeyPressedDown(KeyCode::LeftMouse))
	{
		GrappleFire(Vector2(mousePos.X,mousePos.Y));
	}

	// release grapple on left click up
	if ((GetGrappleState() == GRAPPLE_STATE::EXTENDING || GetGrappleState() == GRAPPLE_STATE::ATTACHED) && Input::getInstance()->isKeyPressedUp(KeyCode::LeftMouse))
	{
		// tell grapple to shrink in the direction of the player
		SetGrappleState(GRAPPLE_STATE::RETURNING);
		// note: moving from the returning state to the inactive state is handled by the grapple
	}

	// start retracting grapple on right click down
	if ((GetGrappleState() == GRAPPLE_STATE::EXTENDING || GetGrappleState() == GRAPPLE_STATE::ATTACHED) && Input::getInstance()->isKeyPressedDown(KeyCode::RightMouse))
	{
		// move player towards grapple
		// tell grapple to shrink?
		SetGrappleState(GRAPPLE_STATE::RETRACTING);
	}

	// continue retracting if right click is held; stop if it's released
	if (GetGrappleState() == GRAPPLE_STATE::RETRACTING)
	{
		if (Input::getInstance()->isKeyHeldDown(KeyCode::RightMouse))
		{
			// continue moving player towards grapple, grapple shrinks; move movement logic into separate function?
			Debug::getInstance()->Log("retracting grapple");
		}
		else if (Input::getInstance()->isKeyPressedUp(KeyCode::RightMouse))
		{
			SetGrappleState(GRAPPLE_STATE::ATTACHED);
		}
	}

#pragma endregion

	// if colliding with a wall, press space and move player away from it
	if (grabbed && Input::getInstance()->isKeyPressedDown(KeyCode::Space))
	{
		// move player away
		grabbed = false;
	}
}

void Player::ImGUIUpdate()
{
	// todo remove this later? used for debugging
	switch (m_grappleState)
	{
	case GRAPPLE_STATE::INACTIVE:
		ImGui::Text("Grapple: Not Attached, Not Moving");
		break;
	case GRAPPLE_STATE::ATTACHED:
		ImGui::Text("Grapple: Attached, Not Moving");
		break;
	case GRAPPLE_STATE::EXTENDING:
		ImGui::Text("Grapple: Not Attached, Grapple Moving To Target");
		break;
	case GRAPPLE_STATE::RETRACTING:
		ImGui::Text("Grapple: Attached, Player Moving To Target");
		break;
	case GRAPPLE_STATE::RETURNING:
		ImGui::Text("Grapple: Not Attached, Grapple Moving To Player");
		break;
	default:
		ImGui::Text("UNKNOWN GRAPPLE STATE");
		break;
	}
}

json* Player::SceneSave()
{
	json* returnObj = new json({
		{"Grappling Hook's State", m_grappleState}
		});

	return returnObj;
}

void Player::SceneLoad(json* componentJSON)
{
	m_grappleState = (*componentJSON)["Grappling Hook's State"];
}

void Player::SetGrappleState(Player::GRAPPLE_STATE state)
{
	// ignore if it's trying to change to the same state
	if (state == m_grappleState) return;

	// any state-specific entry logic
	switch (state)
	{
	case GRAPPLE_STATE::EXTENDING:
		Debug::getInstance()->Log("Player: fire grapple");
		break;
	case GRAPPLE_STATE::ATTACHED:
		Debug::getInstance()->Log("Player: grapple is attached and not unmoving");
		break;
	case GRAPPLE_STATE::RETRACTING:
		Debug::getInstance()->Log("Player: start retracting grapple");
		break;
	case GRAPPLE_STATE::RETURNING:
		Debug::getInstance()->Log("Player: release grapple");
		break;
	}
	m_grappleState = state;
}

void Player::GrappleFire(Vector2 targetPos)
{
	// calculate direction to cursor position
	Vector2 playerPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();

	// create grapple at player position?? pass the direction to the grapple??

	SetGrappleState(GRAPPLE_STATE::EXTENDING);
	// note: moving from the extending state to the attached state is handled by the grapple
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