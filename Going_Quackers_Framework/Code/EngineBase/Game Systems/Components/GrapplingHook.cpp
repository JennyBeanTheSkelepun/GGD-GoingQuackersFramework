#include "GrapplingHook.h"

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
}

void GrapplingHook::ImGUIUpdate()
{
}

json* GrapplingHook::SceneSave()
{
	return nullptr;
}

void GrapplingHook::SceneLoad(json* componentJSON)
{
}
