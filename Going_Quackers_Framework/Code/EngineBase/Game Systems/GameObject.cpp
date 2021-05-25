#include "GameObject.h"
#include "Debug.h"

#include "Components/SpriteRenderer.h"
#include "Components/Physics/Rigidbody.h"
#include "Components/VirtualCamera.h"

GameObject::GameObject(const char* name, GameObject* parent)
{
	this->m_name = name;
	this->mp_parent = parent;
	this->m_id = std::to_string(rand());

	m_components = std::vector<Component*>();
	m_children = std::vector<GameObject*>();

	this->m_shouldDestroy = false;

	mp_transform = AddComponent<Transform>();
	m_active = true;
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		m_components[i]->OnDestroy();
		delete m_components[i];
	}

	mp_transform = nullptr;
	mp_parent = nullptr;
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
			component->OnDestroy();
			delete component;
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

	const char* components[] = { "Transform", "Sprite Renderer", "RigidBody", "Virtual Camera" };
	int selectedComponent = -1;
	if (ImGui::BeginPopup("Component List"))
	{
		for (size_t i = 0; i < IM_ARRAYSIZE(components); i++)
		{
			if (ImGui::Selectable(components[i]))
			{

				//- This should be the only hardcoded connection stating type of component -//
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
						AddComponent<VirtualCamera>();
						break;
					default:
						Debug::getInstance()->LogError("Component Type Not Recgonized");
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