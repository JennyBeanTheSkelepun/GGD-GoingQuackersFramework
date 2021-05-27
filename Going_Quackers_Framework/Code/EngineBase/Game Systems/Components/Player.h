#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "../Components/Component.h"
#include "../GameObject.h"
#include "Physics/Rigidbody.h"

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
	~Player();
	void OnDestroy() override;
	void Update() override;
	void ImGUIUpdate() override;
	bool wallGrabbed;
	GRAPPLE_STATE GetGrappleState();
	void SetGrappleState(GRAPPLE_STATE state);
private:
	GRAPPLE_STATE m_grappleState;
	void GrabWall();
	GameObject* playerObj;
	std::vector<GameObject*> wallObj;
};

#endif // !PLAYER_H