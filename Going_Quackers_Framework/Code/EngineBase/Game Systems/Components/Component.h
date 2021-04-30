#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "../../ImGui/EngineGuiClass.h"

class GameObject;

enum ComponentTypes
{
	SPRITE = 0,
	TRANSFORM = 1,
	SPRITERENDERER = 2,

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
		Deconstruct();
	}

	virtual void Initialize()
	{
	}

	virtual void Deconstruct()
	{
	}

	virtual void Update()
	{
	}

	virtual void Render()
	{
	}

	virtual void ImGUIUpdate()
	{

	}

	///<summary>Returns the GameObject owner of the Component</summary>
	GameObject* GetOwner() { return mp_owner; }

protected:
	GameObject* mp_owner;
	ComponentTypes m_type;
};

#endif