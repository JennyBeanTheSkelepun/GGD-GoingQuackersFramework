#include "Player.h"
#include "../Input.h"
#include "Transform.h"
#include "../Debug.h"
#include "../Time.h"


Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER, "Player")
{
	m_grappleState = GRAPPLE_STATE::ATTACHED;
	wallGrabbed = true;

	wallPushPressed = false;
	wallPushCollided = false;
	wallPushPressTimer = 0.f;
	wallPushCollideTimer = 0.f;

	playerObj = this->GetOwner();
	playerRB = playerObj->GetComponent<Rigidbody>();
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
	// don't do anything in editor mode
	if (EngineGuiClass::getInstance()->IsInPlayMode())
	{
		HandleInput();
		GrabWall();

		// check collision for pushing off a wall
		if (playerRB->GetCollidingBool() && !wallPushCollided)
		{
			wallPushCollided = true;
			wallPushCollideTimer = wallPushTimerMax;
			if (wallObj.size() == 0)
				wallObj = playerRB->GetCollidedObjects();
		}
		if (wallPushPressed && wallPushCollided)
		{
			WallPush();
		}

		// minimum speed; todo adjust value
		Vector2 velocity = playerRB->GetVelocity();
		if (!wallGrabbed && velocity.Length() < 0.001 && velocity.Length() != 0)
		{
			velocity *= (0.001 / velocity.Length());
			playerRB->SetVelocity(velocity);
		}
	}
}

void Player::ImGUIUpdate()
{
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
	Vector3 mousePos = Input::getInstance()->GetWorldSpaceMousePos();

	// don't rotate if grappling hook is out
	if (GetGrappleState() == GRAPPLE_STATE::INACTIVE)
	{
		// aim grapple at cursor position; rotate to face it
		Transform* playerTransform = playerObj->GetComponent<Transform>();
		Vector2 playerPos = playerTransform->GetPosition();

		// calculate vectors, relative to the player
		Vector2 mouseVector = Vector2(mousePos.X, mousePos.Y) - playerPos;
		Vector2 upVector = Vector2(0, 1); // magnitude is always 1
		float mouseMagnitude = mouseVector.Length();

		// just don't change the angle if it would divide by zero
		if (mouseMagnitude != 0)
		{
			// use dot product and determinant
			mouseVector.Normalize();
			upVector.Normalize();
			float dot = mouseVector.Dot(upVector);
			float determinant = mouseVector.X * upVector.Y - mouseVector.Y * upVector.X;
			float angle = std::atan2f(determinant, dot);
			// radians to degrees
			angle *= 180 / 3.1415;
			playerObj->GetComponent<Transform>()->SetLocalRotation(angle);
		}
	}
#pragma endregion

#pragma region Grapple Controls
	// fire loose grapple on left click down
	if (GetGrappleState() == GRAPPLE_STATE::INACTIVE && Input::getInstance()->isKeyPressedDown(KeyCode::LeftMouse))
	{
		GrappleFire(Vector2(mousePos.X, mousePos.Y));
	}

	// release attached grapple on left click down
	if (GetGrappleState() == GRAPPLE_STATE::ATTACHED && Input::getInstance()->isKeyPressedDown(KeyCode::LeftMouse))
	{
		GrappleReturn();
	}

	// cancel grapple extension on left click up
	if (GetGrappleState() == GRAPPLE_STATE::EXTENDING && Input::getInstance()->isKeyPressedUp(KeyCode::LeftMouse))
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
		if (wallPushPressTimer < 0)
			wallPushPressed = false;
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
	Force pushForce;

	// average of collided objects' positions
	Vector2 collPos = Vector2(0, 0);
	if (wallObj.size() > 0) // should always be at least one, but just in case
	{
		for (int i = 0; i < wallObj.size(); i++)
		{
			collPos += wallObj[i]->GetTransform()->GetPosition();
		}
		collPos /= wallObj.size();
	}

	// vector from collided position to player
	Vector2 wallToPlayer = collPos - playerObj->GetTransform()->GetPosition();
	pushForce.force = wallToPlayer;
	pushForce.force.Normalize();

	// pushing off a wall from a standstill
	if (wallGrabbed)
	{
		wallGrabbed = false;
		playerRB->setStatic(false);
		if (GetGrappleState() == GRAPPLE_STATE::ATTACHED || GetGrappleState() == GRAPPLE_STATE::RETRACTING)
		{
			// if grapple is short
			SetGrappleState(GRAPPLE_STATE::INACTIVE);
			// if grapple is long, set to retracting
		}
	}
	else // pushing off a wall while moving
	{
		pushForce.force *= playerObj->GetComponent<Rigidbody>()->GetVelocity().Length();
		pushForce.force * 5.f; // get faster
	}

	// move player away
	playerRB->AddForce(pushForce);
	
	// cleanup
	wallPushPressed = false;
	wallPushCollided = false;
	wallObj.clear();
}

void Player::GrabWall()
{
	if (wallGrabbed == false)
	{
		if (playerObj->GetComponent<AudioSource>() != nullptr)
		{
			playerObj->GetComponent<AudioSource>()->Stop();
		}

	}
	if (playerObj->GetComponent<Rigidbody>() == nullptr)
	{
		Debug::getInstance()->LogError("the player doesn't have a rigidbody component");
	}
	//todo add rope length to the if statement
	else if (m_grappleState == GRAPPLE_STATE::ATTACHED /*&& rope length is less than 1*/ && playerObj->GetComponent<Rigidbody>()->GetCollidedObjects().empty() != true)
	{

		wallGrabbed = true;

		//this gets the object the player is colliding with and puts it into a variable to use 
		wallObj = playerObj->GetComponent<Rigidbody>()->GetCollidedObjects();

		Debug::getInstance()->Log(wallGrabbed);
		Debug::getInstance()->Log("wall grabbed");

	}
	else if (playerObj->GetComponent<Rigidbody>()->GetCollidedObjects().empty() != true && m_grappleState == GRAPPLE_STATE::RETURNING /*&& rope length is less than 1*/)
	{
		wallGrabbed = true;

		wallObj = playerObj->GetComponent<Rigidbody>()->GetCollidedObjects();
		Debug::getInstance()->Log(wallGrabbed);
		Debug::getInstance()->Log("wall grabbed");
	}
	else
	{
		//for testing
		Debug::getInstance()->Log("wall not grabbed");
	}


	if (wallGrabbed)
	{
		//this takes the wall objects the player has collided with and finds the one that it is currently colliding with 
		for (GameObject* obj : wallObj)
		{
			Rigidbody* playerRigidbody = playerObj->GetComponent<Rigidbody>();

			if (playerRigidbody == nullptr)
			{
				return;
			}

			//this is meant to keep the player at the same position by moving them towards the wall with a small force
			Vector2 vectorBetweenPlayerAndWall = (obj->GetTransform()->GetPosition() + obj->GetComponent<Rigidbody>()->GetAABBRect()) - playerObj->GetTransform()->GetPosition();
			Vector2 directionForPlayer = vectorBetweenPlayerAndWall.Normalize();
			Vector2 force = directionForPlayer;
			Force realForce;
			realForce.force = force;
			realForce.moveIgnore = MovementIgnore::ACCEL;
			Debug::getInstance()->Log(force);
			playerRigidbody->AddForce(realForce);
			if (vectorBetweenPlayerAndWall.Length() <= 1)
			{
				playerRigidbody->setStatic(true);
				Debug::getInstance()->Log("attached");
				if (playerObj->GetComponent<AudioSource>() == nullptr)
				{
					Debug::getInstance()->LogError("the player doesn't have a audio source component");
				}
				else
				{
					playerObj->GetComponent<AudioSource>()->Play();
				}
			}
		}

	}
}