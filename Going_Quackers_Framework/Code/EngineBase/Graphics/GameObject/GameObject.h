#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <Windows.h>
#include <vector>

#include "Components/Component.h"
#include "Components/Transform.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Initialize();
	void Update();
	void Render();

	///<summary>Adds a Component to the entity, requires that the Component need no initial parameters other than reference to it's object.</summary>
	template<class T>
	T* AddComponent()
	{
		if(std::is_base_of<Component, T>::value)
		{
			m_components.push_back(static_cast<Component*>(new T(this)));
			return static_cast<T*>(m_components[m_components.size() - 1]);
		}
	}

	///<summary>Retuns a Component from the list in the gameobject.</summary>
	template<class T>
	T* GetComponent()
	{
		for (size_t i = 0; i < m_components.size(); i++)
		{
			if (dynamic_cast<T*>(m_components[i]) != nullptr)
			{
				return static_cast<T*>(m_components[i]);
			}
		}

		//Component not found
		//MessageBox(hwnd, "Error", L"Error", MB_OK));
		return nullptr;
	}

	Transform* GetTransform() { return mp_transform; }
private:
	Transform* mp_transform;

	std::vector<Component*> m_components;
	std::vector<GameObject*> m_children;
};

#endif // !_GAMEOBJECT_H_