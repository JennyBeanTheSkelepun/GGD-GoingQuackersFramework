#include "GrapplingHook.h"
#include "../GameObject.h"
#include "AudioSource.h"
#include "Physics/Rigidbody.h"
#include "../Time.h"
#include "Player.h"

GrapplingHook::GrapplingHook(GameObject* owner) : Component(owner, ComponentTypes::GRAPPLINGHOOK, "Grappling Hook")
{
	m_hit = false;
	m_fired = false;
	m_retractMinimum = 0.5f;
}

GrapplingHook::~GrapplingHook()
{
}

void GrapplingHook::OnDestroy()
{
	this->~GrapplingHook();
}

void GrapplingHook::Update()
{
	if (mp_handler == nullptr)
		return;

	if (!m_fired)
		return;

	//If reached max length, Stop
	if (GetHookDistance() >= m_hookRange)
	{
		if (mp_handler->GetComponent<Player>()->GetGrappleState() != Player::GRAPPLE_STATE::INACTIVE)
			mp_handler->GetComponent<Player>()->SetGrappleState(Player::GRAPPLE_STATE::RETURNING);
	}

	if (!(CheckForWallCollision()))
	{
		Vector2 fireVector = m_fireDirection * m_fireSpeed * Time::GetDeltaTime();
		GetOwner()->GetTransform()->SetPosition(GetOwner()->GetTransform()->GetPosition() + fireVector);
	}
	else
	{
		HitWall();
	}

	if (mp_handler->GetComponent<Player>()->GetGrappleState() == Player::GRAPPLE_STATE::RETRACTING)
	{
		if (GetHookDistance() <= m_retractMinimum)
		{
			ResetHook();
			mp_handler->GetComponent<Player>()->wallGrabbed = true;
		}
	}
	else if (mp_handler->GetComponent<Player>()->GetGrappleState() == Player::GRAPPLE_STATE::RETURNING)
	{
		Vector2 direction = mp_handler->GetTransform()->GetPosition() - GetOwner()->GetTransform()->GetPosition();
		direction.Normalize();

		Vector2 returnVector = direction * m_returnSpeed * Time::GetDeltaTime();		
		GetOwner()->GetTransform()->SetPosition(GetOwner()->GetTransform()->GetPosition() + returnVector);

		if (GetHookDistance() <= m_retractMinimum)
		{
			ResetHook();
		}
	}
}

void GrapplingHook::ImGUIUpdate()
{
	ImGui::InputFloat("Fire Speed", &m_fireSpeed);
	ImGui::InputFloat("Hook Range", &m_hookRange);
	ImGui::InputFloat("Retract Speed", &m_retractSpeed);
	ImGui::InputFloat("Retract Minimum", &m_retractMinimum);
	ImGui::InputFloat("Return Speed", &m_returnSpeed);
}

json* GrapplingHook::SceneSave()
{
	json* returnObj = new json({
		{"Fire Speed", m_fireSpeed},
		{"Hook Range", m_hookRange},
		{"Retract Speed", m_retractSpeed},
		{"Retract Minimum", m_retractMinimum},
		{"Return Speed", m_returnSpeed}
		});

	return returnObj;
}

void GrapplingHook::SceneLoad(json* componentJSON)
{
	m_fireSpeed = (*componentJSON)["Fire Speed"];
	m_hookRange = (*componentJSON)["Hook Range"];
	m_retractSpeed = (*componentJSON)["Retract Speed"];
	m_retractMinimum = (*componentJSON)["Retract Minimum"];
	m_returnSpeed = (*componentJSON)["Return Speed"];
}

void GrapplingHook::Fire(Vector2 targetPos, GameObject* handler)
{
	mp_handler = handler;

	//Calculates the Direction towards the Target Position
	Vector2 handlerPos = mp_handler->GetTransform()->GetPosition();
	Vector2 direction = targetPos - handlerPos;
	direction.Normalize();
	m_fireDirection = direction;

	m_fired = true;

	//Play Sound Effect
	GetOwner()->GetComponent<AudioSource>()->Play();
}

void GrapplingHook::Retract()
{
	Force retract;
	retract.force = m_fireDirection;
	retract.force.Normalize();
	mp_handler->GetComponent<Rigidbody>()->AddForce(retract);
}

void GrapplingHook::Return()
{

}

void GrapplingHook::ResetHook()
{
	mp_handler->GetComponent<Player>()->SetGrappleState(Player::GRAPPLE_STATE::INACTIVE);
	m_hit = false;
	m_fired = false;
	GetOwner()->GetTransform()->SetPosition(mp_handler->GetTransform()->GetPosition());
	GetOwner()->GetComponent<AudioSource>()->SetAudioPath("Hook_Shoot.wav"); //Set Audio Back
}

float GrapplingHook::GetHookDistance()
{
	Vector2 hookPos = GetOwner()->GetTransform()->GetPosition();
	Vector2 handlerPos = mp_handler->GetTransform()->GetPosition();
	return handlerPos.Distance(hookPos);
}

bool GrapplingHook::CheckForWallCollision()
{
	Vector2 hookPosition = GetOwner()->GetTransform()->GetPosition();
	std::vector<GameObject*> collidingObjects = GetOwner()->GetComponent<Rigidbody>()->GetCollidedObjects();
	for (size_t i = 0; i < collidingObjects.size(); i++)
	{
		std::string objName = collidingObjects[i]->GetName();
		if (objName == "Wall")
		{
			// inside a wall? move out of it
			Vector2 objSize = collidingObjects[i]->GetTransform()->GetScale() / 2;
			Vector2 objPos = collidingObjects[i]->GetTransform()->GetPosition();
			bool Xstuck = false;
			bool Ystuck = false;
			if (hookPosition.X < objPos.X + objSize.X && hookPosition.X > objPos.X - objSize.X)
				Xstuck = true;
			if (hookPosition.Y < objPos.Y + objSize.Y && hookPosition.Y > objPos.Y - objSize.Y)
				Ystuck = true;
			if (Xstuck && Ystuck)
			{
				float deltaX = hookPosition.X - objPos.X;
				if (deltaX < 0) deltaX *= -1;
				float deltaY = hookPosition.Y - objPos.Y;
				if (deltaY < 0) deltaY *= -1;
				Vector2 ownSize = GetOwner()->GetTransform()->GetScale() / 2;
				if (deltaX > deltaY)
				{
					if (hookPosition.X - objPos.X > 0) hookPosition.X = objPos.X + objSize.X + ownSize.X;
					else hookPosition.X = objPos.X - objSize.X - ownSize.X;
				}
				else
				{
					if (hookPosition.Y - objPos.Y > 0) hookPosition.Y = objPos.Y + objSize.Y + ownSize.Y;
					else hookPosition.Y = objPos.Y - objSize.Y - ownSize.Y;
				}
			}
			GetOwner()->GetTransform()->SetPosition(hookPosition);
			return true;
		}
	}

	return false;
}

void GrapplingHook::HitWall()
{
	if (!m_hit)
	{
		m_hit = true;
		mp_handler->GetComponent<Player>()->SetGrappleState(Player::GRAPPLE_STATE::ATTACHED);

		//Change and Play Audio Hit Sound
		GetOwner()->GetComponent<AudioSource>()->SetAudioPath("Hook_Hit.wav");
		GetOwner()->GetComponent<AudioSource>()->Play();
	}
}
