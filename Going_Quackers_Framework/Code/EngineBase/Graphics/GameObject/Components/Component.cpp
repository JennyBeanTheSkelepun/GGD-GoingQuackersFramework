#include "Component.h"

Component::Component(GameObject* owner)
{
	this->owner = owner;
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
