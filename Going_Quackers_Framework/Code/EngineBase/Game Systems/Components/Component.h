#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GameObject;

enum ComponentTypes
{
	Not_Usefull = 0,
	Two_D_Sprite_Renderer = 1,
	Three_D_Object_Renderer = 2,
	TileSheet_Sprite_Renderer = 3,
	Virtual_Camera = 4
};

class Component
{
public:
	Component(GameObject* owner)
	{
		this->mp_owner = owner;
	}

	~Component()
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

	virtual void ImGUIUpdate()
	{
	}

	GameObject* GetOwner() { return mp_owner; }

protected:
	GameObject* mp_owner;
	ComponentTypes type;
};

#endif