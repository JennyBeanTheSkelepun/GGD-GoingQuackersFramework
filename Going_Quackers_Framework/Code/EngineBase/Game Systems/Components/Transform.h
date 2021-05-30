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

	///<summary>Returns a matrix that converts from local space to world space</summary>
	DirectX::XMMATRIX GetLocalToWorldMatrix();
	///<summary>Returns a matrix that converts from world space to local space</summary>
	DirectX::XMMATRIX GetWorldToLocalMatrix();

	///<summary>Transforms a point from local space to world space</summary>
	Vector2 TransformPoint(Vector2 point);
	///<summary>Transforms a point from world space to local space</summary>
	Vector2 InverseTransformPoint(Vector2 point);

	//Setters and Getters
	void SetPosition(Vector2 position) { m_localPositionImGui = this->m_localPosition = InverseTransformPoint(position); this->m_position = position; }
	Vector2 GetPosition() { return m_position; }
	void SetLocalPosition(Vector2 position) { m_localPositionImGui = this->m_localPosition = position; this->m_position = TransformPoint(position); }
	Vector2 GetLocalPosition() { return this->m_localPosition; }

	void SetRotation(double rotation) { this->m_rotation = rotation; }
	double GetRotation() { return m_rotation; }
	void SetLocalRotation(double rotation) { m_localRotationImGui = this->m_localRotation = rotation; }
	double GetLocalRotation() { return this->m_localRotation; }

	void SetScale(Vector2 scale) { this->mf_scale = scale; }
	Vector2 GetScale() { return this->mf_scale; }
	void SetLocalScale(Vector2 scale) { m_localScaleImGui = this->m_localScale = scale; }
	Vector2 GetLocalScale() { return this->m_localScale; }

private:
	Vector2 PosToLocalSpace(Vector2& point);

	bool ImGuiShowGlobal;
	bool ImGuiSliderInput;
	bool ImGuiDragInput;
	bool ImGuiTextInput;

	//World Positions
	Vector2 m_position, m_posImGui;
	float m_rotation, m_roationImGui;
	Vector2 mf_scale, m_scaleImGui;

	//Local Positions
	Vector2 m_localPosition, m_localPositionImGui;
	float m_localRotation, m_localRotationImGui;
	Vector2 m_localScale, m_localScaleImGui;

	//Transform that converts from local space to world space
	DirectX::XMMATRIX localToWorldMatrix;

	//Transform that converts from world space to local space
	DirectX::XMMATRIX worldToLocalMatrix;

	//Creates a Matrix that converts from local space to the coordinate space of the parent transform (If there is one) 
	DirectX::XMMATRIX CalculateLocalMatrix();
};

#endif