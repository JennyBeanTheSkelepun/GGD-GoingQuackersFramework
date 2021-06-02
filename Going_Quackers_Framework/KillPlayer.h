#ifndef KILLPLAYER_H
#define KILLPLAYER_H
#pragma once

#include "Code/EngineBase/Game Systems/Components/Component.h"
#include "Code/EngineBase/Game Systems/Components/Physics/Trigger.h"
#include "Code/EngineBase/Game Systems/GameObject.h"
#include "Code/EngineBase/Game Systems/Components/Player.h"

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
	GameObject* KillObject;
	std::vector<GameObject*> playerObj;


#endif // !KILLPLAYER_H
