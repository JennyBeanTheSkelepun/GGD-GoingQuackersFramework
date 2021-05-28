#include "SpringJoint.h"
#include "../Input.h"
#include "../Debug.h"
#include "Physics/Rigidbody.h"

SpringJoint::SpringJoint(GameObject* ap_owner) : Component(ap_owner, ComponentTypes::SPRINGJOINT, "Spring Joint") {
	mf_defaultDesiredLength = 1;
	mf_desiredLength = 1;
	mf_strength = 1;
}

SpringJoint::~SpringJoint() {
	//TODO make sure connectedObjectField is properly destroyed/if it needs to be
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
	if (mb_checkForConnectedObject) {
		mp_connectedObject = ms_connectedObjectID == "NULL" ? nullptr : SceneManager::GetInstance()->GetCurrentScene()->GetObjectByID(ms_connectedObjectID);
		mp_connectedObject->GetName().copy(m_jointObjectNameField, mp_connectedObject->GetName().size());

		mb_checkForConnectedObject = false;
	}

	if (!EngineGuiClass::getInstance()->IsInPlayMode()) {
		mf_desiredLength = mf_defaultDesiredLength;
		return;
	}

	if (mp_connectedObject == nullptr)
		return;

	//Testing
	if (Input::getInstance()->isKeyPressedDown(KeyCode::D))
		mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Force{ Vector2(0.1f, 0), MovementIgnore::MASSACCEL});

	if (Input::getInstance()->isKeyPressedDown(KeyCode::A))
		mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Force{ Vector2(-0.1f, 0), MovementIgnore::MASSACCEL });

	if (Input::getInstance()->isKeyPressedDown(KeyCode::W))
		mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Force{ Vector2(0, 0.1f), MovementIgnore::MASSACCEL });

	if (Input::getInstance()->isKeyPressedDown(KeyCode::S))
		mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Force{ Vector2(0, -0.1f), MovementIgnore::MASSACCEL });

	if (Input::getInstance()->isKeyPressedDown(KeyCode::Space)) {
		Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
		connectedObjectRb->AddForce(Force{ -connectedObjectRb->GetVelocity(), MovementIgnore::MASSACCEL });
	}
	//End of testing

	Vector2 tailOffset = mp_connectedObject->GetTransform()->GetPosition() - GetOwner()->GetTransform()->GetPosition();
	float currentLength = tailOffset.Length();
	float nextTailPositionLength = (tailOffset + mp_connectedObject->GetComponent<Rigidbody>()->GetVelocity()).Length();

	if (mb_selfAdjustDesiredLength) {
		if (currentLength < mf_desiredLength && m_mode != SpringMode::REPEL_ONLY ||
			currentLength > mf_desiredLength && m_mode != SpringMode::ATTRACT_ONLY) {
			mf_desiredLength = currentLength;
		}
	}

	if (nextTailPositionLength < mf_desiredLength) {
		if (m_mode == SpringMode::REPEL_ONLY) {
			SnapTailToDesiredDistance();
		}
		else
			ApplyForce(currentLength);
	}
	else if (nextTailPositionLength > mf_desiredLength) {
		if (m_mode == SpringMode::ATTRACT_ONLY) {
			SnapTailToDesiredDistance();
		}
		else
			ApplyForce(currentLength);
	}
}

void SpringJoint::ImGUIUpdate() {
	ImGui::InputText("Enter joint object name", m_jointObjectNameField, IM_ARRAYSIZE(m_jointObjectNameField));
	ImGui::Separator();
	ImGui::InputFloat("Enter desired length", &mf_defaultDesiredLength, IM_ARRAYSIZE(&mf_defaultDesiredLength));
	ImGui::InputFloat("Enter strength", &mf_strength, IM_ARRAYSIZE(&mf_strength), 0.1f);
	ImGui::Separator();
	ImGui::Combo("Joint type", &mi_typeField, "Fixed\0Non fixed\0\0");
	ImGui::Combo("Mode type", &mi_modeField, "Attract & Repel\0Attract\0Repel\0\0");
	ImGui::Separator();
	ImGui::Checkbox("Self adjust desired distance", &mb_selfAdjustDesiredLength);
	ImGui::Separator();

	if (ImGui::Button("Update joint")) {
		m_type = static_cast<SpringType>(mi_typeField);
		m_mode = static_cast<SpringMode>(mi_modeField);

		SetCurrentDesiredLength(mf_desiredLength);

		//Connected joint
		std::string objectNameField(m_jointObjectNameField);

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
	ms_connectedObjectID = (*componentJSON)["ConnectedObjectID"];
	mb_checkForConnectedObject = true;

	mf_defaultDesiredLength = (*componentJSON)["DefaultDesiredLength"];
	mf_desiredLength = mf_defaultDesiredLength;

	mf_strength = (*componentJSON)["Strength"];

	m_type = (*componentJSON)["Type"];
	mi_typeField = (int)m_type;

	m_mode = (*componentJSON)["Mode"];
	mi_modeField = (int)m_mode;

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

void SpringJoint::SnapTailToDesiredDistance() {
	Transform* tailTransform = mp_connectedObject->GetTransform();
	Vector2 directionFromHead = (tailTransform->GetPosition() - GetOwner()->GetTransform()->GetPosition()).Normalize();

	tailTransform->SetPosition(directionFromHead * mf_desiredLength);

	Rigidbody* connectedObjectRb = mp_connectedObject->GetComponent<Rigidbody>();
	connectedObjectRb->AddForce(Force{ -connectedObjectRb->GetVelocity(), MovementIgnore::MASSACCEL });
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

	mp_connectedObject->GetComponent<Rigidbody>()->AddForce(Force{ directionFromHead * force, MovementIgnore::MASSACCEL });
}

void SpringJoint::ApplyNonFixedHeadSpringForce(float af_currentStretch) {
}