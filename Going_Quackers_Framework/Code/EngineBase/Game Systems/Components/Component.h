#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "../../ImGui/EngineGuiClass.h"

class GameObject;

enum ComponentTypes
{
	SPRITE = 0,
	TRANSFORM = 1,
	SPRITERENDERER = 2,
	RIGIDBODY = 3,

	//Not_Usefull = 0,
	//Two_D_Sprite_Renderer = 1,
	//Three_D_Object_Renderer = 2,
	//TileSheet_Sprite_Renderer = 3,
	//Virtual_Camera = 4
};

class Component
{
public:
	Component(GameObject* owner, ComponentTypes a_type)
	{
		this->mp_owner = owner;
		this->m_type = a_type;
	}

	~Component()
	{
		OnDestroy();
	}

	virtual void Initialize()
	{
	}

	///<summary>Called on the Components Destruction. Use to remove necessary data</summary>
	virtual void OnDestroy()
	{
	}

	virtual void Update()
	{
	}

	virtual void Render()
	{
	}

	///<summary>Called each frame for ImGUI Inputs. Display variables of Components here.</summary>
	virtual void ImGUIUpdate()
	{
	}

	///<summary>Called on Saving the Scene. Use to save information about the Component
	virtual void OnSave()
	{
	}

	///<summary>Called on Loading the Scene. Use to load in saved information about the Component
	virtual void OnLoad()
	{
	}

	///<summary>Returns the GameObject owner of the Component</summary>
	GameObject* GetOwner() { return mp_owner; }
	ComponentTypes GetTag() { return m_type; }

protected:
	GameObject* mp_owner;
	ComponentTypes m_type;
};

#endif