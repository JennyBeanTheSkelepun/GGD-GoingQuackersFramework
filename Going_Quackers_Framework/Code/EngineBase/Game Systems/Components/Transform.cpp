#include "Transform.h"
#include <DirectXMath.h>
#include "../GameObject.h"
#include <math.h>

Transform::Transform(GameObject* owner) : Component(owner, ComponentTypes::TRANSFORM, "Transform")
{
	localToWorldMatrix = DirectX::XMMatrixIdentity();
	worldToLocalMatrix = DirectX::XMMatrixIdentity();

	m_position = Vector2(0.0f, 0.0f);
	m_rotation = 0.0f;
	mf_scale = Vector2(1.0f, 1.0f);
	m_localPosition = Vector2(0.0f, 0.0f);
	m_localRotation = 0.0f;
	m_localScale = Vector2(1.0f, 1.0f);

	m_posImGui = Vector2(0.0f, 0.0f);
	m_roationImGui = 0.0f;
	m_scaleImGui = Vector2(1.0f, 1.0f);
	m_localPositionImGui = Vector2(0.0f, 0.0f);
	m_localRotationImGui = 0.0f;
	m_localScaleImGui = Vector2(1.0f, 1.0f);
}

Transform::~Transform()
{

}

void Transform::OnDestroy()
{
	this->~Transform();
}

void Transform::Update()
{
	Vector2 temp;
	m_position = PosToLocalSpace(temp);
}

void Transform::ImGUIUpdate()
{
	if (ImGui::TreeNode("Show Settings"))
	{
		ImGui::Checkbox("Show Global Values", &ImGuiShowGlobal);
		//ImGui::Checkbox("Use Slider Inputs", &ImGuiSliderInput);
		//ImGui::Checkbox("Use Drag Inputs", &ImGuiDragInput);
		//ImGui::Checkbox("Use Keyboard Input", &ImGuiTextInput);

		if (ImGuiShowGlobal)
		{
			Vector2 tempPos, tempScale;
			tempPos = GetPosition();
			tempScale = GetScale();

			std::string Output = "Global Position X " + std::to_string(tempPos.X) + " Y: " + std::to_string(tempPos.Y);
			ImGui::Text(Output.c_str());
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	//Position Set
	ImGui::PushID(0);
	float* position[2] = { &m_localPositionImGui.X, &m_localPositionImGui.Y };
	ImGui::InputFloat2("Position", position[0]);
	ImGui::SameLine();
	if (ImGui::Button("Apply Changes"))
	{
		SetLocalPosition(m_localPositionImGui);
	}
	ImGui::PopID();

	//Rotation Set
	ImGui::PushID(1);
	ImGui::InputFloat("Rotation", &m_localRotationImGui);
	ImGui::SameLine();
	if (ImGui::Button("Apply Changes"))
	{
		SetLocalRotation(m_localRotationImGui);
	}
	ImGui::PopID();

	//Scale Set
	ImGui::PushID(2);
	float* scale[2] = { &m_localScaleImGui.X, &m_localScaleImGui.Y };
	ImGui::InputFloat2("Scale", scale[0]);
	ImGui::SameLine();
	if (ImGui::Button("Apply Changes"))
	{
		SetLocalScale(m_localScaleImGui);
	}
	ImGui::PopID();


}

json* Transform::SceneSave()
{
	json* returnObj = new json(
	{
		{"PositionX", GetPosition().X},
		{"PositionY", GetPosition().Y},
		{"Rotation", GetRotation()},
		{"ScaleX", GetScale().X},
		{"ScaleY", GetScale().Y},

		{"LocalPositionX", GetLocalPosition().X},
		{"LocalPositionY", GetLocalPosition().Y},
		{"LocalRotation", GetLocalRotation()},
		{"LocalScaleX", GetLocalScale().X},
		{"LocalScaleY", GetLocalScale().Y},
	});

	return returnObj;
}

void Transform::SceneLoad(json* componentJSON)
{
	SetPosition(Vector2((*componentJSON)["PositionX"], (*componentJSON)["PositionY"]));
	SetRotation((*componentJSON)["Rotation"]);
	SetScale(Vector2((*componentJSON)["ScaleX"], (*componentJSON)["ScaleY"]));

	SetLocalPosition(Vector2((*componentJSON)["LocalPositionX"], (*componentJSON)["LocalPositionY"]));
	SetLocalRotation((*componentJSON)["LocalRotation"]);
	SetLocalScale(Vector2((*componentJSON)["LocalScaleX"], (*componentJSON)["LocalScaleY"]));
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

Vector2 Transform::PosToLocalSpace(Vector2& point)
{

	if (this->GetOwner()->GetParent() != nullptr)
	{
		DirectX::XMFLOAT4X4 temp;
		DirectX::XMStoreFloat4x4(&temp, localToWorldMatrix);
		point.X = temp._41;
		point.Y = temp._42;
	}
	return point;
}