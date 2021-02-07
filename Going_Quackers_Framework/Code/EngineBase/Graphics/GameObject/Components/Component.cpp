#include "Component.h"

Component::Component(GameObject* object)
{
	this->object = object;
}

Component::~Component()
{
}

void Component::Initialize()
{
}

void Component::Update()
{
}

void Component::Render(ID3D11DeviceContext* deviceContext)
{
}
