#include "Component.h"
#include "../Debug.h"

Component::Component(GameObject* owner, ComponentTypes a_type, std::string typeName)
{
	this->mp_owner = owner;
	this->m_type = a_type;
	this->ID = rand();
	this->mb_stayAlive = true;
	this->name = typeName;
}

Component::~Component()
{
}

void Component::ImGUIDisplay()
{
	//- If not transform allow component deletion -//
	if (m_type != ComponentTypes::TRANSFORM)
	{
		if (ImGui::CollapsingHeader(name.c_str(), &mb_stayAlive))
			ImGUIUpdate();
	}
	else
	{
		if (ImGui::CollapsingHeader(name.c_str()))
			ImGUIUpdate();
	}

	ImGui::Spacing();
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
	return !mb_stayAlive; 
}