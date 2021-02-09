#pragma once

class GameObject;

class Component
{
public:
	Component(GameObject* owner);
	~Component();

	virtual void Initialize();
	virtual void Update();
	virtual void Render();

	GameObject* GetOwner() { return owner; }
private:
	GameObject* owner;
};