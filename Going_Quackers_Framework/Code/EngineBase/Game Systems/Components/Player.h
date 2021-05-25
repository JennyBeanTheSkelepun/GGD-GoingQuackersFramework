#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "Component.h"
#include "GameObject.h"

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
	bool wallGrabbed;
	//void ImGUIDisplay() override;
	void Update() override;
	GRAPPLE_STATE GetGrappleState();
	void SetGrappleState(GRAPPLE_STATE state);
private:
	GRAPPLE_STATE m_grappleState;
	void GrabWall();
	Component PlayerBody;
};


#endif // !PLAYER_H