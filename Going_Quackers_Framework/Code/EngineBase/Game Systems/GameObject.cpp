#include "GameObject.h"
#include "Components/SpriteRenderer.h"
#include "Components/SpringJoint.h"
#include "Components/Physics/Rigidbody.h"
#include "Debug.h"

GameObject::GameObject(const char* name, GameObject* parent)
{
	this->m_name = name;
	this->mp_parent = parent;
	this->m_id = std::to_string(rand());

	m_components = std::vector<Component*>();
	m_children = std::vector<GameObject*>();

	this->m_shouldDestroy = false;

	Initialize();
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		DeleteComponent(m_components[i]);
	}

	mp_transform = nullptr;
	mp_parent = nullptr;
}

void GameObject::Initialize()
{
	mp_transform = AddComponent<Transform>();

	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Initialize();
	}

	m_active = true;
}

void GameObject::Update()
{
	if (!m_active)
		return;

	for (size_t i = 0; i < m_components.size(); i++)
	{
		Component* component = m_components[i];
		
		if (!component->ShouldDestroy())
			component->Update();

		if (component->ShouldDestroy())
		{
			m_components.erase(m_components.begin() + i);
			DeleteComponent(component);
			break;
		}
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->ShouldDestroy())
		{
			m_children.erase(m_children.begin() + i);
			break;
		}

		m_children[i]->Update();
	}
}

void GameObject::Render()
{
	if (!m_active)
		return;

	for (size_t i = 0; i < m_components.size(); i++)
	{
		if (!m_components[i]->ShouldDestroy())
			m_components[i]->Render();
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Render();
	}
}

void GameObject::ImGUIUpdate()
{
	ImGui::InputText("", (char*)m_name.c_str(), 50);
	
	bool active = m_active;
	ImGui::Checkbox("Active", &active);
	SetActive(active);

	ImGui::Separator();

	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->ImGUIDisplay();
	}

	ImGui::Separator();

	if (ImGui::Button("Delete GameObject"))
	{
		SetToDestroy();
		return;
	}

	ImGui::Separator();

	if (ImGui::Button("Create Component"))
	{
		ImGui::OpenPopup("Component List");
	}

	const char* components[] = { "Transform", "Sprite Renderer", "RigidBody", "Spring Joint" };
	int selectedComponent = -1;
	if (ImGui::BeginPopup("Component List"))
	{
		for (size_t i = 0; i < IM_ARRAYSIZE(components); i++)
		{
			if (ImGui::Selectable(components[i]))
			{
				switch (i)
				{
					case 0:
						AddComponent<Transform>();
					break;

					case 1:
						AddComponent<SpriteRenderer>();
					break;

					case 2:
						AddComponent<Rigidbody>();
					break;

					case 3:
						AddComponent<SpringJoint>();
						break;
				}
			}
		}

		ImGui::EndPopup();
	}	
}

void GameObject::SetToDestroy()
{
	m_shouldDestroy = true;
	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->SetToDestroy();
	}
}

void GameObject::DeleteComponent(Component* component)
{
	switch (component->GetTag())
	{
	case ComponentTypes::SPRITERENDERER:
		delete static_cast<SpriteRenderer*>(component);
		break;
	case ComponentTypes::RIGIDBODY:
		delete static_cast<Rigidbody*>(component);
		break;
	case ComponentTypes::TRANSFORM:
		delete static_cast<Transform*>(component);
		break;
	case ComponentTypes::SPRINGJOINT:
		delete static_cast<SpringJoint*>(component);
		break;
	default:
		Debug::getInstance()->LogWarning("ERROR TRYING TO DELETE A UNSUPORTED COMPONENT");
		break;
	}
}
