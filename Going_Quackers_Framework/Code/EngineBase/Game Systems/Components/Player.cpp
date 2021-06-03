#include "Player.h"
#include "../Input.h"
#include "Transform.h"
#include "../Debug.h"
#include "../Time.h"


Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER, "Player")
{
	m_grappleState = GRAPPLE_STATE::INACTIVE;
	wallGrabbed = true;
	canBounce = false;

	wallPushPressed = false;
	wallPushCollided = false;
	wallPushPressTimer = 0.f;
	wallPushCollideTimer = 0.f;

	playerObj = this->GetOwner();
	if (playerObj->GetComponent<Rigidbody>() == nullptr)
	{
		Debug::getInstance()->LogError("The player doesn't have a Rigidbody component!");
	}
	else playerRB = playerObj->GetComponent<Rigidbody>();
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

		if (playerRB->GetCollidingBool())
		{
			wallPushCollided = true;
			wallPushCollideTimer = wallPushTimerMax;
			if (wallObj.size() == 0)
				wallObj = playerRB->GetCollidedObjects();
			if (!wallGrabbed) canBounce = true;
		}

		if (wallPushPressed && wallPushCollided)
		{
			WallPush();
			canBounce = false;
		}

		if (canBounce) Bounce();

		// minimum speed
		Vector2 velocity = playerRB->GetVelocity();
		if (!wallGrabbed && velocity.Length() < 0.05f)
		{
			if (velocity.Length() == 0) // shouldn't happen, but just in case
			{
				velocity = (Vector2(0, 0.05f));
			}
			else {
				velocity *= (0.05f / velocity.Length());
				playerRB->SetVelocity(velocity);
			}
		}
	}
}

void Player::ImGUIUpdate()
{
}

json* Player::SceneSave()
{
	json* returnObj = new json({
		{"Grappling Hook's State", m_grappleState},
		{"Wall Grabbed", wallGrabbed},
		{"Wall Push Variables",
			{{"Pressed", wallPushPressed}, {"Collided", wallPushCollided},
			{"PressedTimer", wallPushPressTimer},{"CollidedTimer",wallPushCollideTimer}}
		}
	});

	return returnObj;
}

void Player::SceneLoad(json* componentJSON)
{
	m_grappleState = (*componentJSON)["Grappling Hook's State"];
	wallGrabbed = (*componentJSON)["Wall Grabbed"];
	wallPushPressed = (*componentJSON)["Wall Push Variables"]["Pressed"];
	wallPushCollided = (*componentJSON)["Wall Push Variables"]["Collided"];
	wallPushPressTimer = (*componentJSON)["Wall Push Variables"]["PressedTimer"];
	wallPushCollideTimer = (*componentJSON)["Wall Push Variables"]["CollidedTimer"];
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

	if (GetGrappleState() != GRAPPLE_STATE::RETURNING)
	{
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
	GameObject* grapplingHook = SceneManager::GetInstance()->GetCurrentScene()->GetObjectByID("GameObject 24464");
	mp_grapplingHook = grapplingHook->GetComponent<GrapplingHook>();
	mp_grapplingHook->Fire(targetPos, this->GetOwner());

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
	mp_grapplingHook->Retract();
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
	Vector2 wallToPlayer = playerObj->GetTransform()->GetPosition() - collPos;
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
		pushForce.force *= startSpeed;
	}
	else
	{
		// maintain speed
		pushForce.force *= playerObj->GetComponent<Rigidbody>()->GetVelocity().Length();
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
	
	//todo add rope length to the if statement
	if ((m_grappleState == GRAPPLE_STATE::ATTACHED || m_grappleState == GRAPPLE_STATE::RETRACTING)/*&& rope length is less than 1*/ && playerRB->GetCollidedObjects().empty() != true)
	{

		wallGrabbed = true;

		//this gets the object the player is colliding with and puts it into a variable to use 
		wallObj = playerRB->GetCollidedObjects();

		Debug::getInstance()->Log(wallGrabbed);
		Debug::getInstance()->Log("wall grabbed");

	}
	else if (playerRB->GetCollidedObjects().empty() != true && m_grappleState == GRAPPLE_STATE::RETURNING /*&& rope length is less than 1*/)
	{
		wallGrabbed = true;

		wallObj = playerRB->GetCollidedObjects();
		Debug::getInstance()->Log(wallGrabbed);
		Debug::getInstance()->Log("wall grabbed");
	}


	if (wallGrabbed)
	{
		//this takes the wall objects the player has collided with and finds the one that it is currently colliding with 
		for (GameObject* obj : wallObj)
		{
			if (playerRB == nullptr)
			{
				return;
			}

			//this is meant to keep the player at the same position by moving them towards the wall with a small force
			Vector2 vectorBetweenPlayerAndWall = playerObj->GetTransform()->GetPosition() - (obj->GetTransform()->GetPosition() + obj->GetComponent<Rigidbody>()->GetAABBRect());
			Vector2 directionForPlayer = vectorBetweenPlayerAndWall.Normalize();
			if (vectorBetweenPlayerAndWall.Length() > 1)
			{
				Vector2 force = directionForPlayer;
				Force realForce;
				realForce.force = force;
				realForce.moveIgnore = MovementIgnore::ACCEL;
				Debug::getInstance()->Log(force);
				playerRB->AddForce(realForce);
			}
			if (vectorBetweenPlayerAndWall.Length() <= 1 && !playerRB->getIsStatic()) // only attach once
			{
				playerRB->setStatic(true);
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

void Player::Die()
{
	// todo play a sound

	// reset player on death
	playerObj->GetTransform()->SetPosition(Vector2(0, 0));
	playerObj->GetTransform()->SetRotation(0);
	playerRB->SetVelocity(Vector2(0, 0));

	SetGrappleState(GRAPPLE_STATE::ATTACHED);
	wallGrabbed = true;
	wallPushPressed = false;
	wallPushCollided = false;
	wallObj.clear();
}

void Player::Bounce()
{
	canBounce = false;

	// average of collided objects
	Vector2 midpoint = Vector2(0, 0);
	Vector2 midScale = Vector2(0, 0);
	for (int i = 0; i < wallObj.size(); i++)
	{
		
		midpoint += wallObj[i]->GetTransform()->GetPosition();
		midScale += wallObj[i]->GetComponent<Rigidbody>()->GetAABBRect();
	}
	midpoint /= wallObj.size();
	midScale /= wallObj.size();

	// figure out which quadrant the player is in
	Vector2 deltaPos = playerObj->GetTransform()->GetPosition() - midpoint;
	Vector2 axis;
	if ((deltaPos.Y > (midpoint.Y + midScale.Y)) || deltaPos.Y < (midpoint.Y -midScale.Y)) // above or below
	{
		// side is a horizontal line
		axis = Vector2(1, 0);
	}
	else
		axis = Vector2(0, 1); 

	// figure out angle between axis and relative player position
	float angle = axis.Dot(deltaPos) / (axis.Length() * deltaPos.Length());
	angle = acosf(angle);
	angle *= 180 / 3.1415;
	float rAngle = angle;
	if (angle > 90.f) rAngle = 180 - angle;

	Vector2 newVelocity = playerRB->GetVelocity();
	if (rAngle > bounceMinAngle)
	{
		// reflect player velocity by perpendicular axis
		if (axis.X == 1) newVelocity.X = -newVelocity.X;
		else newVelocity.Y = -newVelocity.Y;
		
		// lerp to decrease speed based on proximity to perpendicular
		float proximity = rAngle/90;
		float reductionFactor = 1 - (proximity * bounceSpeedLoss);
		newVelocity *= reductionFactor;
	}
	else
	{
		// set velocity to +/- axis; glide
		if (angle < 90.f) newVelocity = -axis;
		else newVelocity = axis;
	}
	Force bounce;
	// bring force up to minimum velocity
	bounce.force = newVelocity * (0.05f / newVelocity.Length());
	playerRB->AddForce(bounce);
	wallObj.clear();

	// cancel out default bounce
	Force negate;
	negate.force = -playerRB->GetVelocity();
	negate.moveIgnore = MovementIgnore::MASSACCEL;
	playerRB->AddForce(negate);
}