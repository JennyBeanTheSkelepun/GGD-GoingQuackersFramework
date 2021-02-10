#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Component.h"
#include "../../../Maths/Vectors.h"

#include <DirectXMath.h>

class Transform : public Component
{
public:
	Transform(GameObject* owner);
	~Transform();

	void Initialize() override;
	void Update() override;

	void SetPosition(Vector2 position) { this->m_position = position; }
	void SetRotation(double rotation) { this->m_rotation = rotation; }
	void SetScale(Vector2 scale) { this->m_scale = scale; }
	Vector2 GetPosition() { return m_position; }
	double GetRotation() { return m_rotation; }
	Vector2 GetScale() { return m_scale; }

	DirectX::XMMATRIX GetWorldMatrix() { return m_worldMatrix; }

private:
	Vector2 m_position;
	double m_rotation;
	Vector2 m_scale;

	DirectX::XMMATRIX m_worldMatrix;

	void UpdateMatrix();
};

#endif