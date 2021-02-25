#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GameObject;

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
};

#endif