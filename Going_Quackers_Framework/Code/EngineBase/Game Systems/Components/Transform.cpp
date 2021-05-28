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
}

void Transform::ImGUIUpdate()
{
	if (ImGui::TreeNode("Local"))
	{
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

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Global"))
	{
		//Position Set
		ImGui::PushID(0);
		float* position[2] = { &m_posImGui.X, &m_posImGui.Y };
		ImGui::InputFloat2("Position", position[0]);
		ImGui::SameLine();
		if (ImGui::Button("Apply Changes"))
		{
			SetGlobalPosition(m_posImGui);
		}
		ImGui::PopID();

		//Rotation Set
		ImGui::PushID(1);
		ImGui::InputFloat("Rotation", &m_roationImGui);
		ImGui::SameLine();
		if (ImGui::Button("Apply Changes"))
		{
			SetGlobalRotation(m_roationImGui);
		}
		ImGui::PopID();

		//Scale Set
		ImGui::PushID(2);
		float* scale[2] = { &m_scaleImGui.X, &m_scaleImGui.Y };
		ImGui::InputFloat2("Scale", scale[0]);
		ImGui::SameLine();
		if (ImGui::Button("Apply Changes"))
		{
			SetGlobalScale(m_scaleImGui);
		}
		ImGui::PopID();

		ImGui::TreePop();
	}
}

json* Transform::SceneSave()
{
	json* returnObj = new json(
	{
		{"PositionX", GetGlobalPosition().X},
		{"PositionY", GetGlobalPosition().Y},
		{"Rotation", GetGlobalRotation()},
		{"ScaleX", GetGlobalScale().X},
		{"ScaleY", GetGlobalScale().Y},

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
	SetGlobalPosition(Vector2((*componentJSON)["PositionX"], (*componentJSON)["PositionY"]));
	SetGlobalRotation((*componentJSON)["Rotation"]);
	SetGlobalScale(Vector2((*componentJSON)["ScaleX"], (*componentJSON)["ScaleY"]));

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
		point -= mp_owner->GetParent()->GetTransform()->GetGlobalPosition();
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
		point /= mp_owner->GetParent()->GetTransform()->GetGlobalScale();
		mp_owner->GetParent()->GetTransform()->ScaleToGlobalSpace(point);
	}
	return point;
}

float Transform::RotationToLocalSpace(float& point)
{
	if (mp_owner->GetParent() != nullptr)
	{
		//point -= mp_owner->GetParent()->GetTransform()->GetLocalRotation();
		//mp_owner->GetParent()->GetTransform()->RotationToLocalSpace(point);
	}
	return point;
}

float Transform::RotationToGlobalSpace(float& point)
{
	if (mp_owner->GetParent() != nullptr)
	{
		//point += mp_owner->GetParent()->GetTransform()->GetGlobalRotation();
		//mp_owner->GetParent()->GetTransform()->RotationToGlobalSpace(point);
	}
	return point;
}

void Transform::UpdateChildTransforms()
{
	std::vector<GameObject*> children = GetOwner()->GetChildren();
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->GetTransform()->SetGlobalPosition(children[i]->GetTransform()->GetLocalPosition());
		children[i]->GetTransform()->SetLocalScale(Vector2(1.0f, 1.0f));
		children[i]->GetTransform()->SetGlobalScale(children[i]->GetTransform()->GetLocalScale());
		children[i]->GetTransform()->SetGlobalRotation(children[i]->GetTransform()->GetLocalRotation());
	}
}
