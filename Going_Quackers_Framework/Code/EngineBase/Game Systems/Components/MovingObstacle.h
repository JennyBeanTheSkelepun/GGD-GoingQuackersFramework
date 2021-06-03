#ifndef MOVINGOBSTACLE_H
#define MOVINGOBSTACLE_H
#pragma once
#include "Component.h"
#include "Physics/Rigidbody.h"

class MovingObstacle :
    public Component
{
public:
	MovingObstacle(GameObject* owner);
	~MovingObstacle();

	void OnDestroy() override;
	void Update() override;
	void ImGUIUpdate() override;
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;
};

#endif