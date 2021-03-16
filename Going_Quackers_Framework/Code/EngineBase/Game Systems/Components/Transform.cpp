#include "Transform.h"
#include <DirectXMath.h>
#include "../GameObject.h"

Transform::Transform(GameObject* owner) : Component(owner, ComponentTypes::TRANSFORM)
{
	localToWorldMatrix = DirectX::XMMatrixIdentity();
	worldToLocalMatrix = DirectX::XMMatrixIdentity();
}

void Transform::Initialize()
{
	m_position = Vector2(0.0f, 0.0f);
	m_rotation = 0.0f;
	m_scale = Vector2(1.0f, 1.0f);
	m_localPosition = Vector2(0.0f, 0.0f);
	m_localRotation = 0.0f;
	m_localScale = Vector2(1.0f, 1.0f);
}

void Transform::ImGUIUpdate()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position");
		float pVec2[2] = { m_localPosition.X, m_localPosition.Y };
		ImGui::InputFloat2("Position", pVec2);
		SetLocalPosition(Vector2(pVec2[0], pVec2[1]));

		ImGui::Text("Rotation");
		float rVec1[1] = { m_localRotation };
		ImGui::InputFloat("Rotation", rVec1);
		SetLocalRotation(rVec1[0]);

		ImGui::Text("Scale");
		float sVec2[2] = { m_localScale.X, m_localScale.Y };
		ImGui::InputFloat2("Scale", sVec2);
		SetLocalScale(Vector2(sVec2[0], sVec2[1]));
	}
}

DirectX::XMMATRIX Transform::GetLocalToWorldMatrix()
{
	GameObject* parent = mp_owner->GetParent();

	if (parent == nullptr || parent == mp_owner)
	{
		localToWorldMatrix = CalculateLocalMatrix();
	}
	else
	{
		localToWorldMatrix = CalculateLocalMatrix() * parent->transform->GetLocalToWorldMatrix();
	}

	return localToWorldMatrix;
}

DirectX::XMMATRIX Transform::GetWorldToLocalMatrix()
{
	worldToLocalMatrix = DirectX::XMMatrixInverse(nullptr, GetLocalToWorldMatrix());
	return worldToLocalMatrix;
}

Vector2 Transform::TransformPoint(Vector2 point)
{
	DirectX::XMFLOAT4 floatTemp;
	DirectX::XMVECTOR tempVector = DirectX::XMVector4Transform(DirectX::XMVectorSet(point.X, point.Y, 0.0f, 1.0f), GetLocalToWorldMatrix());
	DirectX::XMStoreFloat4(&floatTemp, tempVector);
	return Vector2(floatTemp.x, floatTemp.y);
}

Vector2 Transform::InverseTransformPoint(Vector2 point)
{
	DirectX::XMFLOAT4 floatTemp;
	DirectX::XMVECTOR tempVector = DirectX::XMVector4Transform(DirectX::XMVectorSet(point.X, point.Y, 0.0f, 1.0f), GetWorldToLocalMatrix());
	DirectX::XMStoreFloat4(&floatTemp, tempVector);
	return Vector2(floatTemp.x, floatTemp.y);
}

DirectX::XMMATRIX Transform::CalculateLocalMatrix()
{
	return DirectX::XMMatrixScaling(m_localScale.X, m_localScale.Y, 1.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, m_localRotation * (DirectX::XM_PI / 180.0f)) *
		DirectX::XMMatrixTranslation(m_localPosition.X, m_localPosition.Y, 0.0f);
}

