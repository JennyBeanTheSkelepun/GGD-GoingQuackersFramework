#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <Windows.h>
#include <vector>

#include "Components/Component.h"
#include "Components/Transform.h"

class GameObject
{
public:
	GameObject() : GameObject("Unnamed-Object", nullptr) {};
	GameObject(const char* name) : GameObject(name, nullptr) {};
	GameObject(const char* name, GameObject* parent);

	~GameObject();

	void Initialize();
	void Update();
	void Render();

	///<summary>The Transform attached to the GameObject.</summary>
	Transform* transform;
	///<summary>The name given to the GameObject</summary>
	std::string name;

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

	///<summary>Sets the parent for the GameObject.</summary>
	void SetParent(GameObject* newParent) { mp_parent = newParent; mp_parent->AddChild(this); }
	///<summary>Returns the parent of the GameObject.</summary>
	GameObject* GetParent() { return mp_parent; }

	///<summary>Assigns the GameObject as the parent of the given child.</summary>
	void AddChild(GameObject* child) { m_children.push_back(child); }

	///<summary>Activate/Deactivate the GameObject depending on the given true/false value.</summary>
	void SetActive(bool value) { m_active = value; }
	///<summary>Returns if the GameObject is currently active (Updating/Rendering).</summary>
	bool IsActive() { return m_active; }
private:
	//A list of Components for the GameObject
	std::vector<Component*> m_components;

	//A list of Children that the GameObject is a parent of
	std::vector<GameObject*> m_children;

	//The parent of the GameObject
	GameObject* mp_parent;

	//Is the GameObject currently updating and rendering?
	bool m_active;
};

#endif // !_GAMEOBJECT_H_