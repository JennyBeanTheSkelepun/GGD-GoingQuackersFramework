#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GameObject;

class Component
{
public:
	Component(GameObject* owner);
	~Component();

	virtual void Initialize();
	virtual void Update();
	virtual void Render();

	GameObject* GetOwner() { return mp_owner; }

protected:
	GameObject* mp_owner;
};

#endif