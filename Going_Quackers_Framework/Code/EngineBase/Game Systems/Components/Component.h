#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include "../../ImGui/EngineGuiClass.h"

#include "../../JSON/nlohmann/json.hpp" // Adding JSON for modern C++
// For Convienience
using json = nlohmann::json;

class GameObject;

enum ComponentTypes
{
	SPRITE = 0,
	TRANSFORM = 1,
	SPRITERENDERER = 2,
	RIGIDBODY = 3,
};

class Component
{
public:
	Component(GameObject* owner, ComponentTypes a_type)
	{
		this->mp_owner = owner;
		this->m_type = a_type;
		this->ID = rand();
		this->shouldDestroy = false;
	}

	~Component()
	{
		OnDestroy();
	}

	virtual void OnDestroy()
	{

	}

	virtual void Initialize()
	{
	}

	virtual void Update()
	{
	}

	virtual void Render()
	{
	}

	void ImGUIDisplay()
	{
		const char* name = "";
		switch (GetTag())
		{
		case ComponentTypes::SPRITERENDERER:
			name = "Sprite Renderer";
			break;

		case ComponentTypes::TRANSFORM:
			name = "Transform";
			break;

		case ComponentTypes::RIGIDBODY:
			name = "RigidBody";
			break;
		}

		if (ImGui::CollapsingHeader(name))
		{
			if (ImGui::Button("Delete"))
			{
				shouldDestroy = true;
			}

			ImGUIUpdate();
		}
	}

	virtual void ImGUIUpdate()
	{
	}

	virtual json* SceneSave()
	{
		return nullptr;
	}

	virtual void SceneLoad(json* componentJSON)
	{
	}

	GameObject* GetOwner() { return mp_owner; }
	ComponentTypes GetTag() { return m_type; }
	int GetID() { return ID; }

	///<summary>Checks if the Component should be destroyed & removed from the GameObject</summary>
	bool ShouldDestroy() { return shouldDestroy; }

protected:
	GameObject* mp_owner;
	ComponentTypes m_type;

	int ID; //The ID of the Component. Used to determine the correct Component within the GameObjects list
	bool shouldDestroy;
};

#endif