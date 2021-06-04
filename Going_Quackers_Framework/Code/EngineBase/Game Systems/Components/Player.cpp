#include "Player.h"
#include "../Input.h"
#include "Transform.h"
#include "../Debug.h"
#include "../Time.h"


Player::Player(GameObject* owner) : Component(owner, ComponentTypes::PLAYER, "Player")
{
	m_grappleState = GRAPPLE_STATE::INACTIVE;
	wallGrabbed = false;
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
			std::vector<GameObject*> allObj = playerRB->GetCollidedObjects();
			wallObj.clear();
			for (int i = 0; i < allObj.size(); i++)
			{
				if (allObj[i]->GetComponent<Rigidbody>()->GetType() != PhysicsTypes::RB) continue;
				wallObj.push_back(allObj[i]);
			}
			if (wallObj.size() > 0)
			{
				wallPushCollided = true;
				wallPushCollideTimer = wallPushTimerMax;
				if (!wallGrabbed)
					canBounce = true;
			}
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
			if (velocity.Length() != 0)
			{
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
		}

		// stop retracting if right click is released (rope stays connected)
		if (GetGrappleState() == GRAPPLE_STATE::RETRACTING &&  Input::getInstance()->isKeyPressedUp(KeyCode::RightMouse))
		{
			// slow down movement, but don't stop
			/*Vector2 velocity = playerRB->GetVelocity();
			velocity *= (minSpeed / velocity.Length());
			playerRB->SetVelocity(velocity);*/
			playerRB->ClearForces(); // just stop.
			playerRB->SetVelocity(Vector2(0, 0));
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
	
	m_grappleState = state;
}

void Player::GrappleFire(Vector2 targetPos)
{
	if (mp_grapplingHook == nullptr)
	{
		std::vector<GameObject*> sceneObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();

		for (size_t i = 0; i < sceneObjects.size(); i++) {
			//TODO - Remove string cleaning section when name GetName bug is fixed
			std::string objectName = sceneObjects[i]->GetName();
			int garbageStart = objectName.find('\0');

			if (garbageStart != std::string::npos)
				objectName.erase(objectName.find('\0'));
			//End of string cleaning

			if (sceneObjects[i]->GetComponent<GrapplingHook>() != nullptr) {
				
					mp_grapplingHook = sceneObjects[i]->GetComponent<GrapplingHook>();

				return;
			}
		}
		if (mp_grapplingHook == nullptr) {
			Debug::getInstance()->LogError("There is no grappling hook in the scene!");
			return;
		}
		
	}
	mp_grapplingHook->Fire(targetPos, this->GetOwner());

	SetGrappleState(GRAPPLE_STATE::EXTENDING);
}
void Player::GrappleReturn()
{
	SetGrappleState(GRAPPLE_STATE::RETURNING);
}
void Player::GrappleRetract()
{
	if (playerRB->getIsStatic()) playerRB->setStatic(false);
	SetGrappleState(GRAPPLE_STATE::RETRACTING);
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
	if (!wallGrabbed)
	{
		if (playerObj->GetComponent<AudioSource>() != nullptr)
		{
			playerObj->GetComponent<AudioSource>()->Stop();
		}

	}
	else if (!playerRB->getIsStatic())
	{
		std::vector<GameObject*> collidedObjects = playerRB->GetCollidedObjects();
		for (size_t i = 0; i < collidedObjects.size(); i++)
		{
			std::string objName = collidedObjects[i]->GetName();
			if (objName == "Wall")
			{
				wallObj.push_back(collidedObjects[i]);
			}
		}
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

void Player::Die()
{
	// todo play a sound

	// reset player on death
	playerObj->GetTransform()->SetPosition(Vector2(0, 0));
	playerObj->GetTransform()->SetRotation(0);
	playerRB->ClearForces();
	playerRB->SetVelocity(Vector2(0, 0));

	mp_grapplingHook->ResetHook();
	wallGrabbed = false;
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
	if ((deltaPos.Y > midScale.Y) || deltaPos.Y < -midScale.Y) // above or below
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