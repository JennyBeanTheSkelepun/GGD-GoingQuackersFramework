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
	void Update() override;

	//Creates a Matrix that converts from local space to the coordinate space of the parent transform
	DirectX::XMMATRIX CalculateLocalMatrix();

	//Gets the matrix that converts from local to world space
	DirectX::XMMATRIX GetLocalToWorldMatrix();

	DirectX::XMMATRIX GetWorldToLocalMatrix();

	//Setters and Getters
	void SetLocalPosition(Vector2 position) { this->m_localPosition = position; m_isDirty = true; }
	Vector2 GetLocalPosition() { return m_localPosition; }

	void SetLocalRotation(double rotation) { this->m_localRotation = rotation; m_isDirty = true; }
	double GetLocalRotation() { return m_localRotation; }

	void SetLocalScale(Vector2 scale) { this->m_localScale = scale; m_isDirty = true; }
	Vector2 GetLocalScale() { return m_localScale; }

	DirectX::XMMATRIX GetWorldMatrix() { return GetLocalToWorldMatrix(); }

private:
	//Position relative to the parent transform
	Vector2 m_localPosition;

	//Rotation relative to the parent transform
	double m_localRotation;

	//Scale releative to the parent transform
	Vector2 m_localScale;

	//Boolean to say if the local positions need to be recalculated
	bool m_isDirty;

	//Transform that converts from local to world space
	DirectX::XMMATRIX m_localToWorldMatrix;

	//Transform that converts from world to local space
	DirectX::XMMATRIX m_worldToLocalMatrix;
};

#endif