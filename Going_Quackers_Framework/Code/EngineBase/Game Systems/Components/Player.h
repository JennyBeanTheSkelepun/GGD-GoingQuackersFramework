#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "Component.h"

class Player : public Component
{
public:
	enum class GRAPPLE_STATE
	{
		INACTIVE = 0, // no target, not moving
		EXTENDING = 1, // has a target and is approaching it
		ATTACHED = 2, // has attached to target
		RETRACTING = 3, // is attached to target, player is being pulled towards target
		RETURNING = 4, // detached from target, grapple is returning to player
	};

	Player(GameObject* owner);
	~Player();

	void Update() override;
	void OnDestroy() override;
	void ImGUIUpdate() override;
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;
	
	GRAPPLE_STATE GetGrappleState() { return m_grappleState; };
	void SetGrappleState(GRAPPLE_STATE state);
private:
	GRAPPLE_STATE m_grappleState;
};

#endif // !PLAYER_H