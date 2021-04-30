#include "GameObject.h"

GameObject::GameObject(const char* name, GameObject* parent)
{
	this->name = name;
	this->mp_parent = parent;

	m_components = std::vector<Component*>();
	m_children = std::vector<GameObject*>();

	Initialize();
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_components.size(); i++)
	{
		delete m_components[i];
	}

	transform = nullptr;
	mp_parent = nullptr;
}

void GameObject::Initialize()
{
	transform = AddComponent<Transform>();

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
		m_components[i]->Update();
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Update();
	}
}

void GameObject::Render()
{
	if (!m_active)
		return;

	for (size_t i = 0; i < m_components.size(); i++)
	{
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
		m_components[i]->ImGUIUpdate();
	}
}
