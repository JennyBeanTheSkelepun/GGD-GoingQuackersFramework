#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Component.h"
#include "../../../Maths/Vector3.h"

class Transform : public Component
{
public:
	Transform(GameObject* owner);
	~Transform();

	void Initialize() override;
	void Update() override;

	void SetPosition(Vector3 position) { this->m_position = position; }
	void SetRotation(double rotation) { this->m_rotation = rotation; }
	void SetScale(Vector3 scale) { this->m_scale = scale; }
	Vector3 GetPosition() { return m_position; }
	double GetRotation() { return m_rotation; }
	Vector3 GetScale() { return m_scale; }

	DirectX::XMMATRIX GetWorldMatrix() { return m_worldMatrix; }

private:
	Vector3 m_position;
	double m_rotation;
	Vector3 m_scale;

	DirectX::XMMATRIX m_worldMatrix;

	void UpdateMatrix();
};

#endif