#include "Transform.h"
#include <DirectXMath.h>
#include "../GameObject.h"

Transform::Transform(GameObject* owner) : Component(owner)
{
	m_isDirty = false;
	m_localToWorldMatrix = DirectX::XMMatrixIdentity();
	m_worldToLocalMatrix = DirectX::XMMatrixIdentity();
}

void Transform::Initialize()
{
	m_localPosition = Vector2(0.0f, 0.0f);
	m_localRotation = 0.0f;
	m_localScale = Vector2(1.0f, 1.0f);
}

void Transform::Update()
{
}

DirectX::XMMATRIX Transform::CalculateLocalMatrix()
{
	return DirectX::XMMatrixScaling(m_localScale.X, m_localScale.Y, 0.0f) * 
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, m_localRotation) * 
		DirectX::XMMatrixTranslation(m_localPosition.X, m_localPosition.Y, 0.0f);
}

DirectX::XMMATRIX Transform::GetLocalToWorldMatrix()
{
	if (m_isDirty)
	{
		GameObject* parent = mp_owner->GetParent();
		if (parent == nullptr || parent == mp_owner)
		{
			m_localToWorldMatrix = CalculateLocalMatrix();
		}
		else
		{
			m_localToWorldMatrix = parent->GetTransform()->GetLocalToWorldMatrix() * CalculateLocalMatrix();
		}

		m_isDirty = false;
	}

	return m_localToWorldMatrix;
}

DirectX::XMMATRIX Transform::GetWorldToLocalMatrix()
{
	return DirectX::XMMatrixInverse(nullptr, GetLocalToWorldMatrix());
}
