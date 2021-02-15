#include "GameObject.h"
#include "Time.h"

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