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
		return nullptr;
	}

	GameObject* GetParent() { return mp_parent; }
	void SetParent(GameObject* newParent) { mp_parent = newParent; mp_parent->AddChild(this); }

	void AddChild(GameObject* child) { m_children.push_back(child); }
	std::vector<GameObject*> GetChildren() { return m_children; }

	Transform* GetTransform() { return mp_transform; }

	bool IsActive() { return m_active; }
	void SetActive(bool value) { m_active = value; }
private:
	Transform* mp_transform;

	//A List of Components for the GameObject
	std::vector<Component*> m_components;

	std::vector<GameObject*> m_children;

	//The parent of the current GameObject
	GameObject* mp_parent;

	//Is the GameObject currently updating and rendering?
	bool m_active;
};

#endif // !_GAMEOBJECT_H_