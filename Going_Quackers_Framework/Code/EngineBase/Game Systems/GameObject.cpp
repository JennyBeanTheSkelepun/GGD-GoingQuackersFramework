#include "GameObject.h"
#include "Debug.h"

#include "Components/SpriteRenderer.h"
#include "Components/Physics/Rigidbody.h"
#include "Components/VirtualCamera.h"
#include "Components/Player.h"
#include "Components/AudioSource.h"
#include "Components/SpringJoint.h"
#include "Components/LineRenderer.h"
#include "Components/Pickup.h"
#include "Components/Rope.h"
#include "Debug.h"

GameObject::GameObject(const char* name, GameObject* parent)
{
	this->m_name = name;
	this->mp_parent = parent;
	this->m_id = std::to_string(rand());

	m_components = std::vector<Component*>();
	m_children = std::vector<GameObject*>();

	this->m_shouldLive = true;

	mp_transform = AddComponent<Transform>();
	m_active = true;
}

GameObject::~GameObject()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		delete m_children[i];
	}

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
			delete m_children[i];
			m_children.erase(m_children.begin() + i);
			break;
		}

		m_children[i]->Update();
	}
}

void GameObject::ImGUIUpdate()
{
	const char* components[] = { "Sprite Renderer", "RigidBody", "Virtual Camera", "Player", "Audio Source", "Spring Joint", "Line Renderer", "Pickup"};
	const char* components[] = { "Sprite Renderer", "RigidBody", "Virtual Camera", "Player", "Audio Source", "Spring Joint", "Rope"};
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
					AddComponent<SpriteRenderer>();
					break;
				case 1:
					AddComponent<Rigidbody>();
					break;
				case 2:
					AddComponent<VirtualCamera>();
					break;
				case 3:
					AddComponent<Player>();
					break;
				case 4:
					AddComponent<AudioSource>();
					break;
				case 5:
					AddComponent<SpringJoint>();
					break;
				case 6:
					AddComponent<LineRenderer>();
					break;
				case 7:
					AddComponent<Pickup>();
					break;
		
				case 6:
					AddComponent<Rope>();
					break;
				default:
						Debug::getInstance()->LogError("Component Type Not Recognized");
					break;
				}
			}
		}

		ImGui::EndPopup();
	}


	ImGui::SetNextItemWidth(175);
	ImGui::InputText("", (char*)m_name.c_str(), 50); ImGui::SameLine();
	ImGui::Checkbox("Active", &m_active);

	if (ImGui::Button("Create Component"))
		ImGui::OpenPopup("Component List");
	ImGui::SameLine();
	if (ImGui::Button("Delete GameObject"))
	{
		SetToDestroy();
		return;
	}

	ImGui::Separator();
	if (m_components.size() == 0)
	{
		ImGui::Text("No Components On Gameobject");
		ImGui::Spacing();
	}
	else
	{
		for (size_t i = 0; i < m_components.size(); i++)
		{
			m_components[i]->ImGUIDisplay();
		}
	}
	ImGui::Separator();
}

void GameObject::SetToDestroy()
{
	m_shouldLive = false;
	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->SetToDestroy();
	}
}
