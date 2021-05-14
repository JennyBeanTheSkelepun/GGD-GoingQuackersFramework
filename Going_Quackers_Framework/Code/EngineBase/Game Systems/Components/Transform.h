#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Component.h"
#include "../../Data Structures/Vectors.h"

#include <DirectXMath.h>

class Transform : public Component
{
public:
	Transform(GameObject* owner);

	void Initialize() override;
	void ImGUIUpdate() override;

	///<summary>Returns a matrix that converts from local space to world space</summary>
	DirectX::XMMATRIX GetLocalToWorldMatrix();
	///<summary>Returns a matrix that converts from world space to local space</summary>
	DirectX::XMMATRIX GetWorldToLocalMatrix();

	///<summary>Transforms a point from local space to world space</summary>
	Vector2 TransformPoint(Vector2 point);
	///<summary>Transforms a point from world space to local space</summary>
	Vector2 InverseTransformPoint(Vector2 point);

	//Setters and Getters
	void SetPosition(Vector2 position) { this->m_localPosition = InverseTransformPoint(position); this->m_position = position; }
	Vector2 GetPosition() { return m_position; }
	void SetLocalPosition(Vector2 position) { this->m_localPosition = position; }
	Vector2 GetLocalPosition() { return this->m_localPosition; }

	void SetRotation(double rotation) { this->m_rotation = rotation; }
	double GetRotation() { return m_rotation; }
	void SetLocalRotation(double rotation) { this->m_localRotation = rotation; }
	double GetLocalRotation() { return this->m_localRotation; }

	void SetScale(Vector2 scale) { this->m_scale = scale; }
	Vector2 GetScale() { return this->m_scale; }
	void SetLocalScale(Vector2 scale) { this->m_localScale = scale; }
	Vector2 GetLocalScale() { return this->m_localScale; }

private:
	//World Positions
	Vector2 m_position;
	double m_rotation;
	Vector2 m_scale;

	//Local Positions
	Vector2 m_localPosition;
	double m_localRotation;
	Vector2 m_localScale;

	//Transform that converts from local space to world space
	DirectX::XMMATRIX localToWorldMatrix;

	//Transform that converts from world space to local space
	DirectX::XMMATRIX worldToLocalMatrix;

	//Creates a Matrix that converts from local space to the coordinate space of the parent transform (If there is one) 
	DirectX::XMMATRIX CalculateLocalMatrix();
};

#endif