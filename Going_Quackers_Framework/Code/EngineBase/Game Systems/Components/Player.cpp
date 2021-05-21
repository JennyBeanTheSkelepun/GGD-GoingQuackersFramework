#include "Player.h"

Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER) {
	m_grappleState = GRAPPLE_STATE::INACTIVE;
}

void Player::ImGUIDisplay()
{
	const char* name = "Player";

	if (ImGui::CollapsingHeader(name))
	{
		if (ImGui::Button("Delete"))
		{
			shouldDestroy = true;
		}

		ImGUIUpdate();
	}
}

void Player::Update()
{
#pragma region Grapple Controls
	// this assumes that the grapple is a separate object or objects

	// if the player is at the grapple's position, stop it from retracting more

	// aim; get cursor position

	// fire grapple on left click down
	if (GetGrappleState() == GRAPPLE_STATE::INACTIVE && false)
	{
		// calculate direction to cursor position
		// create grapple at player position??
		SetGrappleState(GRAPPLE_STATE::EXTENDING);
		// note: moving from the extending state to the attached state is handled by the grapple
	}

	// release grapple on left click up
	if ((GetGrappleState() == GRAPPLE_STATE::EXTENDING || GetGrappleState() == GRAPPLE_STATE::ATTACHED) && false)
	{
		// tell grapple to shrink in the direction of the player
		SetGrappleState(GRAPPLE_STATE::RETURNING);
		// note: moving from the returning state to the inactive state is handled by the grapple
	}

	// start retracting grapple on right click down
	if ((GetGrappleState() == GRAPPLE_STATE::EXTENDING || GetGrappleState() == GRAPPLE_STATE::ATTACHED) && false)
	{
		// move player towards grapple
		// tell grapple to shrink?
		SetGrappleState(GRAPPLE_STATE::RETRACTING);
	}

	// continue retracting if right click is held
	if (GetGrappleState() == GRAPPLE_STATE::RETRACTING && false)
	{
		// same as entering the grapple state; move movement logic into separate function?
	}
#pragma endregion

	// if colliding with a wall, press space and move player away from it

}

