#include "GravityEmitter.h"
#include "Rigidbody.h"

void GravityEmitter::applyGravity(GameObject* callObj, std::vector<GameObject*>* CollidedObjects)
{
	Vector2 gravityPosition = callObj->GetTransform()->GetPosition();
	
	for (GameObject* obj : *CollidedObjects)
	{
		Rigidbody* rb = obj->GetComponent<Rigidbody>();
		if (rb != nullptr)
		{

			switch (m_gravType)
			{
			case GravityTypes::DIRECTION:
				rb->AddForce(m_gravityDirection * m_gravityStrength);
				break;
			case GravityTypes::CENTRE:
				Vector2 force = (gravityPosition - obj->GetTransform()->GetPosition()).Normalize() * m_gravityStrength;
				obj->GetComponent<Rigidbody>()->AddForce(force);
				break;
			}
		}
	}
}

void GravityEmitter::LoadGravType(std::string gravType)
{
	m_GravTypeDropdownSelected = gravType;

	if (m_GravTypeDropdownSelected == "Direction")
	{
		m_gravType = GravityTypes::DIRECTION;
	}
	else if (m_GravTypeDropdownSelected == "Centre")
	{
		m_gravType = GravityTypes::CENTRE;
	}
}

void GravityEmitter::ImGuiSetup()
{
	ImGui::InputFloat("Gravity Strength", &m_gravityStrength);

	if (ImGui::BeginCombo("Gravity Type", m_GravTypeDropdownSelected.c_str()))
	{
		for (int i = 0; i < IM_ARRAYSIZE(m_GravTypeDropdown); i++)
		{
			bool is_selected = (m_GravTypeDropdownSelected == m_GravTypeDropdown[i]);

			if (ImGui::Selectable(m_GravTypeDropdown[i].c_str(), is_selected))
			{
				m_GravTypeDropdownSelected = m_GravTypeDropdown[i];

				if (m_GravTypeDropdownSelected == "Direction")
				{
					m_gravType = GravityTypes::DIRECTION;
				}
				else if (m_GravTypeDropdownSelected == "Centre")
				{
					m_gravType = GravityTypes::CENTRE;
				}
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}
}
