#include "SpringJoint.h"
#include "../GameObject.h"
#include "../Input.h"
#include "../Debug.h"
#include "Physics/Rigidbody.h"

SpringJoint::SpringJoint(GameObject* ap_owner) : Component(ap_owner, ComponentTypes::SPRINGJOINTS, "Spring Joint") {
	mp_jointObjectNameField = new char[100]{ "" };

	mf_desiredLength = 1;
	mp_desiredLengthField = new char[100]{ "1" };

	mf_strength = 1;
	mp_strengthField = new char[100]{ "1" };
}

SpringJoint::~SpringJoint() {
	delete[] mp_jointObjectNameField;
	delete[] mp_desiredLengthField;
	delete[] mp_strengthField;
}

void SpringJoint::OnDestroy() {
	this->~SpringJoint();
}

void SpringJoint::SetConnectedObject(GameObject* ap_connectedObject) {
	if (ap_connectedObject != nullptr && ap_connectedObject->GetComponent<Rigidbody>() == nullptr) {
		Debug::getInstance()->LogWarning("Cannot add \"" + ap_connectedObject->GetName() + "\" to " + GetOwner()->GetName() + "as a spring joint without a RigidBody.");
		return;
	}

	mp_connectedObject = ap_connectedObject;
}

void SpringJoint::SetStrength(float af_strength) {
	mf_strength = af_strength;
}

void SpringJoint::SetDefaultDesiredLength(float af_desiredLength) {
	mf_defaultDesiredLength = af_desiredLength;
}

void SpringJoint::SetCurrentDesiredLength(float af_desiredLength) {
	mf_desiredLength = af_desiredLength;
}

void SpringJoint::SetSelfAdjustDesiredLength(bool ab_selfAdjusting) {
	mb_selfAdjustDesiredLength = ab_selfAdjusting;
}

void SpringJoint::SetSpringType(SpringType a_type) {
	m_type = a_type;
}

void SpringJoint::SetSpringMode(SpringMode a_mode) {
	m_mode = a_mode;
}

void SpringJoint::Update() {
	if (!EngineGuiClass::getInstance()->IsInPlayMode())
		return;

	if (mp_connectedObject == nullptr)
		return;

	//Testing
	if (Input::getInstance()->isKeyPressedDown(KeyCode::D)) {
		Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
		connectedObjectRb->setUseAccel(MovementIgnore::MASSACCEL);
		connectedObjectRb->AddForce(Vector2(0.1f, 0));
		connectedObjectRb->setUseAccel(MovementIgnore::NONE);
	}

	if (Input::getInstance()->isKeyPressedDown(KeyCode::A)) {
		Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
		connectedObjectRb->setUseAccel(MovementIgnore::MASSACCEL);
		connectedObjectRb->AddForce(Vector2(-0.1f, 0));
		connectedObjectRb->setUseAccel(MovementIgnore::NONE);
	}

	if (Input::getInstance()->isKeyPressedDown(KeyCode::W)) {
		Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
		connectedObjectRb->setUseAccel(MovementIgnore::MASSACCEL);
		connectedObjectRb->AddForce(Vector2(0, 0.1f));
		connectedObjectRb->setUseAccel(MovementIgnore::NONE);
	}

	if (Input::getInstance()->isKeyPressedDown(KeyCode::S)) {
		Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
		connectedObjectRb->setUseAccel(MovementIgnore::MASSACCEL);
		connectedObjectRb->AddForce(Vector2(0, -0.1f));
		connectedObjectRb->setUseAccel(MovementIgnore::NONE);
	}

	if (Input::getInstance()->isKeyPressedDown(KeyCode::Space)) {
		Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
		connectedObjectRb->setUseAccel(MovementIgnore::MASSACCEL);
		connectedObjectRb->AddForce(-connectedObjectRb->GetVelocity());
		mp_connectedObject->GetComponent<Rigidbody>()->setUseAccel(MovementIgnore::NONE);
	}
	//End of testing

	float currentLength = GetOwner()->GetTransform()->GetPosition().Distance(mp_connectedObject->GetTransform()->GetPosition());
	//Debug::getInstance()->Log("current length " + std::to_string(currentLength) + "                                            " + std::to_string(Time::GetTime()));
	
	/*if (mb_selfAdjustDesiredLength) {
		if (currentLength > mf_desiredLength && m_mode != SpringMode::ATTRACT_ONLY ||
			currentLength < mf_desiredLength && m_mode != SpringMode::REPEL_ONLY)
			mf_desiredLength = currentLength;
	}*/

	if (currentLength < mf_desiredLength) {
		if (mb_selfAdjustDesiredLength && m_mode != SpringMode::REPEL_ONLY)
			mf_desiredLength = currentLength;

		if (m_mode == SpringMode::ATTRACT_ONLY) {
			Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
			connectedObjectRb->setUseAccel(MovementIgnore::MASSACCEL);
			connectedObjectRb->AddForce(-connectedObjectRb->GetVelocity());
			connectedObjectRb->setUseAccel(MovementIgnore::NONE);
		}
		else
			ApplyForce(currentLength);
	}
	else if (currentLength > mf_desiredLength) {
		if (mb_selfAdjustDesiredLength && m_mode != SpringMode::ATTRACT_ONLY)
			mf_desiredLength = currentLength;

		if (m_mode == SpringMode::REPEL_ONLY) {
			Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
			connectedObjectRb->setUseAccel(MovementIgnore::MASSACCEL);
			connectedObjectRb->AddForce(-connectedObjectRb->GetVelocity());
			connectedObjectRb->setUseAccel(MovementIgnore::NONE);
		}
		else
			ApplyForce(currentLength);
	}

	/*if (currentLength < mf_desiredLength && m_mode != SpringMode::ATTRACT_ONLY ||
		currentLength > mf_desiredLength && m_mode != SpringMode::REPEL_ONLY)
		ApplyForce(currentLength);*/
}

void SpringJoint::ImGUIUpdate() {
	ImGui::InputText(((std::string)"Enter joint object name").c_str(), mp_jointObjectNameField, 128);
	ImGui::InputText(((std::string)"Enter desired length").c_str(), mp_desiredLengthField, 128);
	ImGui::InputText(((std::string)"Enter strength").c_str(), mp_strengthField, 128);

	ImGui::Combo("Joint type", &mi_typeField, "Fixed\0Non fixed\0");
	ImGui::Combo("Mode type", &mi_modeField, "Attract & Repel\0Attract\0Repel\0");
	ImGui::Checkbox("Self adjust desired distance", &mb_selfAdjustDesiredLength);

	if (ImGui::Button("Update joint")) {
		m_type = static_cast<SpringType>(mi_typeField);
		m_mode = static_cast<SpringMode>(mi_modeField);

		//Strength and length
		SetCurrentDesiredLength(std::stof(mp_desiredLengthField));
		SetStrength(std::stof(mp_strengthField));

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
	}
}

void SpringJoint::SceneLoad(json* componentJSON) {
	std::string objectID = (*componentJSON)["ConnectedObjectID"];
	mp_connectedObject = objectID == "NULL" ? nullptr : SceneManager::GetInstance()->GetCurrentScene()->GetObjectByID(objectID);

	mf_defaultDesiredLength = (*componentJSON)["DefaultDesiredLength"];
	mf_desiredLength = mf_defaultDesiredLength;

	mf_strength = (*componentJSON)["Strength"];
	m_type = (*componentJSON)["Type"];
	m_mode = (*componentJSON)["Mode"];

	mb_selfAdjustDesiredLength = (*componentJSON)["SelfAdjustDesiredLength"];
}

json* SpringJoint::SceneSave() {
	std::string objectID = mp_connectedObject == nullptr ? "NULL" : mp_connectedObject->GetID();

	json* returnObj = new json({
		{"ConnectedObjectID", objectID},
		{"DefaultDesiredLength", mf_defaultDesiredLength},
		{"Strength", mf_strength},
		{"Type", m_type},
		{"Mode", m_mode},
		{"SelfAdjustDesiredLength", mb_selfAdjustDesiredLength}
		});

	return returnObj;
}

void SpringJoint::ApplyForce(float af_currentStretch) {
	if (m_type == SpringType::FIXED_HEAD) {
		ApplyFixedHeadSpringForce(af_currentStretch);
	}
	else
		ApplyNonFixedHeadSpringForce(af_currentStretch);
}

void SpringJoint::ApplyFixedHeadSpringForce(float af_currentStretch) {
	Vector2 headPosition = GetOwner()->GetTransform()->GetPosition();
	Vector2 tailPosition = mp_connectedObject->GetTransform()->GetPosition();

	float distanceFromDesired = (mf_desiredLength - af_currentStretch);
	Vector2 directionFromHead = (tailPosition - headPosition).Normalize();
	float force = (mf_strength * distanceFromDesired);

	Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
	connectedObjectRb->setUseAccel(MovementIgnore::MASSACCEL);
	connectedObjectRb->AddForce((directionFromHead * force));
	connectedObjectRb->setUseAccel(MovementIgnore::NONE);
}

void SpringJoint::ApplyNonFixedHeadSpringForce(float af_currentStretch) {
}