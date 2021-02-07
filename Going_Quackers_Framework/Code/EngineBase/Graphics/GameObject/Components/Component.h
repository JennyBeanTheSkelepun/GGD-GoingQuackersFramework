#pragma once
#include <d3d11.h>
#include <directxmath.h>

class GameObject;

class Component
{
public:
	Component(GameObject* object);
	~Component();

	virtual void Initialize();

	virtual void Update();
	virtual void Render(ID3D11DeviceContext* deviceContext);

	GameObject* GetOwner() { return object; }

protected:
	GameObject* object;
};