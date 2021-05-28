#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Component.h"
#include "../../Data Structures/Vectors.h"

class Transform : public Component
{
public:
	Transform(GameObject* owner);
	~Transform();

	//- Basic Loops -//
	void OnDestroy() override;
	void Update() override;

	//- ImGui UpdateLoop -//
	void ImGUIUpdate() override;

	//- Scene Save and Load -//
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

	Vector2 PosToLocalSpace(Vector2& point);
	Vector2 PosToGlobalSpace(Vector2& point);
	Vector2 ScaleToLocalSpace(Vector2& point);
	Vector2 ScaleToGlobalSpace(Vector2& point);
	float RotationToLocalSpace(float& point);
	float RotationToGlobalSpace(float& point);

	//Setters and Getters
	void SetGlobalPosition(Vector2 position) { this->m_position = position; this->m_localPosition = this->m_localPositionImGui = PosToLocalSpace(position); UpdateChildTransforms(); }
	Vector2 GetGlobalPosition() { return m_position; }
	void SetLocalPosition(Vector2 position) { this->m_localPosition = position; this->m_position = this->m_posImGui = PosToGlobalSpace(position); UpdateChildTransforms(); }
	Vector2 GetLocalPosition() { return this->m_localPosition; }

	void SetGlobalRotation(float rotation) { this->m_rotation = rotation; this->m_localRotationImGui = RotationToLocalSpace(rotation); this->m_localRotation = this->m_localRotationImGui; UpdateChildTransforms(); }
	float GetGlobalRotation() { return m_rotation; }
	void SetLocalRotation(float rotation) { this->m_localRotation = rotation; this->m_roationImGui = RotationToGlobalSpace(rotation); this->m_rotation = this->m_roationImGui; UpdateChildTransforms(); }
	float GetLocalRotation() { return this->m_localRotation; }

	void SetGlobalScale(Vector2 scale) { this->mf_scale = scale;  this->m_localScale = this->m_localScaleImGui = ScaleToLocalSpace(scale); UpdateChildTransforms(); }
	Vector2 GetGlobalScale() { return this->mf_scale; }
	void SetLocalScale(Vector2 scale) { this->m_localScale = scale; this->mf_scale = this->m_scaleImGui = ScaleToGlobalSpace(scale); UpdateChildTransforms(); }
	Vector2 GetLocalScale() { return this->m_localScale; }

private:
	//World Positions
	Vector2 m_position, m_posImGui;
	float m_rotation, m_roationImGui;
	Vector2 mf_scale, m_scaleImGui;

	//Local Positions
	Vector2 m_localPosition, m_localPositionImGui;
	float m_localRotation, m_localRotationImGui;
	Vector2 m_localScale, m_localScaleImGui;

	void UpdateChildTransforms();
};

#endif