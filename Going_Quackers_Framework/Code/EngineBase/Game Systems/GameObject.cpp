#include "GameObject.h"

#include "Components/SpriteRenderer.h"
#include "Components/Physics/Rigidbody.h"

GameObject::GameObject(const char* name, GameObject* parent)
{
	this->name = name;
	this->mp_parent = parent;

	m_components = std::vector<Component*>();
	m_children = std::vector<GameObject*>();

	this->shouldDestroy = false;

	Initialize();
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		delete m_components[i];
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
			
			switch (component->GetTag())
			{
			case ComponentTypes::SPRITERENDERER:
				delete static_cast<SpriteRenderer*>(component);
				break;
			default:
				delete component;
				break;
			}
			
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
	if (ImGui::BeginTable("", 2))
	{
		ImGui::TableNextColumn(); ImGui::Checkbox("Active", &m_active);
		ImGui::TableNextColumn(); ImGui::InputText("Name", (char*)name.c_str(), 50);

		ImGui::EndTable();
	}

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

	const char* components[] = { "Transform", "Sprite Renderer", "RigidBody" };
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
				}
			}
		}

		ImGui::EndPopup();
	}	
}

void GameObject::SetToDestroy()
{
	shouldDestroy = true;
	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->SetToDestroy();
	}
}
