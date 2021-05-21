#include "Transform.h"
#include <DirectXMath.h>
#include "../GameObject.h"
#include <math.h>

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
	//Position Set
	float position[2] = { m_localPosition.X, m_localPosition.Y};
	ImGui::InputFloat2("Position", position);
	SetLocalPosition(Vector2(position[0], position[1]));

	//Rotation Set
	float rotation = m_localRotation;
	ImGui::InputFloat("Rotation", &m_localRotation);

	//Scale Set
	float scale[2] = { m_localScale.X, m_localScale.Y };
	ImGui::InputFloat2("Scale", scale);
	SetLocalScale(Vector2(scale[0], scale[1]));
}

json* Transform::SceneSave()
{
	json returnObj =
	{
		{"ComponentName", "Transform"},
		{"PositionX", GetPosition().X},
		{"PositionY", GetPosition().Y},
		{"Rotation", GetRotation()},
		{"ScaleX", GetScale().X},
		{"ScaleY", GetScale().Y},

		{"LocalPositionX", GetLocalPosition().X},
		{"LocaPositionY", GetLocalPosition().Y},
		{"LocalRotation", GetLocalRotation()},
		{"LocalScaleX", GetLocalScale().X},
		{"LocalScaleY", GetLocalScale().Y},
	};

	return &returnObj;
}

void Transform::SceneLoad(json* componentJSON)
{
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
		localToWorldMatrix = CalculateLocalMatrix() * parent->GetTransform()->GetLocalToWorldMatrix();
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
	if (mp_owner->GetParent() != nullptr)
		return mp_owner->GetParent()->GetTransform()->GetPosition() + point;
	else
		return point;

	/*
	DirectX::XMFLOAT4 floatTemp;
	DirectX::XMVECTOR tempVector = DirectX::XMVector4Transform(DirectX::XMVectorSet(point.X, point.Y, 0.0f, 1.0f), GetWorldToLocalMatrix());
	DirectX::XMStoreFloat4(&floatTemp, tempVector);
	return Vector2(floatTemp.x, floatTemp.y);
	*/
}

DirectX::XMMATRIX Transform::CalculateLocalMatrix()
{
	return DirectX::XMMatrixScaling(m_localScale.X, m_localScale.Y, 1.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, m_localRotation * (DirectX::XM_PI / 180.0f)) *
		DirectX::XMMatrixTranslation(m_localPosition.X, m_localPosition.Y, 0.0f);
}

