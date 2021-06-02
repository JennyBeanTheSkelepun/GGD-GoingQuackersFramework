#ifndef _GRAPPINGHOOK_H_
#define _GRAPPINGHOOK_H_

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

	///<summary>Retracts the Grappling Hook to pull the Player Closer. Must of been Fired first</summary>
	void Retract();

	///<summary>Resets the Hooks Propoties to allow it to be Re-Fired</summary>
	void ResetHook();

	///<summary> Returns the Distance between the Hook and the Player firing it</summary>
	float GetHookDistance();

	float GetFiringSpeed() { return m_fireSpeed; }
	float GetHookRange() { return m_hookRange; }

	///<summary>Returns if the Grappling Hook has hooked onto a object</summary>
	bool IsHooked() { return m_hit; } 

private:
	bool CheckForWallCollision();
	void HitWall();

private:
	GameObject* mp_handler;

	Vector2 m_fireDirection; //The Direction the Grappling Hook is fired

	float m_fireSpeed; //The Firing Speed of the Grappling Hook
	float m_hookRange; //The Max range of the Grappling Hook

	bool m_hit; //Has the Grappling Hook hit a Wall?
	bool m_fired; //Has the Grappling Hook fired?

	float m_retractSpeed; //The Speed that the Player is pulled towards the Grappling Hook
	float m_retractMinimum; //The Minimum Range the Grappling Hook should retract before it is fully retracted
};

#endif // !_GRAPPINGHOOK_H_