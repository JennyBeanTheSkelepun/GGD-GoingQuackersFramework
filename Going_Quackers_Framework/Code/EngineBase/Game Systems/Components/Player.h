#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "Component.h"

class Player : public Component
{
public:
	enum class GRAPPLE_STATE
	{
		INACTIVE = 0,
		EXTENDING = 1,
		ATTACHED = 2,
		RETRACTING = 3,
		RETURNING = 4,
	};
	Player(GameObject* owner);
	void ImGUIDisplay() override;
	void Update() override;
	GRAPPLE_STATE GetGrappleState();
	void SetGrappleState(GRAPPLE_STATE state);
private:
	GRAPPLE_STATE m_grappleState;
};

#endif // !PLAYER_H