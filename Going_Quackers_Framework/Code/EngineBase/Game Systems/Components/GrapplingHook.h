#pragma once
#include "Component.h"
#include "../../Data Structures/Vectors.h"

class GrapplingHook : public Component
{
public:
	GrapplingHook(GameObject* owner);
	~GrapplingHook();

	//- Basic Loops -//
	void OnDestroy() override;
	void Update() override;

	//- ImGui UpdateLoop -//
	void ImGUIUpdate() override;

	//- Scene Save and Load -//
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

	///<summary>Fires the Grappling Hook towards Target Position, Taking in the GameObject firing the Hook as well</summary>
	void Fire(Vector2 targetPos, GameObject* handler);

	///<summary> Returns the Distance between the Hook and the Player firing it</summary>
	float GetHookDistance();

	float GetFiringSpeed() { return m_fireSpeed; }
	float GetHookRange() { return m_hookRange; }

private:
	GameObject* mp_handler;

	Vector2 m_fireDirection; //The Direction the Grappling Hook is fired

	float m_fireSpeed; //The Firing Speed of the Grappling Hook
	float m_hookRange; //The Max range of the Grappling Hook
};