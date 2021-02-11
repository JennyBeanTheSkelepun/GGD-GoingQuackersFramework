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
	m_position = Vector2(0, 0);
	m_rotation = 0;
	m_scale = Vector2(1, 1);
}

void Transform::Update()
{
	UpdateMatrix();
}

void Transform::UpdateMatrix()
{
	m_worldMatrix = DirectX::XMMatrixScaling(m_scale.X, m_scale.Y, 0.0f) * DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, m_rotation)
		* DirectX::XMMatrixTranslation(m_position.X, m_position.Y, 0.0f);
}
