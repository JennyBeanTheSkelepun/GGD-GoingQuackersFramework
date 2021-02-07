#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <d3d11.h>
#include <memory>
#include <vector>

#include "../Texture/TextureClass.h"
#include "../GameObject/Components/Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();
	void Render(ID3D11DeviceContext* deviceContext);

	///<summary>Retuns a component from the list in the gameobject.</summary>
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
	}

	///<summary>Removes a component from the entity, if multiple of same type of components are in play it removes the first one in the list IE the first one added to it.</summary>
	template<class T>
	T* AddComponent()
	{
		if (std::is_base_of<Component, T>::value)
		{
			m_components.push_back(static_cast<Component*>(new T(this)));
			return static_cast<T*>(m_components[m_components.size() - 1]);
		}
	}

	///<summary>Removes a component from the entity, if multiple of same type of components are in play it removes the first one in the list IE the first one added to it.</summary>
	template<class T>
	void RemoveComponent()
	{
		for (size_t i = 0; i < m_components.size(); i++)
		{
			if (dynamic_cast<T*>(m_components[i]) != nullptr)
			{
				m_components.erase(m_components.begin(), m_components.begin() + i);
				return; //Only delete first instance
			}
		}
	}

	void SetActive(bool value) { this->active = value; }

private:
	std::vector<Component*> m_components;

	bool active;
};

#endif // !_GAMEOBJECT_H_