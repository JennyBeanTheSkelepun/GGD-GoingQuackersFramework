#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
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
