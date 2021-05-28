#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Component.h"
#include "../../Data Structures/Vectors.h"

#include <DirectXMath.h>

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


	//float RotationWorldSpace(float point);
	//float RotationLocalSpace(float point);
	//float UpdateRotation();

	//Setters and Getters
	void SetPosition(Vector2 position) { this->m_localPosition = PosToLocalSpace(position); this->m_position = position; }
	Vector2 GetPosition() { return m_position; }
	void SetLocalPosition(Vector2 position) { this->m_localPosition = position; this->m_position = PosToGlobalSpace(position); }
	Vector2 GetLocalPosition() { return this->m_localPosition; }

	void SetRotation(double rotation) { this->m_rotation = rotation; }
	float GetRotation() { return m_rotation; }
	void SetLocalRotation(double rotation) { this->m_localRotation = rotation; }
	float GetLocalRotation() { return this->m_localRotation; }

	void SetScale(Vector2 scale) { this->mf_scale = scale; }
	Vector2 GetScale() { return this->mf_scale; }
	void SetLocalScale(Vector2 scale) { this->m_localScale = scale; }
	Vector2 GetLocalScale() { return this->m_localScale; }

private:
	//World Positions
	Vector2 m_position;
	float m_rotation;
	Vector2 mf_scale;

	//Local Positions
	Vector2 m_localPosition;
	float m_localRotation;
	Vector2 m_localScale;
};

#endif