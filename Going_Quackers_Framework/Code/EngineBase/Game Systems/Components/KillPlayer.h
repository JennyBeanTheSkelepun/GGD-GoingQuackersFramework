#ifndef KILLPLAYER_H
#define KILLPLAYER_H
#pragma once

#include "Component.h"
#include "Physics/Trigger.h"
#include "../GameObject.h"
#include "Player.h"

class KillPlayer : public Component
{
public:
	KillPlayer(GameObject* owner);
	~KillPlayer();

	void Update() override;
	void OnDestroy() override;
	void ImGUIUpdate() override;
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;
private:
	void KillThePlayer();
	GameObject* KillObj;
	std::vector<GameObject*> playerObj;
	Rigidbody* killRB;
};


#endif // !KILLPLAYER_H
