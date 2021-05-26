#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include "../../ImGui/EngineGuiClass.h"

#include "../../JSON/nlohmann/json.hpp" // Adding JSON for modern C++
// For Convienience
using json = nlohmann::json;

class GameObject;

enum class ComponentTypes
{
	SPRITE = 0,
	TRANSFORM = 1,
	SPRITERENDERER = 2,
	RIGIDBODY = 3,
	VIRTUALCAMERA = 4,
	PLAYER = 5,
	SPRINGJOINTS = 6
};

class Component
{
public:
	Component(GameObject* owner, ComponentTypes a_type, std::string typeName);
	~Component();
	
	virtual void OnDestroy() = 0;
	virtual void Update() = 0;
	virtual void ImGUIUpdate() = 0;
	virtual json* SceneSave() = 0;
	virtual void SceneLoad(json* componentJSON) = 0;
	
	void ImGUIDisplay();
	GameObject* GetOwner();
	ComponentTypes GetType();
	int GetID();
	///<summary>Checks if the Component should be destroyed & removed from the GameObject</summary>
	bool ShouldDestroy();

protected:
	GameObject* mp_owner;
	ComponentTypes m_type;

	int ID; //The ID of the Component. Used to determine the correct Component within the GameObjects list
	bool mb_stayAlive;
	std::string name;
};

#endif