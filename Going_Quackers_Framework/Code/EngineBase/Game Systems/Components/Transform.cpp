#include "Transform.h"
#include <DirectXMath.h>
#include "../GameObject.h"
#include <math.h>

Transform::Transform(GameObject* owner) : Component(owner, ComponentTypes::TRANSFORM, "Transform")
{
	m_position = Vector2(0.0f, 0.0f);
	m_rotation = 0.0f;
	mf_scale = Vector2(1.0f, 1.0f);
	m_localPosition = Vector2(0.0f, 0.0f);
	m_localRotation = 0.0f;
	m_localScale = Vector2(1.0f, 1.0f);
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
}

void Transform::ImGUIUpdate()
{
	if (ImGui::TreeNode("Local"))
	{
		//Position Set
		float* position[2] = { &m_localPosition.X, &m_localPosition.Y };
		ImGui::InputFloat2("Position", position[0]);
		SetPosition(Vector2(*position[0], *position[1]));

		//Rotation Set
		ImGui::InputFloat("Rotation", &m_localRotation);

		//Scale Set
		float scale[2] = { m_localScale.X, m_localScale.Y };
		ImGui::InputFloat2("Scale", scale);
		SetScale(Vector2(scale[0], scale[1]));
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Global"))
	{
		//Position Set
		float* position[2] = { &m_position.X, &m_position.Y };
		ImGui::InputFloat2("Position", position[0]);
		//SetLocalPosition(Vector2(*position[0], *position[1]));

		//Rotation Set
		ImGui::InputFloat("Rotation", &m_rotation);

		//Scale Set
		float scale[2] = { mf_scale.X, mf_scale.Y };
		ImGui::InputFloat2("Scale", scale);
		//SetLocalScale(Vector2(scale[0], scale[1]));
		ImGui::TreePop();
	}
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

Vector2 Transform::PosToLocalSpace(Vector2& point)
{
	if (mp_owner->GetParent() != nullptr)
	{
		point += mp_owner->GetParent()->GetTransform()->GetLocalPosition();
		mp_owner->GetParent()->GetTransform()->PosToLocalSpace(point);
	}
	return point;
}

Vector2 Transform::PosToGlobalSpace(Vector2& point)
{
	if (mp_owner->GetParent() != nullptr)
	{
		point -= mp_owner->GetParent()->GetTransform()->GetPosition();
		mp_owner->GetParent()->GetTransform()->PosToGlobalSpace(point);
	}
	return point;
}

Vector2 Transform::ScaleToLocalSpace(Vector2& point)
{
	if (mp_owner->GetParent() != nullptr)
	{
		point *= mp_owner->GetParent()->GetTransform()->GetLocalScale();
		mp_owner->GetParent()->GetTransform()->ScaleToLocalSpace(point);
	}
	return point;
}

Vector2 Transform::ScaleToGlobalSpace(Vector2& point)
{
	if (mp_owner->GetParent() != nullptr)
	{
		point /= mp_owner->GetParent()->GetTransform()->GetScale();
		mp_owner->GetParent()->GetTransform()->ScaleToGlobalSpace(point);
	}
	return point;
}

float Transform::RotationToLocalSpace(float& point)
{
	if (mp_owner->GetParent() != nullptr)
	{
		point += mp_owner->GetParent()->GetTransform()->GetLocalRotation();
		mp_owner->GetParent()->GetTransform()->RotationToLocalSpace(point);
	}
	return point;
}

float Transform::RotationToGlobalSpace(float& point)
{
	if (mp_owner->GetParent() != nullptr)
	{
		point -= mp_owner->GetParent()->GetTransform()->GetRotation();
		mp_owner->GetParent()->GetTransform()->RotationToGlobalSpace(point);
	}
	return point;
}