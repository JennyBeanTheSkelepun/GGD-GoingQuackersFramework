#include "Component.h"

void Component::Initialize(GameObject* parent)
{
	this->parent = parent;
}

void Component::Update()
{
	if (!active)
		return;
}

void Component::Render(ID3D11DeviceContext* deviceContext)
{
	if (!active)
		return;
}

void Component::SetActive(bool active)
{
	this->active = active;
}

GameObject* Component::GetParent()
{
	return parent;
}
