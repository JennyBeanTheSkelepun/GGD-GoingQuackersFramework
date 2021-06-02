#include "GrapplingHook.h"
#include "../GameObject.h"
#include "AudioSource.h"
#include "../Time.h"

GrapplingHook::GrapplingHook(GameObject* owner) : Component(owner, ComponentTypes::GRAPPLINGHOOK, "Grappling Hook")
{
}

GrapplingHook::~GrapplingHook()
{
	this->~GrapplingHook();
}

void GrapplingHook::OnDestroy()
{
}

void GrapplingHook::Update()
{
	if (mp_handler == nullptr)
		return;

	//If reached max length, Stop
	if (GetHookDistance() >= m_hookRange)
		return;

	Vector2 fireVector = m_fireDirection * m_fireSpeed * Time::GetDeltaTime();
	GetOwner()->GetTransform()->SetPosition(GetOwner()->GetTransform()->GetPosition() + fireVector);
}

void GrapplingHook::ImGUIUpdate()
{
	ImGui::InputFloat("Fire Speed", &m_fireSpeed);
	ImGui::InputFloat("Hook Range", &m_hookRange);
}

json* GrapplingHook::SceneSave()
{
	json* returnObj = new json({
		{"Fire Speed", m_fireSpeed},
		{"Hook Range", m_hookRange}
		});

	return returnObj;
}

void GrapplingHook::SceneLoad(json* componentJSON)
{
	m_fireSpeed = (*componentJSON)["Fire Speed"];
	m_hookRange = (*componentJSON)["Hook Range"];
}

void GrapplingHook::Fire(Vector2 targetPos, GameObject* handler)
{
	mp_handler = handler;

	//Calculates the Direction towards the Target Position
	Vector2 handlerPos = mp_handler->GetTransform()->GetPosition();
	Vector2 direction = targetPos - handlerPos;
	direction.Normalize();
	m_fireDirection = direction;

	//Play Sound Effect
	GetOwner()->GetComponent<AudioSource>()->Play();
}

float GrapplingHook::GetHookDistance()
{
	Vector2 hookPos = GetOwner()->GetTransform()->GetPosition();
	Vector2 handlerPos = mp_handler->GetTransform()->GetPosition();
	return handlerPos.Distance(hookPos);
}
