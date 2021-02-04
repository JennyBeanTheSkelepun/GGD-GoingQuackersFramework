#pragma once
#include <d3d11.h>

class GameObject;

class Component
{
protected:
	GameObject* parent;
	
	bool active;

public:
	virtual void Initialize(GameObject* parent);
	virtual void Update();
	virtual void Render(ID3D11DeviceContext* deviceContext);

	void SetActive(bool active);

	GameObject* GetParent();
};