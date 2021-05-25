#include "Player.h"
#include "../Input.h"
#include "../Debug.h"

Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER, "Player")
{
	m_grappleState = GRAPPLE_STATE::INACTIVE;
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
#pragma region Grapple Controls
	// this assumes that the grapple is a separate object or objects

	// if the player is at the grapple's position, stop it from retracting more

	// aim; get cursor position

	// fire grapple on left click down
	if (GetGrappleState() == GRAPPLE_STATE::INACTIVE && Input::getInstance()->isKeyPressedDown(KeyCode::LeftMouse))
	{
		// calculate direction to cursor position
		// create grapple at player position??
		SetGrappleState(GRAPPLE_STATE::EXTENDING);
		// note: moving from the extending state to the attached state is handled by the grapple
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
}