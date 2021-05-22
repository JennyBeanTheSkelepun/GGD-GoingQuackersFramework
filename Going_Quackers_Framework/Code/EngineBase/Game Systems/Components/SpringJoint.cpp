#include "SpringJoint.h"
#include "../GameObject.h"
#include "../Debug.h"
#include "Physics/Rigidbody.h"
#include <cstdlib>

SpringJoint::SpringJoint(GameObject* ap_owner) : Component(ap_owner, ComponentTypes::SPRINGJOINT) {
	mp_jointObjectNameField = new char[100] { "" };

	mp_desiredLengthField = new char[100] { "1" };
	mf_desiredLength = 1;

	mp_strengthField = new char[100] { "1" };
	mf_strength = 1;
}

SpringJoint::~SpringJoint() {
	if (mp_connectedObject != nullptr)
		mp_connectedObject->SetParent(nullptr);
}

void SpringJoint::SetConnectedObject(GameObject* ap_connectedObject) {
	if (ap_connectedObject != nullptr && ap_connectedObject->GetComponent<Rigidbody>() == nullptr) {
		Debug::getInstance()->LogWarning("Cannot add \"" + ap_connectedObject->GetName() + "\" to " + GetOwner()->GetName() + "as a spring joint without a RigidBody.");
		return;
	}

	mp_connectedObject = ap_connectedObject;

	if (mp_connectedObject == nullptr)
		return;

	//mp_connectedObject->SetParent(nullptr);
	GetOwner()->AddChild(mp_connectedObject);

	//TODO - check the child has a valid rigidbody before adding it
}

void SpringJoint::SetStrength(float af_strength) {
	mf_strength = af_strength;
}

void SpringJoint::SetDesiredLength(float af_desiredLength) {
	mf_desiredLength = af_desiredLength;
}

void SpringJoint::SetSpringType(SpringType a_type) {
	m_type = a_type;
}

void SpringJoint::SetSpringMode(SpringMode a_mode) {
	m_mode = a_mode;
}

void SpringJoint::Update() {
	if (mp_connectedObject == nullptr)
		return;

	//temp for testing
	Debug::getInstance()->LogWarning("sdfsdfsdf");
	GetOwner()->GetComponent<Rigidbody>()->AddForce(Vector2(1, 0));

	float currentLength = GetOwner()->GetTransform()->GetPosition().Distance(mp_connectedObject->GetTransform()->GetPosition());

	if (currentLength < mf_desiredLength && m_mode != SpringMode::REPEL_ONLY ||
		currentLength > mf_desiredLength && m_mode != SpringMode::ATTRACT_ONLY) {
		if (m_type == SpringType::FIXED_HEAD) {
			//ApplyFixedHeadSpringForce(currentLength);
		}
		else
			ApplyNonFixedHeadSpringForce(currentLength);
	}
}

void SpringJoint::ImGUIUpdate() {
	ImGui::InputText(((std::string)"Enter joint object name").c_str(), mp_jointObjectNameField, 128);
	ImGui::InputText(((std::string)"Enter desired length").c_str(), mp_desiredLengthField, 128);
	ImGui::InputText(((std::string)"Enter strength").c_str(), mp_strengthField, 128);

	if (ImGui::Button("Update joint")) {
		//Connected joint
		std::string objectNameField(mp_jointObjectNameField);

		if (objectNameField == GetOwner()->GetName()) {
			Debug::getInstance()->LogWarning("Cannot join \"" + objectNameField + "\" to itself");
			return;
		}

		std::vector<GameObject*> sceneObjects = SceneManager::GetInstance()->GetCurrentScene()->GetSceneObjects();

		for (size_t i = 0; i < sceneObjects.size(); i++) {
			//TODO - Remove string cleaning section when name GetName bug is fixed
			std::string objectName = sceneObjects[i]->GetName();
			int garbageStart = objectName.find('\0');

			if (garbageStart != std::string::npos)
				objectName.erase(objectName.find('\0'));
			//End of string cleaning

			if (objectName == objectNameField) {
				if (mp_connectedObject != sceneObjects[i])
					SetConnectedObject(sceneObjects[i]);

				return;
			}
		}

		Debug::getInstance()->LogWarning("No match for \"" + objectNameField + "\" found in this scene");
		SetConnectedObject(nullptr);

		//Strength and length
		SetDesiredLength(std::stof(mp_desiredLengthField));
		SetStrength(std::stof(mp_strengthField));
	}
}

void SpringJoint::ApplyFixedHeadSpringForce(float af_currentStretch) {
	Vector2 position = GetOwner()->GetTransform()->GetPosition();
	Vector2 direction = (position - mp_connectedObject->GetTransform()->GetPosition()).Normalize(); //TODO - check this is the right way around

	float force = mf_strength * af_currentStretch;

	mp_connectedObject->GetComponent<Rigidbody>()->AddForce(direction * force);
}

void SpringJoint::ApplyNonFixedHeadSpringForce(float af_currentStretch) {
}
