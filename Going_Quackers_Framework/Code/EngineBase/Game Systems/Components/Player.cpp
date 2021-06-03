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
	GameObject* grapplingHook = SceneManager::GetInstance()->GetCurrentScene()->GetObjectByName("Grappling Hook");
	if (grapplingHook == nullptr) {
		Debug::getInstance()->LogError("There is no grappling hook in the scene!");
	}
	else mp_grapplingHook = grapplingHook->GetComponent<GrapplingHook>();
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
		if (!wallGrabbed && velocity.Length() < minSpeed)
		{
			if (velocity.Length() == 0) // shouldn't happen, but just in case
			{
				velocity = (Vector2(0, minSpeed));
			}
			else {
				velocity *= (minSpeed / velocity.Length());
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

	// no state-specific entry logic
	m_grappleState = state;
}

void Player::GrappleFire(Vector2 targetPos)
{
	mp_grapplingHook->Fire(targetPos, this->GetOwner());

	SetGrappleState(GRAPPLE_STATE::EXTENDING);
	// note: moving from the extending state to the attached state is handled by the grapple
}
void Player::GrappleReturn()
{
	SetGrappleState(GRAPPLE_STATE::RETURNING);
}
void Player::GrappleRetract()
{
	mp_grapplingHook->Retract();
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
	std::vector<GameObject*> collidedObjects = playerRB->GetCollidedObjects();
	if (!collidedObjects.empty())
	{
		for (size_t i = 0; i < collidedObjects.size(); i++)
		{
			std::string objName = collidedObjects[i]->GetName();
			if (objName == "Wall")
			{
				wallObj = playerRB->GetCollidedObjects();
			}
		}
	}
	
	/*if ((m_grappleState == GRAPPLE_STATE::ATTACHED || m_grappleState == GRAPPLE_STATE::RETRACTING) && mp_grapplingHook->GetHookDistance() <= 2 && playerRB->GetCollidedObjects().empty() != true)
	if ((m_grappleState == GRAPPLE_STATE::ATTACHED || m_grappleState == GRAPPLE_STATE::RETRACTING) && mp_grapplingHook->GetHookDistance() <= 2 && wallObj.empty() != true)
	{

		wallGrabbed = true;


		Debug::getInstance()->Log(wallGrabbed);
		Debug::getInstance()->Log("wall grabbed");

	}
	else if (wallObj.empty() != true && m_grappleState == GRAPPLE_STATE::RETURNING && mp_grapplingHook->GetHookDistance()<=0.5f)
	{
		wallGrabbed = true;

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
	bounce.force = newVelocity * (minSpeed / newVelocity.Length());
	playerRB->AddForce(bounce);
	wallObj.clear();

	// cancel out default bounce
	Force negate;
	negate.force = -playerRB->GetVelocity();
	negate.moveIgnore = MovementIgnore::MASSACCEL;
	playerRB->AddForce(negate);
}