#include "GameObject.h"

GameObject::GameObject()
{
	active = true;
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		delete m_components[i];
	}
}

void GameObject::Update()
{
	if (!active)
		return;

	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Update();
	}
}

void GameObject::Render(ID3D11DeviceContext* deviceContext)
{
	if (!active)
		return;

	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Render(deviceContext);
	}
}