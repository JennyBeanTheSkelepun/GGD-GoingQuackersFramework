#include "GameObject.h"
#include "../../Time/Time.h"

GameObject::GameObject()
{
	Initialize();
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		delete m_components[i];
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		delete m_children[i];
	}
}

void GameObject::Initialize()
{
	mp_transform = AddComponent<Transform>();

	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Initialize();
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Initialize();
	}
}

void GameObject::Update()
{
	Vector2 newPosition = mp_transform->GetPosition();
	Vector2 offset = Vector2(0.001f, 0);
	//mp_transform->SetPosition(newPosition + offset);

	mp_transform->SetRotation((mp_transform->GetRotation() + (0.1f *  Time::GetDeltaTime())));
	//mp_transform->SetScale(mp_transform->GetScale() + Vector2(0.0001f, 0.0001f, 0.0f));

	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Update();
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Update();
	}
}

void GameObject::Render()
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Render();
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}
}
