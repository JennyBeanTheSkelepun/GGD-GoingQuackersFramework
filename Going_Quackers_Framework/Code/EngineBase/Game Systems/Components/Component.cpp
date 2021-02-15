#include "Component.h"

Component::Component(GameObject* owner)
{
	this->mp_owner = owner;
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

void Component::Render()
{
}
