#include "Component.h"

Component::Component(GameObject* owner, ComponentTypes a_type, std::string typeName)
{
	this->mp_owner = owner;
	this->m_type = a_type;
	this->ID = rand();
	this->shouldDestroy = false;
	this->name = typeName;
}

Component::~Component()
{
}

void Component::ImGUIDisplay()
{
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		if (ImGui::Button("Delete"))
		{
			shouldDestroy = true;
		}
		ImGUIUpdate();
	}
}

GameObject* Component::GetOwner() 
{
	return mp_owner; 
}

ComponentTypes Component::GetType() 
{ 
	return m_type;
}

int Component::GetID() 
{
	return ID;
}

bool Component::ShouldDestroy() 
{
	return shouldDestroy; 
}