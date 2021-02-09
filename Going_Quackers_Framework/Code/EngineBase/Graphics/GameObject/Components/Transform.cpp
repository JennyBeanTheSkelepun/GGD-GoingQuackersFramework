#include "Transform.h"
#include <DirectXMath.h>

Transform::Transform(GameObject* owner) : Component(owner)
{
	m_worldMatrix = DirectX::XMMatrixIdentity();
}

Transform::~Transform()
{
}

void Transform::Initialize()
{
	m_position = Vector3(0, 0, 0);
	m_rotation = 0;
	m_scale = Vector3(1, 1, 1);
}

void Transform::Update()
{
	UpdateMatrix();
}

void Transform::UpdateMatrix()
{
	m_worldMatrix = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, m_rotation)
		* DirectX::XMMatrixTranslation(m_position.x, m_position.y, 1.0f);
}
