#pragma once
#include "Component.h"

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
};