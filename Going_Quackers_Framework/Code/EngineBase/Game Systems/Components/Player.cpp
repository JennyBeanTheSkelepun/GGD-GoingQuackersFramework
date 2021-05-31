#include "Player.h"
#include "../Input.h"
#include "Transform.h"
#include "Physics/Rigidbody.h"
#include "../Debug.h"
#include "../Time.h"


Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER, "Player")
{
	m_grappleState = GRAPPLE_STATE::INACTIVE;
	wallGrabbed = false;
	wallPushPressed = false;
	wallPushCollided = false;
	wallPushPressTimer = 0.f;
	wallPushCollideTimer = 0.f;
	playerObj = this->GetOwner();
}

Player::~Player()
{
	playerObj = nullptr;
}

void Player::OnDestroy()
{
	this->~Player();
}

void Player::Update()
{
	// ignore input in editor mode
	if (EngineGuiClass::getInstance()->IsInPlayMode())
	{
		HandleInput();
		GrabWall();

		// check collision for pushing off a wall
		if (playerObj->GetComponent<Rigidbody>()->GetCollidingBool())
		{
			wallPushCollided = true;
			wallPushCollideTimer = wallPushTimerMax;
		}
		if (wallPushPressed && wallPushCollided)
		{
			WallPush();
		}
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

void Player::HandleInput()
{
#pragma region Look At Cursor
	// aim grapple at cursor position; rotate to face it
	Transform* playerTransform = playerObj->GetComponent<Transform>();
	Vector3 mousePos = Input::getInstance()->GetWorldSpaceMousePos();
	Vector2 playerPos = playerTransform->GetPosition();

	// calculate vectors, centered on the centre of the player
	Vector2 mouseVector = Vector2(mousePos.X, mousePos.Y) - playerPos;
	Vector2 upVector = Vector2(playerPos.X, playerPos.Y + 1);
	float mouseMagnitude = mouseVector.Length();
	float upMagnitude = upVector.Length();

	// just don't change the angle if it would divide by zero
	if (mouseMagnitude != 0 && upMagnitude != 0)
	{
		// use dot product and determinant
		Vector2 mouseNormal = mouseVector.Normalize();
		Vector2 upNormal = upVector.Normalize();
		float dot = mouseNormal.Dot(upNormal);
		float determinant = mouseNormal.X * upNormal.Y - mouseNormal.Y * upNormal.X;
		float angle = std::atan2f(determinant, dot);
		// radians to degrees
		angle *= 180 / 3.1415;
		playerObj->GetComponent<Transform>()->SetLocalRotation(angle);
	}
#pragma endregion

#pragma region Grapple Controls
	// this assumes that the grapple is a separate object or objects

	// fire grapple on left click down
	if (GetGrappleState() == GRAPPLE_STATE::INACTIVE && Input::getInstance()->isKeyPressedDown(KeyCode::LeftMouse))
	{
		GrappleFire(Vector2(mousePos.X, mousePos.Y));
	}

	// release grapple on left click up
	if ((GetGrappleState() == GRAPPLE_STATE::EXTENDING || GetGrappleState() == GRAPPLE_STATE::ATTACHED) && Input::getInstance()->isKeyPressedUp(KeyCode::LeftMouse))
	{
		GrappleReturn();
		// note: moving from the returning state to the inactive state is handled by the grapple
	}

	// start retracting grapple on right click down
	if (GetGrappleState() == GRAPPLE_STATE::ATTACHED && Input::getInstance()->isKeyPressedDown(KeyCode::RightMouse))
	{
		GrappleRetract();
		SetGrappleState(GRAPPLE_STATE::RETRACTING);
	}

	// continue retracting if right click is held; stop if it's released (rope stays connected)
	if (GetGrappleState() == GRAPPLE_STATE::RETRACTING)
	{
		if (Input::getInstance()->isKeyHeldDown(KeyCode::RightMouse))
		{
			GrappleRetract();
		}
		else if (Input::getInstance()->isKeyPressedUp(KeyCode::RightMouse))
		{
			SetGrappleState(GRAPPLE_STATE::ATTACHED);
		}
	}

#pragma endregion

	// count down wall-pushing input buffer timers
	if (wallPushPressed)
	{
		wallPushPressTimer -= Time::GetDeltaTime() * 1000;
		if (wallPushPressTimer < 0) {
			wallPushPressed = false;
			Debug::getInstance()->Log("keypress ran out!");
		}
	}
	if (wallPushCollided)
	{
		wallPushCollideTimer -= Time::GetDeltaTime() * 1000;
		if (wallPushCollideTimer < 0)
			wallPushCollided = false;
	}

	// if near a wall, press space to push off it
	if (Input::getInstance()->isKeyPressedDown(KeyCode::Space))
	{
		wallPushPressed = true;
		wallPushPressTimer = wallPushTimerMax;
	}
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
void Player::GrappleReturn()
{
	// tell grapple to shrink in the direction of the player
	SetGrappleState(GRAPPLE_STATE::RETURNING);
}
void Player::GrappleRetract()
{
	// move player towards rope end
	// tell rope to shrink?
	Debug::getInstance()->Log("retracting grapple");
}

void Player::WallPush()
{
	wallGrabbed = false;
	playerObj->GetComponent<Rigidbody>()->setStatic(false);

	// move player away
	
	wallPushPressed = false;
	wallPushCollided = false;
}

void Player::GrabWall()
{
}